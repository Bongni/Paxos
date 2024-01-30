#include "server.h"

#include "../network/network.h"
#include "node.h"

/*
    Initialization
*/

Node initServer(int id, int value){
    return (Node) {id, value,  initNode(id), initNetwork()};
}

void destroyServer(Node *server){
    destroyNode(server->node);
    destroyNetwork(server->network);
}

/*
    Add / Remove nodes
*/

void addClient(Node *server, Node *client){
    addNode(server->network, client->node);
}

void removeClient(Node *server, Node *client){
    removeNode(server->network, client->node->id);
}

/*
    Paxos
*/

int paxosServer(Node *server){
    return -1;
}
