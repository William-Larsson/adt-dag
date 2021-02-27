#ifndef DAG_H
#define DAG_H

#include "list.h"

struct Vertex {
    int id;
    void *weight;
};

struct Edge {
    struct Vertex *from;
    struct Vertex *to;
    void *weight;
};

struct Dag {
    struct list *v_list;
    struct list *e_list;
    int id;
};

struct Dag *dag_create(void);

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

int dag_destroy(struct Dag *d, bool free_weight);


#endif