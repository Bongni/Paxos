#include "messages/message.h"

#include "network/network.h"
#include "paxos/node.h"
#include "paxos/server.h"

#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#define PORT 8080
#define N_CLIENTS 4
#define N_SERVERS 5

void * runServer(int *server_fd) {
    Node server = initServer(server_fd[N_CLIENTS], 0);

    for(int i = 0; i < N_CLIENTS; i++) {
        int client_fd = acceptConnection(server_fd[i]);
        addClient(&server, client_fd);
    }

    paxosServer(&server);
}

int main(int argc, char *argv) {

    int server_fd[N_SERVERS][N_CLIENTS + 1];
    pthread_t serverThread[N_SERVERS];

    for (int i = 0; i < N_SERVERS; i++) {
        server_fd[i][N_CLIENTS] = i;
        for (int j = 0; j < N_CLIENTS; j++) {
            server_fd[i][j] = setupServer(PORT + j * N_SERVERS + i);
        }

        pthread_create(&serverThread[i], NULL, (void * (*)(void *)) runServer, &server_fd[i]);
    }

    for (int i = 0; i < N_SERVERS; i++) {
        pthread_join(serverThread[i], NULL);
    }
    
    return 0;
}