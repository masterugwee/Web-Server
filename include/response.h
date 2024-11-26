#ifndef RESPONSE_H
#define RESPONSE_H

void send_response(int client_socket, const char* status, const char* content_type, const char* body);
void send_file_response(int client_socket, const char* status, const char* content_type, FILE* file);

#endif // RESPONSE_H
