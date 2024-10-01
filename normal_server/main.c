#include "proc_parse.h"

int DEBUG_MODE = 1;
#define dprint      \
    if (DEBUG_MODE == 1) \
    printf

struct ClientInformation {
    int socket_fd;
    struct sockaddr_in address;
};

struct AppInformation {
    int port;
    int socket_fd;
    struct sockaddr_in *address;
    bool multithreaded;
};

// Global server reference for cleanup in signal handler
struct AppInformation *server_global;

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

int Receive(int socketFD, char *buffer, unsigned int buffer_size, int flags) {
    const ssize_t received_size = recv(socketFD, buffer, buffer_size, flags);
    if (received_size < 0) {
        perror("[ERROR] Receive failed");
        return -1; // Return -1 on failure
    } else {
        dprint("[INFO] Received %zd bytes\n", received_size);
        buffer[received_size] = 0; // Null-terminate the string
    }

    return (int)received_size;
}


void *pthread_function(void *arg) {
    int client_socket_fd = *(int *)arg;
    free(arg);  // Free dynamically allocated memory
    char buffer[1024] = {0};

    if (Receive(client_socket_fd, buffer, sizeof(buffer), 0) <= 0) {
        close(client_socket_fd);
        return NULL;
    }

    dprint("[Server Received] : %s\n", buffer);

    char message[1024] = {0};
    struct TopTwoProcesses processes = get_max_two_procs_by_cpu_usage();

    snprintf(message, sizeof(message),
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

    Send(message, client_socket_fd);
    close(client_socket_fd); // Ensure the socket is closed after sending
    return NULL;
}


void AcceptHandler(struct ClientInformation *client, bool multithreaded) {
    pthread_t id;
    int *client_socket_fd = malloc(sizeof(int));
    if (!client_socket_fd) {
        perror("[ERROR] Memory allocation failed for client socket fd");
        close(client->socket_fd);
        free(client);
        return;
    }
    *client_socket_fd = client->socket_fd;

    // Single Threaded
    if (!multithreaded) {
        pthread_function(client_socket_fd);
        free(client);
        return;
    }

    // Multithreaded
    if (pthread_create(&id, NULL, pthread_function, client_socket_fd) != 0) {
        perror("[ERROR] Failed to create a thread");
        free(client_socket_fd);
        close(client->socket_fd); // Close the socket on thread creation failure
        free(client); // Free the client structure
    } else {
        dprint("[INFO] Created a thread successfully!\n");
        pthread_detach(id); // Automatically reclaim resources when the thread terminates
    }
}

struct ClientInformation *accept_client_request(struct AppInformation *server) {
    struct ClientInformation *client = malloc(sizeof(struct ClientInformation));
    if (!client) {
        perror("[ERROR] Memory allocation failed for client information");
        exit(EXIT_FAILURE);
    }

    int client_address_size = sizeof(struct sockaddr_in);
    client->socket_fd = accept(server->socket_fd, (struct sockaddr *)&client->address, &client_address_size);

    if (client->socket_fd < 0) {
        perror("[ERROR] Accept failed");
        free(client); // Free the client structure on failure
        return NULL; // Return NULL to indicate failure
    }

    return client;
}

void Accept(struct AppInformation *server) {
    while (true) {
        struct ClientInformation *client = accept_client_request(server);
        if (client == NULL) {
            continue; // Continue to the next iteration if accepting a client failed
        }

        AcceptHandler(client, server->multithreaded);
        // Client is freed by AcceptHandler
    }
}

struct sockaddr_in *CreateAddress(struct AppInformation *server, const char *ip, const int port) {
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    if (!address) {
        perror("[ERROR] Memory allocation failed for socket address");
        exit(EXIT_FAILURE);
    }

    address->sin_family = AF_INET;
    address->sin_port = htons(port);

    if (strlen(ip) == 0) {
        address->sin_addr.s_addr = INADDR_ANY;
    } else {
        if (inet_pton(AF_INET, ip, &address->sin_addr.s_addr) <= 0) {
            perror("[ERROR] Invalid IP address");
            free(address);
            exit(EXIT_FAILURE);
        }
    }

    server->address = address;
    return address;
}

int Socket(struct AppInformation *server) {
    server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socket_fd < 0) {
        perror("[ERROR] Could not create server socket");
        return -1; // Return -1 on failure
    }
    dprint("[SUCCESS] Server socket created successfully\n");
    return server->socket_fd;
}

void SetSocketOpt(struct AppInformation *server) {
    int option = 1;
    server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server->socket_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
}

int Bind(struct AppInformation *server) {
    int result = bind(server->socket_fd, (struct sockaddr *)server->address, sizeof(*server->address));
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
        perror("[ERROR] Socket was not listened successfully!");
        return -1; // Return -1 on failure
    }
    dprint("[SUCCESS] Socket was listened successfully!\n");
    return result;
}

void Cleanup(struct AppInformation *server) {
    if (server->socket_fd >= 0) {
        if (shutdown(server->socket_fd, SHUT_RDWR) == 0) {
            dprint("[CLEANUP] Shutdown successful!\n");
        } else {
            dprint("[CLEANUP] Shutdown failed!\n");
        }
        close(server->socket_fd);
    }
    if (server->address) {
        free(server->address);
    }
    dprint("[SUCCESS] Socket was cleaned up successfully!\n");
}

void sigint_handler(int sig_num) {
    dprint("\n[INFO] SIGINT received, cleaning up resources...\n");
    if (server_global) {
        Cleanup(server_global);
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <single-character>\n", argv[0]);
        return 1;
    }

    char *arg = argv[1];
    if (!(arg[1] == '\0' && (arg[0] == 'm' || arg[0] == 's'))) {
        printf("[ERROR] Invalid arguments passed\n");
        return 1;
    }

    struct AppInformation server;
    server_global = &server;  // Assign the global reference for the signal handler

    // Set up SIGINT handler
    signal(SIGINT, sigint_handler);

    server.multithreaded = (arg[0] == 'm');

    if (Socket(&server) < 0) {
        Cleanup(&server);
        return 1;
    }

    CreateAddress(&server, "", 2000);
    if (Bind(&server) < 0) {
        dprint("[INFO] Trying SO_REUSEADDR\n");
        SetSocketOpt(&server);
        Bind(&server);
        // Cleanup(&server);
        // return 1;
    }

    if (Listen(&server, 20) < 0) {
        Cleanup(&server);
        return 1;
    }

    Accept(&server);
    Cleanup(&server);

    return 0;
}
