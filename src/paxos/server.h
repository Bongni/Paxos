#ifndef PAXOS_SERVER_H
#define PAXOS_SERVER_H 

#include "../network/network.h"
#include "node.h"

/*
    Initialization
*/

Node initServer(int id, int value);

void destroyServer(Node *server);

/*
    Add / Remove nodes
*/

void addClient(Node *server, Node *client);

void removeClient(Node *server, Node *client);

/*
    Paxos
*/

void *paxosServer(Node *server);

#endif