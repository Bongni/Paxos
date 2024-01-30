#include "server.h"

#include "../network/network.h"
#include "node.h"

#include <pthread.h>
#include <stdio.h>

/*
    Initialization
*/

Node initServer(int id, int value){
    Network *network = initNetwork();
    NetworkNode *node = initNode(id);

    addNode(network, node);

    return (Node) {id, value,  node, network};
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

void *paxosServer(Node *server){

    while(true) {

        int maxTicket = 0;
        int valueTicket = 0;
        server->value = 0;

        while(true) {
            if (canReceiveMessage(server->network, server->id)) {
                Message msg = receiveMessage(server->network, server->id);
    
                if (msg.command == RequestTicket) {
                    if(msg.ticket > maxTicket) {
                        maxTicket = msg.ticket;

                        printf("Server %d received request(%d) from client %d\n", server->id, msg.ticket, msg.sender);

                        Message ok = {server->id, Ok, valueTicket, server->value};
                        sendMessage(server->network, msg.sender, ok);
                    }
                } else if (msg.command == Propose) {
                    if(msg.ticket == maxTicket) {
                        server->value = msg.value;
                        valueTicket = msg.ticket;

                        printf("Server %d received propose(%d, %d) from client %d\n", server->id, msg.ticket, msg.value, msg.sender);

                        Message success = {server->id, Success, valueTicket, server->value};
                        sendMessage(server->network, msg.sender, success);
                    }
                } else if (msg.command == Execute) {
                    server->value = msg.value;
                    printf("Server %d received execute(%d) from client %d\n", server->id, msg.value, msg.sender);
                    pthread_exit(&server->value);
                    return NULL;
                }
            } else {
                sched_yield();
            }
        }
    }
}
