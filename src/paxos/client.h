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

void addServer(Node *client, Node *server);

void removeServer(Node *client, Node *server);

/*
    Paxos
*/

int paxosClient(Node *client);

#endif