#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "response.h"

void send_response(int client_socket, const char *response) {
    write(client_socket, response, strlen(response));
}

void handle_get(int client_socket) {
    const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nGET response";
    send_response(client_socket, response);
}

void handle_post(int client_socket) {
    const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nPOST response";
    send_response(client_socket, response);
}

void handle_put(int client_socket) {
    const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nPUT response";
    send_response(client_socket, response);
}

void handle_delete(int client_socket) {
    const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nDELETE response";
    send_response(client_socket, response);
}
