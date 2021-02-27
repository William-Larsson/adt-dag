#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "dag.h"

void test_no_cycles(void);

int main(void) {
    // struct Dag *d = dag_create();

    // int w1 = 1, w2 = 2, w3 = 3;

    // struct Vertex *v1 = dag_add_vertex(d, &w1);
    // struct Vertex *v2 = dag_add_vertex(d, &w2);
    // struct Vertex *v3 = dag_add_vertex(d, &w3);

    test_no_cycles();
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
    } else {
        fprintf(stdout, "SUCCESS: dag_test\n");
    }

    dag_destroy(d, false);
}