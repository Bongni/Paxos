#include "queue.h"

#include <stdlib.h>

#include "message.h"

/*
    Initialization
*/

Queue *initQueue() {
    Queue *queue = (Queue *) malloc(sizeof(Queue));

    if (queue == NULL) return NULL;


    queue->first = NULL;
    queue->last = NULL;

    return queue;
}

void destroyQueue(Queue *queue){
    if (queue == NULL) return;

    // Free all nodes queue
    QueueNode *curr = queue->first;
    QueueNode *next;

    while(curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }

    // Free queue
    free(queue);
}

/*
    Adding / Removing messages
*/

void push(Queue *queue, Message msg){
    QueueNode *node = (QueueNode *) malloc(sizeof(QueueNode));

    if(node == NULL) return;

    node->msg = msg;
    node->next = queue->last;
    node->prev = NULL;

    // Queue is empty
    if(queue->last == NULL) {
        queue->first = node;
    } else {
        queue->last->prev = node;
    }

    queue->last = node;
}

Message pop(Queue *queue){
    // If Queue is empty
    if (queue->first == NULL) return (Message) {-1, -1};

    Message first = queue->first->msg;

    // Only one element in queue
    if(queue->first == queue->last) {
        free(queue->first);
        queue->first = NULL;
        queue->last = NULL;
    } else {
        queue->first = queue->first->prev;
        queue->first->next = NULL;
    }

    return first;
}
