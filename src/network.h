#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>

/*
    Datastructures
*/

typedef struct Queue Queue;

typedef struct QueueNode QueueNode;

typedef struct Network Network;

typedef struct NetworkNode NetworkNode;

typedef struct NetworkListNode NetworkListNode;

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

#endif