#ifndef PAXOS_NODE_H
#define PAXOS_NODE_H

#include "../network/network.h"

/*
    Datastructures
*/

typedef struct {
    int id;
    int value;
    Network *network;
} Node;

#endif