#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

void handle_request(int client_socket);
void parse_request(char* request, char* method, char* path);
void send_response(int client_socket, const char* status, const char* content_type, const char* body);
void serve_file(int client_socket, const char* path);
void handle_post(int client_socket, const char* path, const char* body);
void handle_put(int client_socket, const char* path, const char* body);
const char* extract_body(const char* request);

#endif // REQUEST_HANDLER_H
