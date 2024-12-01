#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "request_handler.h"
#include "response.h"

void handle_request(int client_socket) {
    char buffer[1024] = {0}, method[16], path[256], host[256] = {0}, content_type[64] = {0};
    int content_length = 0;

    read(client_socket, buffer, sizeof(buffer));
    parse_request_headers(buffer, method, path, host, content_type, &content_length);

    // Ensure the Host header is present
    if (strlen(host) == 0) {
        send_response(client_socket, "400 Bad Request", "text/plain", "Missing Host header");
        return;
    }

    if (strcmp(method, "GET") == 0) {
        serve_file(client_socket, path);
    } else if (strcmp(method, "POST") == 0 || strcmp(method, "PUT") == 0) {
        // Ensure Content-Type and Content-Length are present for POST and PUT
        if (strlen(content_type) == 0 || content_length == 0) {
            send_response(client_socket, "400 Bad Request", "text/plain", "Missing Content-Type or Content-Length header");
            return;
        }
        const char* body = extract_body(buffer);
        if (body != NULL) {
            if (strcmp(method, "POST") == 0) {
                handle_post(client_socket, path, body);
            } else if (strcmp(method, "PUT") == 0) {
                handle_put(client_socket, path, body);
            }
        } else {
            send_response(client_socket, "400 Bad Request", "text/plain", "Invalid request body");
        }
    } else {
        send_response(client_socket, "405 Method Not Allowed", "text/plain", "Method not allowed");
    }
}

void parse_request_headers(const char* request, char* method, char* path, char* host, char* content_type, int* content_length) {
    sscanf(request, "%s %s", method, path);
    
    const char* header_start = strstr(request, "\r\n") + 2;  // Skip the request line
    const char* header_end;
    
    while ((header_end = strstr(header_start, "\r\n")) != NULL) {
        char header_line[256];
        strncpy(header_line, header_start, header_end - header_start);
        header_line[header_end - header_start] = '\0';
        
        if (strncmp(header_line, "Host:", 5) == 0) {
            sscanf(header_line, "Host: %s", host);
        } else if (strncmp(header_line, "Content-Type:", 13) == 0) {
            sscanf(header_line, "Content-Type: %s", content_type);
        } else if (strncmp(header_line, "Content-Length:", 15) == 0) {
            sscanf(header_line, "Content-Length: %d", content_length);
        }
        
        header_start = header_end + 2;  // Move to the next header line
    }
}

void serve_file(int client_socket, const char* path) {
    char full_path[512] = "./web";
    strcat(full_path, path);

    struct stat file_stat;
    if (stat(full_path, &file_stat) == -1 || S_ISDIR(file_stat.st_mode)) {
        send_response(client_socket, "404 Not Found", "text/html; charset=UTF-8", "<html><body>404 Not Found</body></html>");
        return;
    }

    FILE* file = fopen(full_path, "r");
    if (!file) {
        send_response(client_socket, "404 Not Found", "text/html; charset=UTF-8", "<html><body>404 Not Found</body></html>");
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
