#ifndef QUEUE_H
#define QUEUE_H



struct Queue {
    struct list *l;
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
 */
int queue_dequeue(struct Queue *q);

/**
 * retrieves the element first in the queue
 */
int queue_peek(struct Queue *q);


#endif