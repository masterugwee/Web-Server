#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "response.h"

void send_response(int client_socket, const char* status, const char* content_type, const char* body) {
    char response[2048];
    snprintf(response, sizeof(response),
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n"
             "\r\n"
             "%s",
             status, content_type, strlen(body), body);
    write(client_socket, response, strlen(response));
}

void send_file_response(int client_socket, const char* status, const char* content_type, FILE* file) {
    char response[2048];
    char buffer[1024];
    size_t file_size;

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    snprintf(response, sizeof(response),
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n"
             "\r\n",
             status, content_type, file_size);
    write(client_socket, response, strlen(response));

    while (!feof(file)) {
        size_t bytes_read = fread(buffer, 1, sizeof(buffer), file);
        write(client_socket, buffer, bytes_read);
    }
}
