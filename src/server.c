#include <stdio.h> // Common functions, print, scan, open, read
#include <stdlib.h> // exit, memory
#include <string.h> // string handling
#include <unistd.h> // fork()
#include <arpa/inet.h>  // internet operations, ip addresses
#include <sys/wait.h>  // Process term and wait
#include "server.h" // User-defined header-files
#include "request_handler.h" 

#define PORT 1337
#define BACKLOG 10

void start_server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
// Sockets are endpoints for send and receiving data over network

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
// The condition checks if socket creation failed, IPv4, TCP, 0 - defaults to TCP. 
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;   // Specifies IPv4 addresses
    address.sin_addr.s_addr = INADDR_ANY; // Any incoming interfaces
    address.sin_port = htons(PORT);     // Port

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        // binds sockets to IP addresses
        perror("bind failed"); 
        close(server_fd);
        // releases the resources associated with socket
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, BACKLOG) < 0) {
        // Listen to incoming connections 
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d\n", PORT);

    while (1) {
        //Infinite loop for accepting connections
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        // Waits for new connection, if connected creates a new socket
        if (new_socket < 0) {
            perror("accept");
            continue;
        }

        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            close(server_fd); // No need to accept any further connection, closes the fd
            handle_request(new_socket); // Sends it to handle_request
            close(new_socket); // Closes the socket after handling the request
            exit(0); // Finally terminates the child process
        } else if (pid < 0) {
            perror("fork");
        } else {
            // Parent process
            close(new_socket);
            waitpid(-1, NULL, WNOHANG);  // Clean up zombie processes
        }
    }

    close(server_fd);
}
