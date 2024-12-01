#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "request_handler.h"
#include "response.h"


int is_valid_content_type(const char* content_type)
 { regex_t regex;
  int reti;
   // Compile regular expression for MIME type validation
    reti = regcomp(&regex, "^(text|application|form)/(plain|data|html|css|javascript|json|x-www-form-urlencoded)$", REG_EXTENDED); 
    if (reti) { return 0; // regex compilation failed 
    } // Execute regular expression 
    reti = regexec(&regex, content_type, 0, NULL, 0);
    regfree(&regex); // Free compiled regular expression 
    if (!reti) { return 1;
     // valid content-type
    }
 return 0; // invalid content-type
  }


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

    // Validate the Content-Type for POST and PUT requests
    if ((strcmp(method, "POST") == 0 || strcmp(method, "PUT") == 0) && !is_valid_content_type(content_type)) {
        send_response(client_socket, "400 Bad Request", "text/plain", "Invalid Content-Type header");
        return;
    }

    // Ensure Content-Length is present for POST and PUT
    if ((strcmp(method, "POST") == 0 || strcmp(method, "PUT") == 0) && content_length <= 0) {
        send_response(client_socket, "400 Bad Request", "text/plain", "Invalid Content-Length header");
        return;
    }

    if (strcmp(method, "GET") == 0) {
        serve_file(client_socket, path);
    } else if (strcmp(method, "POST") == 0 || strcmp(method, "PUT") == 0) {
        const char* body = extract_body(buffer, content_length);
        if (body != NULL) {
            if (strcmp(method, "POST") == 0) {
                handle_post(client_socket, path, body);
            } else if (strcmp(method, "PUT") == 0) {
                handle_put(client_socket, path, body, content_length);
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

void handle_put(int client_socket, const char* path, const char* body, int content_length) {
    // Create the full file path
    char full_path[512] = "./web";
    strcat(full_path, path);

    // Open or create the file
    FILE* file = fopen(full_path, "w");
    if (!file) {
        send_response(client_socket, "500 Internal Server Error", "text/html; charset=UTF-8", "<html><body>500 Internal Server Error</body></html>");
        return;
    }

    // Write the body content to the file
    fwrite(body, 1, content_length, file);
    fclose(file);

    // Respond with a success message
    char response_body[1024];
    snprintf(response_body, sizeof(response_body), "<html><body>Received PUT data and stored it in %s</body></html>", path);
    send_response(client_socket, "200 OK", "text/html; charset=UTF-8", response_body);
}

const char* extract_body(const char* request, int content_length) {
    const char* body_start = strstr(request, "\r\n\r\n");
    if (!body_start) {
        return NULL;
    }
    body_start += 4; // Skip the "\r\n\r\n"
    // Ensure the body length matches the content length
    if (strlen(body_start) != content_length) {
        return NULL;
    }
    return body_start;
}
