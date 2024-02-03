#include "messages/message.h"

#include "network/network.h"
#include "paxos/node.h"
#include "paxos/client.h"

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

void *runClient(int client_fd[]) {
    Node client = initClient(client_fd[N_SERVERS], 42 + client_fd[N_SERVERS]);

    for(int i = 0; i < N_SERVERS; i++) {
        addServer(&client, client_fd[i]);
    }

    paxosClient(&client);
}

int main(int argc, char *argv) {

    int client_fd[N_CLIENTS][N_SERVERS + 1];
    pthread_t clientThread[N_CLIENTS];

    for (int i = 0; i < N_CLIENTS; i++) {
        client_fd[i][N_SERVERS] = i;
        for (int j = 0; j < N_SERVERS; j++) {
            client_fd[i][j] = setupClient(PORT + i * N_SERVERS + j);
        }

        pthread_create(&clientThread[i], NULL, (void * (*)(void *)) runClient, &client_fd[i]);
    }

    for (int i = 0; i < N_CLIENTS; i++) {
        pthread_join(clientThread[i], NULL);
    }
    
    return 0;
}