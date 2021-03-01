#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "queue.h"
#include "list.h"

struct node;

struct Queue *queue_create() {
    struct Queue *q = malloc(sizeof(*q));
    if (q == NULL) {
        return NULL;
    }

    q->first = NULL;
    q->last = NULL;

    return q;
}

/**
 * adds an element to the end of the queue.
 * returns - 0 on success; -1 on failure.
 */
int queue_enqueue(struct Queue *q, void *e) {
    struct node *n = malloc(sizeof(*n));
    if (n == NULL) { 
        return -1;
    }

    n->next = NULL;
    n->value = e;
    if (q->first == NULL || q->last == NULL) {
        q->first = n;
        q->last = n;
    } else {
        q->last->next = n;
        q->last = n;
    }

    return 0;
}

/**
 * removes an element from the beginning of the queue 
 */
struct node *queue_dequeue(struct Queue *q) {
    struct node *n = q->first;

    if (q->first)
        q->first = q->first->next;
        
    if (q->first == NULL)
        q->last = NULL;
    
    return n;
}

/**
 * retrieves the element first in the queue
 */
struct node *queue_peek(struct Queue *q) {
    return q->first;
}

bool queue_is_empty(struct Queue *q) {
    return q->first == NULL;
}

int queue_destroy(struct Queue *q) {
    node *n = queue_dequeue(q);

    while (n != NULL) {
        free(n);

        n = queue_dequeue(q);
    }

    free(q);
}