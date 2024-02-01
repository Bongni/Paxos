#include "messages/message.h"

#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define N_CLIENTS 4
#define N_SERVERS 5

int setupClient(uint16_t port) {
    int client_fd;
    struct sockaddr_in server_addr;
    
    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        printf("\nInvalid address\n");
        return -1;
    }

    int status;

    if((status = connect(client_fd, (struct sockaddr*) &server_addr, sizeof(server_addr))) < 0) {
        printf("\nConnection failed\n");
        return -1;
    }

    return client_fd;
}

void sendMessage(int client_fd, Message *message) {
    unsigned char serialized_message[sizeof(*message)], *ptr;

    ptr = serialize_message(serialized_message, message);

    send(client_fd, serialized_message, ptr - serialized_message, 0);
    printf("Message sent\n");
}

void *runClient(void *) {
    int client_fd = setupClient(PORT);

    Message message = {1, 2, 3, 4};
    sendMessage(client_fd, &message);    

    close(client_fd);
}

int main(int argc, char *argv) {

    int client_fd[N_SERVERS];
    pthread_t clientThread[N_SERVERS];

    for (int i = 0; i < N_SERVERS; i++) {
        client_fd[i] = setupClient(PORT);

        pthread_create(&clientThread[i], NULL, (void * (*)(void *)) runClient, &client_fd[i]);
    }

    for (int i = 0; i < N_SERVERS; i++) {
        pthread_join(clientThread[i], NULL);
    }
    
    return 0;
}