#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "response.h"

void send_response(int client_socket, const char* status, const char* content_type, const char* body) {
    char response[1024];

    snprintf(response, sizeof(response),
             "HTTP/1.0 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "\r\n"
             "%s",
             status, content_type, strlen(body), body);

    write(client_socket, response, strlen(response));
}

void send_file_response(int client_socket, const char* status, const char* content_type, FILE* file) {
    char header[512];
    char buffer[1024];
    size_t n;

    // Read file content
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    snprintf(header, sizeof(header),
             "HTTP/1.0 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "\r\n",
             status, content_type, file_size);

    write(client_socket, header, strlen(header));

    while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        write(client_socket, buffer, n);
    }
}
