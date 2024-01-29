#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"
#include "message.h"

/*
    Datastructures
*/

typedef struct {
    int id;
    Queue *receiveQueue;
} NetworkNode;

typedef struct NetworkListNode {
    int id;
    NetworkNode *node;
    struct NetworkListNode *next;
} NetworkListNode;

typedef struct {
    NetworkListNode *nodes;
} Network;

/*
    Initialization
*/

Network *initNetwork();

void destroyNetwork(Network *network);

NetworkNode *initNode(int id);

/*
    Adding / Removing nodes
*/

int addNode(Network *network, NetworkNode *node);


bool containsNode(Network *network, int id);


NetworkListNode *getNetworkListNode(Network *network, int id);

NetworkListNode *getPrevNetworkListNode(Network *network, int id);

NetworkNode *getNode(Network *network, int id);


void removeNode(Network *network, int id);

/*
    Sending / receiving messages
*/

void sendMessage(Network *network, int receiver, Message msg);

Message receiveMessage(Network *network, int id);

#endif