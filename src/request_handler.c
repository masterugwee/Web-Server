#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "request_handler.h"
#include "response.h"

void handle_request(int client_socket) {
    char buffer[1024] = {0}, method[16], path[256];

    read(client_socket, buffer, sizeof(buffer));
    parse_request(buffer, method, path);

    if (strcmp(method, "GET") == 0) {
        serve_file(client_socket, path);
    } else if (strcmp(method, "POST") == 0 || strcmp(method, "PUT") == 0) {
        const char* body = extract_body(buffer);
        if (body != NULL) {
            if (strcmp(method, "POST") == 0) {
                handle_post(client_socket, path, body);
            } else if (strcmp(method, "PUT") == 0) {
                handle_put(client_socket, path, body);
            }
        } else {
            const char* error_response = "<html><body>400 Bad Request</body></html>";
            send_response(client_socket, "400 Bad Request", "text/html; charset=UTF-8", error_response);
        }
    } else {
        const char* body = "<html><body>405 Method Not Allowed</body></html>";
        send_response(client_socket, "405 Method Not Allowed", "text/html; charset=UTF-8", body);
    }
}

void parse_request(char* request, char* method, char* path) {
    sscanf(request, "%s %s", method, path);
}

void serve_file(int client_socket, const char* path) {
    char full_path[512] = "./web";
    strcat(full_path, path);

    struct stat file_stat;
    if (stat(full_path, &file_stat) == -1 || S_ISDIR(file_stat.st_mode)) {
        const char* body = "<html><body>404 Not Found</body></html>";
        send_response(client_socket, "404 Not Found", "text/html; charset=UTF-8", body);
        return;
    }

    FILE* file = fopen(full_path, "r");
    if (!file) {
        const char* body = "<html><body>404 Not Found</body></html>";
        send_response(client_socket, "404 Not Found", "text/html; charset=UTF-8", body);
        return;
    }

    send_file_response(client_socket, "200 OK", "text/html; charset=UTF-8", file);
    fclose(file);
}

void handle_post(int client_socket, const char* path, const char* body) {
    char response_body[1024];
    snprintf(response_body, sizeof(response_body), "<html><body>Received POST data: %s</body></html>", body);
    send_response(client_socket, "200 OK", "text/html; charset=UTF-8", response_body);
}

void handle_put(int client_socket, const char* path, const char* body) {
    char response_body[1024];
    snprintf(response_body, sizeof(response_body), "<html><body>Received PUT data: %s</body></html>", body);
    send_response(client_socket, "200 OK", "text/html; charset=UTF-8", response_body);
}

const char* extract_body(const char* request) {
    const char* body = strstr(request, "\r\n\r\n");
    return body ? body + 4 : NULL;
}
