#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>

typedef struct NetworkNode NetworkNode;

typedef struct Network Network;

/*
    Initialization
*/

Network *initNetwork();

void destroyNetwork(Network *network);

/*
    Adding / Removing nodes
*/

int addNode(Network *network, int index, struct sockaddr *addr);


bool containsNode(Network *network, int index);


NetworkNode *getNetworkNode(Network *network, int index);

NetworkNode *getPrevNetworkNode(Network *network, int index);

struct sockaddr *getNode(Network *network, int index);


void removeNode(Network *network, int index);

#endif