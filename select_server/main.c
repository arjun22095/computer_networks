#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>

#include "proc_parse.h"

#define PORT 2000
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

int DEBUG_MODE = 1;
#define dprint      \
    if (DEBUG_MODE == 1) \
    printf

struct AppInformation {
    int port;
    int socket_fd;
    struct sockaddr_in address;
    bool multithreaded;
};

struct AppInformation *server_global;

int Socket(struct AppInformation *server) {
    server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socket_fd < 0) {
        perror("[ERROR] Could not create server socket");
        return -1; // Return -1 on failure
    }
    dprint("[SUCCESS] Server socket created successfully\n");
    return server->socket_fd;
}

void CreateAddress(struct AppInformation *server, const char *ip, const int port) {
    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(PORT);
    dprint("[SUCCESS] Server address set successfully\n");
}

int SetSocketOption(struct AppInformation *server, int option) {
    int result = setsockopt(server->socket_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (result < 0) {
        perror("Setsockopt failed");
    } else {
        dprint("[SUCCESS] Setsockopt successfully\n");
    }
    return result;
}

int Bind(struct AppInformation *server) {
    int result = bind(server->socket_fd, (struct sockaddr *) &server->address, sizeof(server->address));
    if (result < 0) {
        perror("[ERROR] Socket was not bound");
        return -1; // Return -1 on failure
    }
    dprint("[SUCCESS] Socket was bound successfully!\n");
    return result;
}

int Listen(struct AppInformation *server, const int n) {
    int result = listen(server->socket_fd, n);
    if (result < 0) {
        perror("[ERROR] Socket can't listen!\n");
        return -1; // Return -1 on failure
    }
    dprint("[SUCCESS] Socket is listening successfully!\n");
    return result;
}


void Cleanup(struct AppInformation *server) {
    if (server->socket_fd >= 0) {
        shutdown(server->socket_fd, SHUT_RDWR);
        close(server->socket_fd);
    }
    dprint("[SUCCESS] Socket was cleaned up successfully!\n");
}

void get_proc_details_string(char *message) {
    struct TopTwoProcesses processes = get_max_two_procs_by_cpu_usage();

    snprintf(message, 1024,
             "Top Two Processes:\n"
             "1. PID: %d, Name: %s, User Time: %ld, Kernel Time: %ld, "
             "Child User Time: %ld, Child Kernel Time: %ld, Total Time: %ld\n"
             "2. PID: %d, Name: %s, User Time: %ld, Kernel Time: %ld, "
             "Child User Time: %ld, Child Kernel Time: %ld, Total Time: %ld\n",
             processes.process_1.pid, processes.process_1.name, processes.process_1.utime,
             processes.process_1.stime, processes.process_1.cutime, processes.process_1.cstime,
             processes.process_1.total_time,
             processes.process_2.pid, processes.process_2.name, processes.process_2.utime,
             processes.process_2.stime, processes.process_2.cutime, processes.process_2.cstime,
             processes.process_2.total_time);
}

int Send(char *message, int socketFD) {
    const ssize_t sent_bytes = send(socketFD, message, strlen(message), 0);
    if (sent_bytes <= 0) {
        dprint("[ERROR]: Send failed, sent %zd bytes\n", sent_bytes);
        return -1; // Return -1 on failure
    } else {
        dprint("[INFO]: Send success, sent %zd bytes\n", sent_bytes);
    }

    return (int)sent_bytes;
}

void SelectHandler(struct AppInformation *server) {
    int new_socket, client_sockets[MAX_CLIENTS], max_sd, sd;
    fd_set readfds;
    char buffer[BUFFER_SIZE];

    // Initialize client sockets
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0;
    }

    int addrlen = sizeof(server->address);

    while (true) {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add server socket to set
        FD_SET(server->socket_fd, &readfds);
        max_sd = server->socket_fd;

        // Add client sockets to set
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        // Wait for activity on any of the sockets
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            perror("Select error");
        }

        // If something happened on the server socket, it's an incoming connection
        if (FD_ISSET(server->socket_fd, &readfds)) {
            if ((new_socket = accept(server->socket_fd, (struct sockaddr *) &server->address,
                                     (socklen_t *) &addrlen)) < 0) {
                perror("Accept failed");
                exit(EXIT_FAILURE);
            }

            printf("New connection, socket fd is %d\n", new_socket);

            // Add new socket to the client_sockets array
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);
                    break;
                }
            }
        }

        // Check all client sockets
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];
            if (FD_ISSET(sd, &readfds)) {
                // Check if it was for closing, and also read the incoming message
                int valread = read(sd, buffer, BUFFER_SIZE);
                if (valread == 0) {
                    // Client disconnected
                    getpeername(sd, (struct sockaddr *) &server->address, (socklen_t *) &addrlen);
                    printf("Host disconnected, IP %s, port %d\n",
                           inet_ntoa(server->address.sin_addr), ntohs(server->address.sin_port));

                    close(sd);
                    client_sockets[i] = 0; // Mark as 0 for reuse
                } else if (valread < 0) {
                    // Handle read error
                    perror("Read error");
                    close(sd);
                    client_sockets[i] = 0; // Mark as 0 for reuse
                } else {
                    // Echo back the message to the client
                    printf("Received: %s\n", buffer);

                    char message[1024];
                    get_proc_details_string(message);
                    Send(message, sd);
                    close(sd); // Close socket after sending message
                    client_sockets[i] = 0; // Mark as 0 for reuse
                }
            }
        }
    }
}


int main() {
    struct AppInformation server;
    server.port = 2000;
    server_global = &server;

    // Create server socket
    if (Socket(&server) < 0) {
        Cleanup(&server);
        return 1;
    }

    // Set socket options
    if (SetSocketOption(&server, 1) < 0) {
        Cleanup(&server);
        return 1;
    }

    CreateAddress(&server, "", server.port);

    if (Bind(&server) < 0) {
        Cleanup(&server);
        return 1;
    }

    // Listen for incoming connections
    if (Listen(&server, 20) < 0) {
        Cleanup(&server);
        return 1;
    }

    SelectHandler(&server);
    Cleanup(&server);
    return 0;
}
