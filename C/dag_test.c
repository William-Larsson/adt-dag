#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "dag.h"

void test_connected(void);
void test_connected_large(void);
void test_no_cycles(void);

int main(void) {
    test_connected();
    test_connected_large();
    test_no_cycles();

    return 0;
}

void test_connected(void) {
    struct Dag *d = dag_create();

    int w1 = 1, w2 = 2, w3 = 3;

    struct Vertex *v1 = dag_add_vertex(d, &w1);
    struct Vertex *v2 = dag_add_vertex(d, &w2);
    struct Vertex *v3 = dag_add_vertex(d, &w3);

    int res1 = dag_add_edge(d, v1, v2, &w1); 
    int res2 = dag_add_edge(d, v2, v3, &w1); 
    int res3 = dag_add_edge(d, v1, v3, &w1); 

    if (res1 < 0 || res2 < 0 || res3 < 0) {
        fprintf(stderr, "ERROR: test_connected: fail to add edge\n");
    }

    if (dag_is_connected(d, v1, v2) < 0) {
        fprintf(stderr, "ERROR: test_connected: fail\n");
    } 
    if (dag_is_connected(d, v2, v3) < 0) {
        fprintf(stderr, "ERROR: test_connected: fail\n");
    }
    if (dag_is_connected(d, v1, v3) < 0) {
        fprintf(stderr, "ERROR: test_connected: fail\n");
    }

    if (dag_is_connected(d, v2, v1) == 1) {
        fprintf(stderr, "ERROR: test_connected: fail\n");
    }
    if (dag_is_connected(d, v3, v1) == 1) {
        fprintf(stderr, "ERROR: test_connected: fail\n");
    }
    if (dag_is_connected(d, v3, v2) == 1) {
        fprintf(stderr, "ERROR: test_connected: fail\n");
    }
}

// Based on this graph from Wikipedia:
// https://upload.wikimedia.org/wikipedia/commons/thumb/6/61/Polytree.svg/768px-Polytree.svg.png
void test_connected_large(void) {
    struct Dag *d = dag_create();

    int w1 = 1, w2 = 2, w3 = 3;

    struct Vertex *A = dag_add_vertex(d, &w1);
    struct Vertex *B = dag_add_vertex(d, &w2);
    struct Vertex *C = dag_add_vertex(d, &w3);
    struct Vertex *D = dag_add_vertex(d, &w3);
    struct Vertex *E = dag_add_vertex(d, &w3);
    struct Vertex *F = dag_add_vertex(d, &w3);
    struct Vertex *G = dag_add_vertex(d, &w3);
    struct Vertex *I = dag_add_vertex(d, &w3);


    int res = dag_add_edge(d, A, D, &w1); 
    res -= dag_add_edge(d, A, C, &w1); 
    res -= dag_add_edge(d, B, D, &w1); 
    res -= dag_add_edge(d, D, F, &w1); 
    res -= dag_add_edge(d, D, G, &w1); 
    res -= dag_add_edge(d, G, I, &w1); 

    if (res != 0) {
        fprintf(stderr, "ERROR: test_connected_large, could not add all edges");
    }

    if (dag_is_connected(d, A, C) < 0) {
        fprintf(stderr, "ERROR: test_connected_large, connect fail");
    }
    if (dag_is_connected(d, A, D) < 0) {
        fprintf(stderr, "ERROR: test_connected_large, connect fail");
    }
    if (dag_is_connected(d, A, F) < 0) {
        fprintf(stderr, "ERROR: test_connected_large, connect fail");
    }
    if (dag_is_connected(d, A, G) < 0) {
        fprintf(stderr, "ERROR: test_connected_large, connect fail");
    }
    if (dag_is_connected(d, A, I) < 0) {
        fprintf(stderr, "ERROR: test_connected_large, connect fail");
    }
    if (dag_is_connected(d, D, G) < 0) {
        fprintf(stderr, "ERROR: test_connected_large, connect fail");
    }
    if (dag_is_connected(d, D, I) < 0) {
        fprintf(stderr, "ERROR: test_connected_large, connect fail");
    }

}

void test_no_cycles(void) {
    struct Dag *d = dag_create();

    int w1 = 1, w2 = 2;

    struct Vertex *v1 = dag_add_vertex(d, &w1);
    struct Vertex *v2 = dag_add_vertex(d, &w2);

    int res1 = dag_add_edge(d, v1, v2, &w1); 
    int res2 = dag_add_edge(d, v2, v1, &w1); 

    if (res2 != -1) {
        fprintf(stderr, "ERROR: test_no_cycles - Graph allows cycles!\n");
    }

    //dag_destroy(d, false);
}