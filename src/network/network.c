#include "network.h"

#include "../messages/queue.h"

#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

/*
    Initialization
*/

Network *initNetwork() {
    Network *network = (Network *) malloc(sizeof(Network));

    if (network == NULL) return NULL;


    network->nodes = NULL;

    return network;
}

void destroyNetwork(Network *network) {
    if (network == NULL) return;

    // Free all nodes in node list
    NetworkListNode *curr = network->nodes;
    NetworkListNode *next;

    while(curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }

    // Free network
    free(network);
}

/*
    Adding / Removing nodes
*/

int addNode(Network *network, int id) {
    NetworkListNode *new = (NetworkListNode *) malloc(sizeof(NetworkListNode));

    if (new == NULL) return -1;
    if (containsNode(network, id)) return -1;

    new->id = id;
    new->next = network->nodes;
    network->nodes = new;

    return 0;
}

bool containsNode(Network *network, int id) {
    NetworkListNode *curr = network->nodes;

    while (curr != NULL) {
        if (curr->id == id) return true;
        curr = curr->next;
    }

    return false;
}

NetworkListNode *getNetworkListNode(Network *network, int id) {
    NetworkListNode *curr = network->nodes;

    while (curr != NULL) {
        if (curr->id == id) return curr;
        curr = curr->next;
    }

    return NULL;
}

NetworkListNode *getPrevNetworkListNode(Network *network, int id) {
    NetworkListNode *curr = network->nodes;
    NetworkListNode *prev = NULL;

    while (curr != NULL) {
        if (curr->id == id) return prev;
        prev = curr;
        curr = curr->next;
    }

    return NULL;
}

int size(Network *network) {
    NetworkListNode *curr = network->nodes;
    int size = 0;

    while (curr != NULL) {
        size++;
        curr = curr->next;
    }

    return size;
}

void removeNode(Network *network, int id) {
    NetworkListNode *listNode = getNetworkListNode(network, id);

    if (listNode == NULL) return;

    if (network->nodes == listNode) {
        network->nodes = listNode->next;
    } else {
        NetworkListNode *prev = getPrevNetworkListNode(network, id);
        prev->next = listNode->next;
    }

    free(listNode);
    listNode = NULL;
}

/*
    Setup Client / Server
*/

int setupClient(uint16_t port) {
    int client_fd;
    struct sockaddr_in server_addr;
    
    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        printf("\nInvalid address\n");
        return -1;
    }

    int status;

    if((status = connect(client_fd, (struct sockaddr*) &server_addr, sizeof(server_addr))) < 0) {
        printf("\nConnection failed\n");
        return -1;
    }

    return client_fd;
}

int setupServer(uint16_t port) {
    int server_fd;
    struct sockaddr_in address;
    
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

int acceptConnection(int server_fd) {
    int client_fd;
    struct sockaddr_in address;
    socklen_t client_addrlen = sizeof(address);

    if((client_fd = accept(server_fd, (struct sockaddr*) &address, &client_addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    return client_fd;
}

/*
    Sending / receiving messages
*/

void broadcastMessage(Network *network, Message *msg){
    NetworkListNode *node = network->nodes;

    while(node != NULL) {
        sendMessage(node->id, msg);

        node = node->next;
    }
}

void sendMessage(int receiver_fd, Message *message) {
    unsigned char serialized_message[sizeof(*message)], *ptr;

    ptr = serialize_message(serialized_message, message);

    send(receiver_fd, serialized_message, ptr - serialized_message, 0);
    printf("Message sent\n");
}

Message receiveMessage(int node_fd) {
    Message message;
    unsigned char serialized_message[1024];

    read(node_fd, serialized_message, 1024 - 1);

    deserialize_message(serialized_message, &message);

    return message;
}
