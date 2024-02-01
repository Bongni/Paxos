#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char *argv) {
    
    int client_fd;
    struct sockaddr_in server_addr;
    
    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        printf("\nInvalid address\n");
        return -1;
    }

    int status;

    if((status = connect(client_fd, (struct sockaddr*) &server_addr, sizeof(server_addr))) < 0) {
        printf("\nConnection failed\n");
        return -1;
    }

    char *message = "Test!";
    send(client_fd, message, strlen(message), 0);
    printf("Message sent\n");

    close(client_fd);
    return 0;
}