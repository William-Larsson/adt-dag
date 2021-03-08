#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "dag.h"

enum WeightComp int_compare(void *a_v, void *b_v);
void *add_ints(void *a_v, void *b_v);
void *get_int(void *a_v);

enum WeightComp int_compare(void *a_v, void *b_v) {
    int *a = (int *) a_v;
    int *b = (int *) b_v;
    if (*a > *b) return GREATER_THAN;
    if (*a < *b) return LESS_THAN;

    return EQUAL;
}

void *add_ints(void *a_v, void *b_v) {
    int a;
    if (a_v == NULL)
        a = 0;
    else 
        a = *(int*) a_v;

    int b = *(int*) b_v;
    int *res = malloc(sizeof(*res));
    *res = a + b;
    return res;
}

void *get_int(void *a_v) {
    int *a = (int *) a_v;
    return a;
}

int main(void) {
    struct Dag *d = dag_create(add_ints, int_compare);

    int w1 = 1, w2 = 2, w3 = 3;

    struct Vertex *v1 = dag_add_vertex(d, &w1);
    struct Vertex *v2 = dag_add_vertex(d, &w2);
    struct Vertex *v3 = dag_add_vertex(d, &w3);

    int res = dag_add_edge(d, v1, v2, &w1); 
    res -= dag_add_edge(d, v2, v3, &w1); 
    res -= dag_add_edge(d, v1, v3, &w1); 

    if (res < 0)
        fprintf(stderr, "Failed to add edge!\n");
    
    int connected = dag_is_connected(d, v1, v3);
    if (connected < 0) 
        fprintf(stderr, "Is connected encountered an error\n");
    
    int *weight = dag_weight_of_longest_path(d, v1, v3, get_int, get_int);

    // Prints out: Is connected: v1, v3: 1
    fprintf(stdout, "Is connected: v1, v3: %d\n", connected);
    // Prints out: Longest path weight: 8
    fprintf(stdout, "Longest path weight: %d\n", *weight);
    
    // Cleans up dag, 2nd param ensures weights are not freed
    dag_destroy(d, false);
    free(weight);
    return 0;
}