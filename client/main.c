#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

struct AppInformation {
    int port;
    int socket_fd;
    struct sockaddr_in *address;
};

struct ThreadArguments {
    struct AppInformation *client;
};


struct sockaddr_in *CreateAddress(struct AppInformation *client, const char *ip, const int port) {
    // Creating a IP v4 Address
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(port);

    if (strlen(ip) == 0) {
        address->sin_addr.s_addr = INADDR_ANY;
    } else {
        inet_pton(AF_INET, ip, &address->sin_addr.s_addr);
    }

    client->address = address;
    return address;
}


int Send(char *message, int socketFD) {
    // Sending message to client socket_fd
    const ssize_t sent_bytes = send(socketFD, message, strlen(message), 0);
    if (sent_bytes <= 0) {
        printf("[ERROR]: Send failed, sent %zd bytes\n", sent_bytes);
    } else {
        printf("[INFO]: Send success, sent %zd bytes\n", sent_bytes);
    }

    return sent_bytes;
}


void send_message_to_server(int socketFD) {
    size_t lineSize = 0;
    char *line = "Request message from client\n";
    char buffer[1024] = {0};
    sprintf(buffer, "%s", line);
    Send(buffer, socketFD);
}

int Receive(int socketFD, char *buffer, unsigned int buffer_size, int flags) {
    const ssize_t received_size = recv(socketFD, buffer, buffer_size, flags);
    if (received_size < 0) {
        printf("[ERROR] Receive failed, received %zd bytes\n", received_size);
    } else {
        printf("[INFO] Received %zd bytes\n", received_size);
        buffer[received_size] = 0;
    }

    return received_size;
}

void listen_to_server_and_print_response(int socketFD) {
    char buffer[1024] = {0};
    Receive(socketFD, buffer, 1024, 0);
    printf("[INFO] Response was %s\n ", buffer);
    close(socketFD);
}

void startListeningAndPrintMessagesOnNewThread(int socketFD) {
    pthread_t id;
    pthread_create(&id,NULL, listen_to_server_and_print_response, &socketFD);
    pthread_detach(id);
}

int Socket(struct AppInformation *server) {
    server->socket_fd = socket(AF_INET,SOCK_STREAM, 0); // Creating TCP IPv4 socket
    if (server->socket_fd >= 0) {
        printf("[SUCCESS] Server socket created successfully\n");
    } else {
        printf("[ERROR] Could not create server socket\n");
    }
    return server->socket_fd;
}

void Cleanup(struct AppInformation *client) {
    // shutdown(client->socket_fd,SHUT_RDWR);
    close(client->socket_fd);
    free(client->address);
    printf("[SUCCESS] Socket was cleaned up successfully!\n");
}

int Connect(struct AppInformation *client) {
    int result = connect(client->socket_fd, client->address, sizeof(*client->address));
    if (result < 0) {
        printf("[ERROR] Could not connect to server socket\n");
    } else {
        printf("[SUCCESS] Server socket connected\n");
    }
    return result;
}

void *clientThread(void *arg) {
    struct ThreadArguments *threadArgs = (struct ThreadArguments *) arg;
    struct AppInformation *client = threadArgs->client;

    Socket(client);
    CreateAddress(client, "127.0.0.1", 2000);

    if (Connect(client) == 0) {
        send_message_to_server(client->socket_fd);
        // startListeningAndPrintMessagesOnNewThread(client->socket_fd);
        listen_to_server_and_print_response(client->socket_fd);
    }

    Cleanup(client);
    // pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <number_of_clients>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    pthread_t threads[n];
    struct ThreadArguments args[n];
    struct AppInformation clients[n];

    // Creating n client threads
    for (int i = 0; i < n; i++) {
        args[i].client = &clients[i];
        pthread_create(&threads[i], NULL, clientThread, &args[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
