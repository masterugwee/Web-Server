#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

void handle_request(int client_socket);
void parse_request_headers(const char* request, char* method, char* path, char* host, char* content_type, int* content_length);
void serve_file(int client_socket, const char* path);
void handle_get(int client_socket, const char* path);
void handle_post(int client_socket, const char* path, const char* body);
void handle_put(int client_socket, const char* path, const char* body, int content_length);
void send_response(int client_socket, const char* status, const char* content_type, const char* body);
void send_file_response(int client_socket, const char* status, const char* content_type, FILE* file);
int is_valid_content_type(const char* content_type);
const char* extract_body(const char* request, int content_length);

#endif // REQUEST_HANDLER_H
