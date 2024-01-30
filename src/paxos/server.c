#include "server.h"

#include "../network/network.h"
#include "node.h"

#include <pthread.h>

/*
    Initialization
*/

Node initServer(int id, int value){
    Network *network = initNetwork();
    Node *node = initNode(id);

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

                        Message ok = {server->id, Ok, valueTicket, server->value};
                        sendMessage(server->network, msg.sender, ok);
                    }
                } else if (msg.command == Propose) {
                    if(msg.ticket == maxTicket) {
                        server->value = msg.value;
                        valueTicket = msg.ticket;

                        Message success = {server->id, Ok, valueTicket, server->value};
                        sendMessage(server->network, msg.sender, success);
                    }
                } else if (msg.command == Execute) {
                    if(msg.value == server->value) {
                        pthread_exit(&server->value);
                        return NULL;
                    }
                }
            } else {
                sched_yield();
            }
        }
    }
}
