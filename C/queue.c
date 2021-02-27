#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "queue.h"
#include "list.h"

struct Queue *queue_create() {
    struct Queue *q = malloc(sizeof(*q));
    if (q == NULL) {
        return NULL;
    }

    q->l = list_create();
    
    if (q->l == NULL) {
        free(q);
        return NULL;
    }

    q->first = NULL;
    q->last = NULL;
}

/**
 * adds an element to the end of the queue.
 */
int queue_enqueue(struct Queue *q, void *e) {
    return 0;
}

/**
 * removes an element from the beginning of the queue 
 */
int queue_dequeue(struct Queue *q) {
    return 0;
}

/**
 * retrieves the element first in the queue
 */
int queue_peek(struct Queue *q) {
    return 0;
}