#ifndef QUEUE_H
#define QUEUE_H



struct Queue {
    struct node *first;
    struct node *last;
};

struct Queue *queue_create();

/**
 * adds an element to the end of the queue.
 */
int queue_enqueue(struct Queue *q, void *e);

/**
 * removes an element from the beginning of the queue 
 * returns - the node that was dequeued.
 */
struct node *queue_dequeue(struct Queue *q);

/**
 * retrieves the element first in the queue
 */
struct node *queue_peek(struct Queue *q);

bool queue_is_empty(struct Queue *q);

int queue_destroy(struct Queue *q);

#endif