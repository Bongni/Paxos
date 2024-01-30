#include "client.h"

#include "../network/network.h"
#include "server.h"

#include <time.h>
#include <stdlib.h>
#include <pthread.h>

/*
    Initialization
*/

Node initClient(int id, int value) {
    return (Node) {id, value,  initNode(id), initNetwork()};
}

void destroyClient(Node *client) {
    destroyNode(client->node);
    destroyNetwork(client->network);
}

/*
    Add / Remove nodes
*/

void addServer(Node *client, Node *server){
    addNode(client->network, server->node);
}

void removeServer(Node *client, Node *server){
    removeNode(client->network, server->node->id);
}

/*
    Paxos
*/

void requestAll(Node *client, int ticket) {
    Message request = {client->id, RequestTicket, ticket, 0};

    broadcastMessage(client->network, request);
}

void proposeMajority(Node *client, Network *majority, int ticket, int value) {
    Message propose = {client->id, Propose, ticket, value};

    broadcastMessage(majority, propose);
}

void executeAll(Node *client, int value) {
    Message execute = {client->id, Execute, 0, value};

    broadcastMessage(client->network, execute);
}

int paxosClient(Node *client) {

    srand(time(NULL));
    int ticket = rand();
    int value = client->value;

    Queue *queue = client->node->receiveQueue;

    while(true) {

    /*
        Phase 1
    */

        ticket++;
        requestAll(client, ticket);

    /*
        Phase 2
    */

        int n = size(client->network);
        int numberOk = 0;

        int maxTicket = 0;
        Network *majority = initNetwork();

        while(numberOk <= n / 2) {
            pthread_yield(NULL);

            while(length(queue) > 0) {
                Message msg = pop(queue);

                if (msg.command == Ok) {
                    numberOk++;
                    // Add all nodes that sent ok to majority
                    addNode(majority, getNode(client->network, msg.sender));

                    if(msg.ticket > maxTicket) {
                        maxTicket = msg.ticket;
                        value = msg.value;
                    }
                }
            }
        }

        proposeMajority(client, majority, ticket, value);

    /*
        Phase 3
    */

        int numberSuccess = 0;

        while(numberSuccess <= n / 2) {
            pthread_yield(NULL);

            while(length(queue) > 0) {
                Message msg = pop(queue);

                if (msg.command == Success) {
                    numberSuccess++;
                }
            }
        }

        executeAll(client, value);
    }

    return value;
}
