#include "./network.h"

#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>

/*
    Datastructures
*/

typedef struct Network {
    NetworkNode *nodes;
} Network;

typedef struct NetworkNode {
    int index;
    struct sockaddr *addr;
    NetworkNode *next;
} NetworkNode;

/*
    Initialization
*/

Network* initNetwork() {
    Network *network = (Network *) malloc(sizeof(Network));

    if (network == NULL) return NULL;


    network->nodes = NULL;

    return network;
}

void destroyNetwork(Network *network) {
    if (network == NULL) return;

    free(network);
    network = NULL;
}

/*
    Adding / Removing nodes
*/

int addNode(Network *network, int index, struct sockaddr *addr) {
    NetworkNode *new = (NetworkNode *) malloc(sizeof(NetworkNode));

    if (new == NULL) return -1;
    if (containsNode(network, index)) return -1;

    new->index = index;
    new->addr = addr;
    new->next = network->nodes;
    network->nodes = new;

    return 0;
}

bool containsNode(Network *network, int index) {
    NetworkNode *curr = network->nodes;

    while (curr != NULL) {
        if (curr->index == index) return true;
        curr = curr->next;
    }

    return false;
}

NetworkNode *getNetworkNode(Network *network, int index) {
    NetworkNode *curr = network->nodes;

    while (curr != NULL) {
        if (curr->index == index) return curr;
        curr = curr->next;
    }

    return NULL;
}

NetworkNode *getPrevNetworkNode(Network *network, int index) {
    NetworkNode *curr = network->nodes;
    NetworkNode *prev = NULL;

    while (curr != NULL) {
        if (curr->index == index) return prev;
        prev = curr;
        curr = curr->next;
    }

    return NULL;
}

struct sockaddr *getNode(Network *network, int index) {
    NetworkNode *node = getNetworkNode(network, index);

    if (node == NULL) return NULL;

    return node->addr;
}

void removeNode(Network *network, int index) {
    NetworkNode *node = getNetworkNode(network, index);

    if (node == NULL) return;

    if (network->nodes == node) {
        network->nodes = node->next;
    } else {
        NetworkNode *prev = getPrevNetworkNode(network, index);
        prev->next = node->next;
    }

    free(node);
    node = NULL;
}

