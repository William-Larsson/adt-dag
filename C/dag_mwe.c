#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "dag.h"


int main(void) {
    struct Dag *d = dag_create(NULL, NULL);

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

    fprintf(stdout, "Is connected: v1, v3: %d\n", connected);
    // Prints out: Is connected: v1, v3: 1
    
    // Cleans up dag, 2nd param ensures weights are not freed
    dag_destroy(d, false);
    return 0;
}