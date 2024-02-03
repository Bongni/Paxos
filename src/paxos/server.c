#include "server.h"

#include "../network/network.h"
#include "node.h"
#include "../helpers/timer.h"

#include <pthread.h>
#include <stdio.h>
#include <time.h>

/*
    Initialization
*/

Node initServer(int id, int value){
    Network *network = initNetwork();

    return (Node) {id, value, network};
}

void destroyServer(Node *server){
    destroyNetwork(server->network);
}

/*
    Add / Remove nodes
*/

void addClient(Node *server, int client){
    addNode(server->network, client);
}

void removeClient(Node *server, int client){
    removeNode(server->network, client);
}

/*
    Paxos
*/

void *paxosServer(Node *server){

    int timeout1 = 4000;
    int timeout2 = 8000;
    clock_t start = clock();

    while(timer(start) < timeout2) {

        int maxTicket = 0;
        int valueTicket = 0;
        server->value = 0;

        while(timer(start) < timeout1) {
            Message msg = receiveMessage(server->id);

            printf("Message received {%d, %d, %d, %d}\n", msg.sender, msg.command, msg.ticket, msg.value);

            if (msg.command == RequestTicket) {
                if(msg.ticket > maxTicket) {
                    maxTicket = msg.ticket;

                    printf("Server %d received request(%d) from client %d\n", server->id, msg.ticket, msg.sender);

                    Message ok = {server->id, Ok, valueTicket, server->value};
                    sendMessage(server->id, &ok);
                }
            } else if (msg.command == Propose) {
                if(msg.ticket == maxTicket) {
                    server->value = msg.value;
                    valueTicket = msg.ticket;

                    printf("Server %d received propose(%d, %d) from client %d\n", server->id, msg.ticket, msg.value, msg.sender);

                    Message success = {server->id, Success, valueTicket, server->value};
                    sendMessage(msg.sender, &success);
                }
            } else if (msg.command == Execute) {
                server->value = msg.value;
                printf("Server %d received execute(%d) from client %d\n", server->id, msg.value, msg.sender);
                pthread_exit(&server->value);
            }
        }
    }

    printf("Server %d timed out ...\n", server->id);
    pthread_exit(&server->value);
}
