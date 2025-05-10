#include "utils.h"
#include "queue.h"

void init_queue(Queue* q) {
    q->size = 0;
}


int enqueue(Queue* q, Request r) {
    if (q->size >= QUEUE_SIZE) {
        return -1;
    }

    int i = q->size - 1;
    while (i >= 0 && (q->items[i].lamport_clock > r.lamport_clock || (q->items[i].lamport_clock == r.lamport_clock && q->items[i].rank > r.rank))) {
        q->items[i + 1] = q->items[i];
        i--;
    }
    q->items[i + 1] = r;
    q->size++;
    return 0;
}

int dequeue(Queue* q, Request* out) {
    if (q->size == 0) {
        return -1;
    }
    *out = q->items[0];
    for (int i = 1; i < q->size; i++) {
        q->items[i - 1] = q->items[i];
    }
    q->size--;
    return 0;
}

int is_queue_empty(Queue* q) {
    return q->size == 0;
}
