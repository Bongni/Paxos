#include "client.h"

#include "../networkSimulation/network.h"
#include "server.h"
#include "../helpers/timer.h"

#include <time.h>
#include <pthread.h>
#include <stdio.h>

/*
    Initialization
*/

Node initClient(int id, int value) {
    Network *network = initNetwork();
    NetworkNode *node = initNode(id);

    addNode(network, node);

    return (Node) {id, value,  node, network};
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

void *paxosClient(Node *client) {

    srand(time(NULL) + client->id);
    int ticket = rand();


    int stop;

    int timeout = 1000;
    clock_t start = clock();

    while(timer(start) < timeout) {

        sched_yield();

    /*
        Phase 1
    */

        ticket++;
        printf("Client %d requestAll(%d)\n", client->id, ticket);
        requestAll(client, ticket);

        sched_yield();

    /*
        Phase 2
    */

        int n = size(client->network);
        int numberOk = 0;

        int maxTicket = 0;
        Network *majority = initNetwork();

        int timeoutPhase2 = 100;
        clock_t startPhase2 = clock();

        while(numberOk <= n / 2 && (stop = timer(startPhase2)) < timeoutPhase2) {
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

        if (stop >= timeoutPhase2) continue;

        printf("Client %d proposeMajority(%d, %d)\n", client->id, ticket, client->value);
        proposeMajority(client, majority, ticket, client->value);

        sched_yield();

    /*
        Phase 3
    */

        int numberSuccess = 0;

        int timeoutPhase3 = 100;
        clock_t startPhase3 = clock();

        while(numberSuccess <= n / 2 && (stop = timer(startPhase3)) < timeoutPhase3) {  /*TODO add timer*/
            sched_yield();

            while(canReceiveMessage(client->network, client->id)) {
                Message msg = receiveMessage(client->network, client->id);

                if (msg.command == Success) {
                    numberSuccess++;
                }
            }
        }

        if (stop >= timeoutPhase3) continue;

        printf("Client %d executeAll(%d)\n", client->id, client->value);
        executeAll(client, client->value);

        pthread_exit(&client->value);
        return NULL;
    }

    printf("Client %d timed out ...\n", client->id);
    pthread_exit(&client->value);
}
