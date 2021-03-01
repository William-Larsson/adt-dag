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

    res->value = v;
    v->id = d->id++;
    v->weight = w;

    return v;
}
int dag_add_edge(struct Dag *d, struct Vertex *a, struct Vertex *b, void *w) {
    // this would lead to a cycle
    if (dag_is_connected(d, b, a) == 1) {
        return -1;
    }
    struct Edge *e = malloc(sizeof(*e));

    if (e == NULL) {
        return -1;
    }

    node *res = list_insert_after(d->e_list, NULL, e);
    if (res == NULL) {
        free(e);
        return -1;
    }

    res->value = e;
    e->from = a;
    e->to = b;
    e->weight = w;

    b->in_count++;

    return 0;
}

struct Edge *dag_find_edge(struct Dag *d, struct Vertex *a, struct Vertex *b) {
    struct node *n = list_first(d->e_list);

    while (n != NULL) { 
        struct Edge *e = n->value;
        if (a->id == e->from->id && b->id == e->to->id) {
            return e;
        }

        n = list_next(n);
    }

    return NULL;
}

/**
 * returns: 1 if connected; 0 if not connected; -1 if an error occurred.
 */
int dag_is_connected(struct Dag *d, struct Vertex *a, struct Vertex *b) {
    if (!d || !a || !b) return -1;

    struct Queue *q = queue_create();
    if (q == NULL) {
        return -1;
    }

    queue_enqueue(q, a);

    while(!queue_is_empty(q)) {
        struct node *next = queue_dequeue(q);
        struct Vertex *v = next->value;

        if (v->id == b->id) {
            return 1;
        }

        // Find all nodes we can reach from v
        struct list *l = d->e_list;
        struct node *n = list_first(l);
        while (n != NULL) {
            struct Edge *e = n->value;
            if (v->id == e->from->id) {
                queue_enqueue(q, e->to);
            }

            n = list_next(n);
        }
    }

    return 0;
}

void *dag_weight_of_longest_path(struct Dag *d, 
                                struct Vertex *a, struct Vertex *b,
                                get_weight_func f, get_weight_func g) {
    struct list *all_paths = dag_get_all_paths(d, a, b);
    struct node *n = list_first(all_paths);

    void *curr_weight = NULL;

    while (n != NULL) {
        struct list *path = n->value;
        struct node *v_it = list_first(path);

        struct Vertex *from = v_it->value;
        struct Vertex *to = list_next(v_it)->value;
        struct Edge *e = dag_find_edge(d, from, to);

        void *weight = f(from->weight);
        weight = d->add(weight, g(e->weight));
        
        v_it = list_next(v_it);
        int i = 1;
        while (v_it != NULL) {
            struct Vertex *v = v_it->value;
            weight = d->add(weight, f(v->weight));

            if (i < path->size - 1) {
                struct Edge *edge = dag_find_edge(d, v, list_next(v_it)->value);
                weight = d->add(weight, g(edge->weight));
            }
            
            v_it = list_next(v_it);
            i++;
        }

        if (curr_weight == NULL || d->comp(curr_weight, weight) == GREATER_THAN) {
            curr_weight = weight;
        }

        n = list_next(n);
    }
    return curr_weight;
}

struct list *dag_topological_ordering(struct Dag *d) {

    return NULL;
}

/**
 * Each element in the list is a list of vertices describing the path.
 * returns - a list of all paths.
 */
struct list *dag_get_all_paths(struct Dag *d, struct Vertex *a, struct Vertex *b) {
    struct list *all_paths = list_create();

    struct Queue *queue = queue_create();
    struct list *first_path = list_create();

    list_insert_after(first_path, NULL, a);
    queue_enqueue(queue, first_path);

    while(!queue_is_empty(queue)) {
        struct list *path = queue_dequeue(queue)->value;
        struct Vertex *next = list_get_last(path)->value;

        if (next->id == b->id) {
            // The end of a path
            list_insert_last(all_paths, path);
        }

        // Find all nodes we can reach from `next`
        struct list *l = d->e_list;
        struct node *n = list_first(l);
        while (n != NULL) {
            struct Edge *e = n->value;
            if (next->id == e->from->id) {
                // create new path
                struct list *new_path = list_create();
                // copy path to new_path
                struct node *it = list_first(path);
                while (it != NULL) {
                    list_insert_last(new_path, it->value);

                    it = list_next(it);
                }
                list_insert_last(new_path, e->to);
                queue_enqueue(queue, new_path);
            }

            n = list_next(n);
        }
    }

    return all_paths;
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