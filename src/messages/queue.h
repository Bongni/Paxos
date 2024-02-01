#ifndef NETWORK_QUEUE_H
#define NETWORK_QUEUE_H

#include "message.h"

#include <stdlib.h>

/*
    Datastructures
*/

typedef struct QueueNode {
    Message msg;
    struct QueueNode *next;
    struct QueueNode *prev;
} QueueNode;

typedef struct {
    QueueNode *first;
    QueueNode *last;
} Queue;

/*
    Initialization
*/

Queue *initQueue();

void destroyQueue(Queue *queue);

/*
    Adding / Removing nodes
*/

void push(Queue *queue, Message msg);

Message pop(Queue *queue);

int length(Queue *queue);

#endif