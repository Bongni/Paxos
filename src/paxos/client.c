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

    while(true) {

        srand(time(NULL));
        int ticket = rand();

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

        while(numberOk <= n / 2) {  /*TODO add timer*/
            sched_yield();

            while(canReceiveMessage(client->network, client->id)) {
                Message msg = receiveMessage(client->network, client->id);

                if (msg.command == Ok) {
                    numberOk++;
                    // Add all nodes that sent ok to majority
                    addNode(majority, getNode(client->network, msg.sender));

                    if(msg.ticket > maxTicket) {
                        maxTicket = msg.ticket;
                        client->value = msg.value;
                    }
                }
            }
        }

        proposeMajority(client, majority, ticket, client->value);

    /*
        Phase 3
    */

        int numberSuccess = 0;

        while(numberSuccess <= n / 2) {  /*TODO add timer*/
            sched_yield();

            while(canReceiveMessage(client->network, client->id)) {
                Message msg = receiveMessage(client->network, client->id);

                if (msg.command == Success) {
                    numberSuccess++;
                }
            }
        }

        executeAll(client, client->value);

        pthread_exit(&client->value);
        return client->value;
    }
}
