#ifndef DAG_H
#define DAG_H

#include "list.h"

// add and compare
// weight interpret

typedef enum WeightComp (*weight_comp_func)(void *, void *);
typedef void* (*add_weight_func)(void *, void *);
typedef void* (*get_weight_func)(void *);

enum WeightComp {
    GREATER_THAN,
    LESS_THAN,
    EQUAL
};

struct Vertex {
    int id;
    int in_count;
    void *weight;
};

struct Edge {
    struct Vertex *from;
    struct Vertex *to;
    void *weight;
};

struct Dag {
    add_weight_func add;
    weight_comp_func comp;
    struct list *v_list;
    struct list *e_list;
    int id;
};

struct Dag *dag_create(void);
struct Dag *dag_clone(struct Dag *d);

/**
 * Adds a new vertex to the graph. The Vertex will have weight w
 * reutrns - the new vertex if it was created successfully, NULL otherwise.
 */
struct Vertex *dag_add_vertex(struct Dag *d, void *w);

/**
 * Adds an edge between a and b to the graph. The edge will be w.
 * Failure to add the edge could be because the edge would introduce a cycle.
 * 
 * returns - 0 if the edge was added; -1 on failure.
 */
int dag_add_edge(struct Dag *d, struct Vertex *a, struct Vertex *b, void *w);

struct Edge *dag_find_edge(struct Dag *d, struct Vertex *a, struct Vertex *b);

int dag_is_connected(struct Dag *d, struct Vertex *a, struct Vertex *b);

int dag_destroy(struct Dag *d, bool free_weight);

struct list *dag_get_all_paths(struct Dag *d, 
                               struct Vertex *a, struct Vertex *b);

void *dag_weight_of_longest_path(struct Dag *d, 
                                struct Vertex *a, struct Vertex *b,
                                get_weight_func f, get_weight_func g);

struct list *dag_topological_ordering(struct Dag *d);

static void dag_destroy_path(struct list *path);
void dag_all_paths_list_destroy(struct list *all_paths);

#endif