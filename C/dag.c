#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"
#include "list.h"
#include "dag.h"

struct Dag *dag_create(void) {
    struct Dag *d = malloc(sizeof(*d));
    if (!d) {
        return NULL;
    }

    d->v_list = list_create();
    d->e_list = list_create();

    if (!d->v_list || !d->e_list) {
        free(d);
        return NULL;
    }

    d->id = 0;

    return d;
}

struct Vertex *dag_add_vertex(struct Dag *d, void *w) {
    struct Vertex *v = malloc(sizeof(*v));
    if (v == NULL) {
        return NULL;
    }


    node *res = list_insert_after(d->v_list, NULL, v);
    if (res == NULL) {
        free(v);
        return NULL;
    }

    v->id = d->id++;
    v->weight = w;

    return NULL;
}
int dag_add_edge(struct Dag *d, struct Vertex *a, struct Vertex *b, void *w) {
    struct Edge *e = malloc(sizeof(*e));

    if (e == NULL) {
        return -1;
    }

    node *res = list_insert_after(d->e_list, NULL, e);
    if (res == NULL) {
        free(e);
        return -1;
    }



    return 0;
}

int dag_is_connected(struct Dag *d, struct Vertex *a, struct Vertex *b) {
    if (!d || !a || !b) return -1;

    struct Queue *l = queue_create();

    return 0;
}

int dag_destroy(struct Dag *d, bool free_weight) {
    struct node *n = list_first(d->v_list);

    while (n) {
        struct Vertex *v = list_inspect(n);
        if (free_weight)
            free(v->weight);
        free(v);
        n = list_first(d->v_list);
    }

    return 0;
}