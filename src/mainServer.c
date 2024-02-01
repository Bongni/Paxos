#include "messages/message.h"

#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#define PORT 8080
#define N 5

int setupServer(uint16_t port) {
    int server_fd;
    struct sockaddr_in address;
    
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

Message receiveMessage(int server_fd) {
    int client_fd;
    struct sockaddr_in address;
    socklen_t client_addrlen;

    Message message;
    unsigned char serialized_message[1024];

    if((client_fd = accept(server_fd, (struct sockaddr*) &address, &client_addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    read(client_fd, serialized_message, 1024 - 1);

    deserialize_message(serialized_message, &message);

    close(client_fd);

    return message;
}

void * runServer(int *server_fd) {
    Message message;

    while (true) {
        message = receiveMessage(*server_fd);

        printf("Server %d \t Message {%d, %d, %d, %d}\n", 
            *server_fd,
            message.sender, 
            message.command, 
            message.ticket, 
            message.value);
    }
}

int main(int argc, char *argv) {

    int server_fd[N];
    pthread_t serverThread[N];

    for (int i = 0; i < N; i++) {
        server_fd[i] = setupServer(PORT + i);

        pthread_create(&serverThread[i], NULL, (void * (*)(void *)) runServer, &server_fd[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(serverThread[i], NULL);
    }
    
    return 0;
}