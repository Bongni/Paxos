#include "messages/message.h"

#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define PORT 8080

int main(int argc, char *argv) {
    
    int server_fd;
    struct sockaddr_in address;
    
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    /*
        Accept incoming connections
    */

    int client_fd;
    socklen_t client_addrlen;

    Message message;
    unsigned char serialized_message[1024];

    while (true) {
        if((client_fd = accept(server_fd, (struct sockaddr*) &address, &client_addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        read(client_fd, serialized_message, 1024 - 1);

        deserialize_message(serialized_message, &message);

        printf("Message {%d, %d, %d, %d}\n", 
            message.sender, 
            message.command, 
            message.ticket, 
            message.value);
    }
    
    return 0;
}