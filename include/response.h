#ifndef RESPONSE_H
#define RESPONSE_H

void send_response(int client_socket, const char *response);
void handle_get(int client_socket);
void handle_post(int client_socket);
void handle_put(int client_socket);
void handle_delete(int client_socket);

#endif // RESPONSE_H
