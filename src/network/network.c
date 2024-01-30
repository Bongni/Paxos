#include "network.h"

#include "queue.h"

#include <stdlib.h>
#include <stdbool.h>

/*
    Initialization
*/

Network *initNetwork() {
    Network *network = (Network *) malloc(sizeof(Network));

    if (network == NULL) return NULL;


    network->nodes = NULL;

    return network;
}

void destroyNetwork(Network *network) {
    if (network == NULL) return;

    // Free all nodes in node list
    NetworkListNode *curr = network->nodes;
    NetworkListNode *next;

    while(curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }

    // Free network
    free(network);
}

NetworkNode *initNode(int id) {
    NetworkNode *node = (NetworkNode *) malloc(sizeof(NetworkNode));

    if (node == NULL) return NULL;


    node->id = id;
    node->receiveQueue = initQueue();

    return node;
}

void destroyNode(NetworkNode *node) {
    if (node == NULL) return;

    destroyQueue(node->receiveQueue);

    free(node);
}

/*
    Adding / Removing nodes
*/

int addNode(Network *network, NetworkNode *node) {
    NetworkListNode *new = (NetworkListNode *) malloc(sizeof(NetworkListNode));

    if (new == NULL) return -1;
    if (containsNode(network, node->id)) return -1;

    new->id = node->id;
    new->node = node;
    new->next = network->nodes;
    network->nodes = new;

    return 0;
}

bool containsNode(Network *network, int id) {
    NetworkListNode *curr = network->nodes;

    while (curr != NULL) {
        if (curr->id == id) return true;
        curr = curr->next;
    }

    return false;
}

NetworkListNode *getNetworkListNode(Network *network, int id) {
    NetworkListNode *curr = network->nodes;

    while (curr != NULL) {
        if (curr->id == id) return curr;
        curr = curr->next;
    }

    return NULL;
}

NetworkListNode *getPrevNetworkListNode(Network *network, int id) {
    NetworkListNode *curr = network->nodes;
    NetworkListNode *prev = NULL;

    while (curr != NULL) {
        if (curr->id == id) return prev;
        prev = curr;
        curr = curr->next;
    }

    return NULL;
}

NetworkNode *getNode(Network *network, int id) {
    NetworkListNode *listNode = getNetworkListNode(network, id);

    if (listNode == NULL) return NULL;

    return listNode->node;
}

int size(Network *network) {
    NetworkListNode *curr = network->nodes;
    int size = 0;

    while (curr != NULL) {
        size++;
        curr = curr->next;
    }

    return size;
}

void removeNode(Network *network, int id) {
    NetworkListNode *listNode = getNetworkListNode(network, id);

    if (listNode == NULL) return;

    if (network->nodes == listNode) {
        network->nodes = listNode->next;
    } else {
        NetworkListNode *prev = getPrevNetworkListNode(network, id);
        prev->next = listNode->next;
    }

    free(listNode);
    listNode = NULL;
}

/*
    Sending / receiving messages
*/

void sendMessage(Network *network, int receiver, Message msg){
    NetworkNode *node = getNode(network, receiver);

    push(node->receiveQueue, msg);
}

void broadcastMessage(Network *network, Message msg){
    NetworkListNode *node = network->nodes;

    while(node != NULL) {
        sendMessage(network, node->id, msg);

        node = node->next;
    }
}

bool canReceiveMessage(Network *network, int id){
    NetworkNode *node = getNode(network, id);

    return length(node->receiveQueue) > 0;
}

Message receiveMessage(Network *network, int id){
    NetworkNode *node = getNode(network, id);

    return pop(node->receiveQueue);
}
