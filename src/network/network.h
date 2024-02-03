#ifndef NETWORK_H
#define NETWORK_H

#include "../messages/queue.h"
#include "../messages/message.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*
    Datastructures
*/

typedef struct NetworkListNode {
    int id;
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

/*
    Adding / Removing nodes
*/

int addNode(Network *network, int id);


bool containsNode(Network *network, int id);


NetworkListNode *getNetworkListNode(Network *network, int id);

NetworkListNode *getPrevNetworkListNode(Network *network, int id);

int size(Network *network);


void removeNode(Network *network, int id);

/*
    Setup Client / Server
*/

int setupClient(uint16_t port);

int setupServer(uint16_t port);

int acceptConnection(int server_fd);

/*
    Sending / receiving messages
*/

void sendMessage(int receiver_fd, Message *msg);

void broadcastMessage(Network *network, Message *msg);

Message receiveMessage(int node_fd);

#endif