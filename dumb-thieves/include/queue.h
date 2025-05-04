#ifndef QUEUE_H
#define QUEUE_H

#include "request.h"

#define QUEUE_SIZE 16

typedef struct Queue {
    Request items[QUEUE_SIZE];
    int size;
} Queue;

void init_queue(Queue* q);
int enqueue(Queue* q, Request r);
int dequeue(Queue* q, Request* out);
int is_queue_empty(Queue* q);

#endif //QUEUE_H
