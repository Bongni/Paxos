
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

Node client;
pthread_t clientThread;

Node servers[5];
pthread_t serverThreads[5];

void setup() {
    client = initClient(0, 42);

    for(int i = 0; i < 5; i++) {
        servers[i] = initServer(i + 1, 0);

        // Add client to server network
        addClient(&servers[i], &client);

        // Add server to client network
        addServer(&client, &servers[i]);
    }
}

void teardown() {
    destroyClient(&client);

    for(int i = 0; i < 5; i++) {
        destroyServer(&servers[i]);
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

    for(int i = 0; i < 5; i++) {
        pthread_create(&serverThreads[i], NULL, paxosServer, &servers[i]);
    }

    // Create client thread

    pthread_create(&clientThread, NULL, paxosClient, &client);

    // Wait for the threads to complete

    int value;
    int values[5];

    pthread_join(&clientThread, &value);
    printf("Client value: %d\n", value);

    for(int i = 0; i < 5; i++) {
        pthread_join(&serverThreads[i], &values[i]);
        printf("Server %d value: %d\n", i, values[i]);
    }

    teardown();

    return 0;
}
