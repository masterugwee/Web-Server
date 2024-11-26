#include <stdio.h>
#include <string.h>
#include "request_handler.h"
#include "response.h"

void handle_request(int client_socket) {
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);

    if (strncmp(buffer, "GET", 3) == 0) {
        handle_get(client_socket);
    } else if (strncmp(buffer, "POST", 4) == 0) {
        handle_post(client_socket);
    } else if (strncmp(buffer, "PUT", 3) == 0) {
        handle_put(client_socket);
    } else if (strncmp(buffer, "DELETE", 6) == 0) {
        handle_delete(client_socket);
    } else {
        send_response(client_socket, "HTTP/1.1 400 Bad Request\r\n\r\n");
    }
}
