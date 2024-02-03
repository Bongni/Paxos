#ifndef PAXOS_CLIENT_H
#define PAXOS_CLIENT_H 

#include "../network/network.h"
#include "node.h"

/*
    Initialization
*/

Node initClient(int id, int value);

void destroyClient(Node *client);

/*
    Add / Remove nodes
*/

void addServer(Node *client, int server);

void removeServer(Node *client, int server);

/*
    Paxos
*/

void *paxosClient(Node *client);

#endif