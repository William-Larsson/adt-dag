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

/**
 * Creates a deep clone of the given dag.
 */
struct Dag *dag_clone(struct Dag *d) {
    struct Dag *clone = dag_create();
    clone->id = d->id;
    clone->add = d->add;
    clone->comp = d->comp;

    struct list *v_list = d->v_list;
    node *v_it = list_first(d->v_list);
    while(v_it != NULL) {
        list_insert_after(clone->v_list, NULL, v_it->value);

        v_it = list_next(v_it);
    }

    struct list *e_list = d->e_list;
    node *e_it = list_first(d->e_list);
    while (e_it != NULL) {
        list_insert_after(clone->e_list, NULL, e_it->value);

        e_it = list_next(e_it);
    }

    return clone;
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
    v->in_count = 0;

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
 * Gets all edges from the given node.
 * 
 */
struct list *dag_get_edges_from(struct Dag *d, struct Vertex *a) {
    struct list *res = list_create();
    struct node *e_it = list_first(d->e_list);

    while (e_it != NULL) {
        struct Edge *e = e_it->value;
        if (e->from->id == a->id) {
            list_insert_last(res, e);
        }
        e_it = list_next(e_it);
    }

    return res;
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
            free(next);
            queue_destroy(q);
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

        free(next);
    }

    queue_destroy(q);

    return 0;
}

void *dag_weight_of_longest_path(struct Dag *d,
                                struct Vertex *a, struct Vertex *b,
                                get_weight_func f, get_weight_func g) {
    struct list *all_paths = dag_get_all_paths(d, a, b);
    struct node *n = list_first(all_paths);

    void *curr_weight = NULL;
    void *first = NULL;
    void *prev;

    while (n != NULL) {
        struct list *path = n->value;
        struct node *v_it = list_first(path);

        struct Vertex *from = v_it->value;
        struct Vertex *to = list_next(v_it)->value;
        struct Edge *e = dag_find_edge(d, from, to);

        void *weight = NULL;
        weight = d->add(weight, f(from->weight));
        prev = weight;
        weight = d->add(weight, g(e->weight));
        free(prev);
        
        v_it = list_next(v_it);

        int i = 1;
        while (v_it != NULL) {
            struct Vertex *v = v_it->value;
            prev = weight;
            weight = d->add(weight, f(v->weight));
            free(prev);

            if (i < path->size - 1) {
                struct Edge *edge = dag_find_edge(d, v, list_next(v_it)->value);
                prev = weight;
                weight = d->add(weight, g(edge->weight));
                free(prev);
            }
        
            v_it = list_next(v_it);
            i++;
        }

        if (curr_weight == NULL || d->comp(curr_weight, weight) == GREATER_THAN) {
            if (curr_weight) free(curr_weight);
            curr_weight = weight;
        } else {
            free(weight);
        }
        
        n = list_next(n);
    }


    dag_all_paths_list_destroy(all_paths);

    return curr_weight;
}

struct list *dag_topological_ordering(struct Dag *d) {
    struct Dag *graph = dag_clone(d);

    struct list *sorted_list = list_create();
    struct list *no_incoming_edges = list_create();

    // Store all vertices with no incoming edges in the given list.
    struct node *n = list_first(graph->v_list);
    while (n != NULL) {
        struct Vertex *v = n->value;
        if (v->in_count == 0) {
            list_insert_after(no_incoming_edges, NULL, v);
        }
        n = list_next(n);
    }

    while(no_incoming_edges->size > 0 && list_first(no_incoming_edges)) {
        struct Vertex *f_node = list_first(no_incoming_edges)->value;
        list_insert_last(sorted_list, f_node);
        list_remove_after(no_incoming_edges, NULL);        

        struct list *edges = dag_get_edges_from(graph, f_node);
        if (edges->size > 0) {
            // Loop through all edges that have an edge from `node`
            struct node *iterator = list_first(edges);
            while (iterator != NULL) {
                struct Edge *edge = iterator->value;
                edge->to->in_count--;

                list_remove_after(edges, NULL);

                if (edge->to->in_count == 0) {
                    list_insert_last(no_incoming_edges, edge->to);
                }

                iterator = list_first(edges);
            }
        }

        struct node *e = list_first(edges);
        while (e != NULL) {
            list_remove_after(edges, list_first(edges));
            e = list_first(edges);
        }
        list_destroy(edges);
    }

    list_destroy(no_incoming_edges);
    
    while (list_first(graph->e_list)) {
        list_remove_after(graph->e_list, NULL);
    } 
    while (list_first(graph->v_list)) {
        list_remove_after(graph->v_list, NULL);
    }
    list_destroy(graph->e_list);
    list_destroy(graph->v_list);
    free(graph);

    return sorted_list;
}

struct list *dag_get_all_paths(struct Dag *d, struct Vertex *a, struct Vertex *b) {
    struct list *all_paths = list_create();

    struct Queue *queue = queue_create();
    struct list *first_path = list_create();

    list_insert_after(first_path, NULL, a);
    queue_enqueue(queue, first_path);

    while(!queue_is_empty(queue)) {
        struct node *p = queue_dequeue(queue);
        struct list *path = p->value;
        struct Vertex *next = list_get_last(path)->value;

        free(p);

        int has_path = 0;
        if (next->id == b->id) {
            // The end of a path
            has_path = 1;
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

        // This implies that the current path is a dead end
        if (has_path == 0) {
            dag_destroy_path(path);
        }
    }

    queue_destroy(queue);

    return all_paths;
}

int dag_destroy(struct Dag *d, bool free_weight) {
    while (list_first(d->v_list)) {
        struct Vertex *v = list_first(d->v_list)->value;
        if (free_weight)
            free(v->weight);
        free(v);

        list_remove_after(d->v_list, NULL);
    }

    while (list_first(d->e_list)) {
        struct Edge *e = list_first(d->e_list)->value;
        if (free_weight) {
            free(e->weight);
        }
        free(e);

        list_remove_after(d->e_list, NULL);
    }        

    list_destroy(d->v_list);
    list_destroy(d->e_list);

    free(d);

    return 0;
}
void dag_destroy_path(struct list *path) {
    struct node *v_it = list_first(path);
    while (v_it != NULL) {
        list_remove_after(path, NULL);
        v_it = list_first(path);
    }
    
    list_destroy(path);
}

void dag_all_paths_list_destroy(struct list *all_paths) {
    struct node *n = list_first(all_paths);
    while (n != NULL) {
        dag_destroy_path(n->value);

        list_remove_after(all_paths, NULL);
        n = list_first(all_paths);
    }

    list_destroy(all_paths);  
}
