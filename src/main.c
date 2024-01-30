
#include "network/network.h"
#include "paxos/client.h"
#include "paxos/server.h"
#include "paxos/node.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

/*
    Debug flag
*/

bool DEBUG_FLAG = false;

void set_flags(char *argv[], int argc) {
    if ( strcmp(argv[1], "-d") == 0 ) {
        DEBUG_FLAG = true;
        printf("Debugging enabled.\n");
    }
}

/*
    Setup environment
*/

#define N 4
#define M 5

Node clients[N];
pthread_t clientThreads[N];

Node servers[M];
pthread_t serverThreads[M];

void setup() {
    for(int i = 0; i < N; i++) {
        clients[i] = initClient(i, 42 + i);
    }

    for(int j = 0; j < M; j++) {
        servers[j] = initServer(N + j, 0);
    }

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            // Add client to server network
            addClient(&servers[j], &clients[i]);

            // Add server to client network
            addServer(&clients[i], &servers[j]);
        }
    }
}

void teardown() {
    for(int i = 0; i < N; i++) {
        destroyClient(&clients[i]);
    }

    for(int j = 0; j < M; j++) {
        destroyServer(&servers[j]);
    }
}

/*
    Main
*/
 
int main (int argc, char *argv[]) {

    printf("Executing Paxos ... \n");

    if (argc >= 2) {
        set_flags(argv, argc);
    }

    setup();

    // Create server threads

    for(int j = 0; j < M; j++) {
        pthread_create(&serverThreads[j], NULL, (void * (*)(void *)) paxosServer, &servers[j]);
    }

    // Create client thread

    for(int i = 0; i < N; i++) {
        pthread_create(&clientThreads[i], NULL, (void * (*)(void *)) paxosClient, &clients[i]);
    }
    // Wait for the threads to complete

    int *value;

    for(int i = 0; i < N; i++) {
        pthread_join(clientThreads[i], (void *) &value);
        if (i == 0) printf("\nPaxos complete\n\n");
        printf("Client %d value: %d\n", i, *value);
    }

    for(int j = 0; j < M; j++) {
        pthread_join(serverThreads[j], (void *) &value);
        if (j == 0) printf("\nServers\n\n");
        printf("Server %d value: %d\n", N + j, *value);
    }

    teardown();

    return 0;
}
