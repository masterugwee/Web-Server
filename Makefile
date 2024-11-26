CC = gcc
CFLAGS = -Wall -Iinclude

all: server

server: src/main.o src/server.o src/request_handler.o src/response.o
	$(CC) -o server src/main.o src/server.o src/request_handler.o src/response.o

src/main.o: src/main.c include/server.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/server.o: src/server.c include/server.h include/request_handler.h
	$(CC) $(CFLAGS) -c src/server.c -o src/server.o

src/request_handler.o: src/request_handler.c include/request_handler.h include/response.h
	$(CC) $(CFLAGS) -c src/request_handler.c -o src/request_handler.o

src/response.o: src/response.c include/response.h
	$(CC) $(CFLAGS) -c src/response.c -o src/response.o

.PHONY: clean
clean:
	rm -f server src/*.o
