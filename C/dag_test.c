#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "dag.h"

void test_connected(void);
void test_connected_large(void);
void test_no_cycles(void);
void test_all_paths(void);
void test_longest_path(void);
void test_longest_path_large(void);
void test_topological_ordering(void);
void test_small_topological_ordering(void);
void test_topological_ordering_large(void);

int main(void) {
    test_no_cycles();
    test_connected();
    test_connected_large();
    test_all_paths();
    test_longest_path();
    test_longest_path_large();
    test_small_topological_ordering();
    test_topological_ordering_large();
    
    return 0;
}

void test_connected(void) {
    struct Dag *d = dag_create();

    int w1 = 5, w2 = 10, w3 = 15, w4 = 20;

    struct Vertex *A = dag_add_vertex(d, &w1);
    struct Vertex *B = dag_add_vertex(d, &w2);
    struct Vertex *C = dag_add_vertex(d, &w3);
    struct Vertex *D = dag_add_vertex(d, &w4);

    int res = dag_add_edge(d, A, B, &w1); 
    res -= dag_add_edge(d, A, C, &w2); 
    res -= dag_add_edge(d, B, D, &w2); 

    if (res < 0) {
        fprintf(stderr, "ERROR: test_connected: fail to add edge\n");
    }

    if (dag_is_connected(d, A, B) < 0) {
        fprintf(stderr, "ERROR: test_connected: fail\n");
    } 
    if (dag_is_connected(d, A, C) < 0) {
        fprintf(stderr, "ERROR: test_connected: fail\n");
    }
    if (dag_is_connected(d, A, D) < 0) {
        fprintf(stderr, "ERROR: test_connected: fail\n");
    }
    if (dag_is_connected(d, B, D) < 0) {
        fprintf(stderr, "ERROR: test_connected: fail\n");
    }

    if (dag_is_connected(d, B, A) == 1) {
        fprintf(stderr, "ERROR: test_connected: fail\n");
    }
    if (dag_is_connected(d, C, A) == 1) {
        fprintf(stderr, "ERROR: test_connected: fail\n");
    }
    if (dag_is_connected(d, D, B) == 1) {
        fprintf(stderr, "ERROR: test_connected: fail\n");
    }
    if (dag_is_connected(d, B, C) == 1) {
        fprintf(stderr, "ERROR: test_connected: fail\n");
    }

    dag_destroy(d, false);
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

    dag_destroy(d, false);
}

void test_no_cycles(void) {
    struct Dag *d = dag_create();

    int w1 = 1, w2 = 2;

    struct Vertex *v1 = dag_add_vertex(d, &w1);
    struct Vertex *v2 = dag_add_vertex(d, &w2);

    dag_add_edge(d, v1, v2, &w1); 
    int res2 = dag_add_edge(d, v2, v1, &w1); 

    if (res2 != -1) {
        fprintf(stderr, "ERROR: test_no_cycles - Graph allows cycles!\n");
    }

    dag_destroy(d, false);
}

void test_all_paths(void) {
     struct Dag *d = dag_create();

    // all paths from A to C
    // A (0) -> B (1) -> C (2)
    // A (0) -> D (3) -> C (2)
    // A (0) -> E (4)-> C (2)
    int w1 = 1, w2 = 2, w3 = 3;
    struct Vertex *A = dag_add_vertex(d, &w1);
    struct Vertex *B = dag_add_vertex(d, &w2);
    struct Vertex *C = dag_add_vertex(d, &w3);
    struct Vertex *D = dag_add_vertex(d, &w3);
    struct Vertex *E = dag_add_vertex(d, &w3);

    int res = 0;
    res -= dag_add_edge(d, A, B, &w1); 
    res -= dag_add_edge(d, A, D, &w1); 
    res -= dag_add_edge(d, A, E, &w1); 
    res -= dag_add_edge(d, B, C, &w1); 
    res -= dag_add_edge(d, D, C, &w1); 
    res -= dag_add_edge(d, E, C, &w1);   

    if (res != 0) {
        fprintf(stderr, "ERROR: test_all_paths - Could not add edge\n");

    }

    struct list *all_paths = dag_get_all_paths(d, A, C);
    struct node *n = list_first(all_paths);

    while (n != NULL) {
        fprintf(stdout, "Path: ");
        struct list *path = n->value;
        struct node *v_it = list_first(path);
        while (v_it != NULL) {
            struct Vertex *v = v_it->value;
            fprintf(stdout, "%d ", v->id);

            v_it = list_next(v_it);
        }
        fprintf(stdout, "\n");

        n = list_next(n);
    }

    dag_all_paths_list_destroy(all_paths);
    dag_destroy(d, false);
}

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

void test_longest_path(void) {
    struct Dag *d = dag_create();
    d->add = add_ints;
    d->comp = int_compare;


    // all paths from A to C
    // A (0) -> B (1) -> C (2) -- cost: 1 + 1 + 2 + 1 + 4 = 9
    // A (0) -> D (3) -> C (2) -- cost: 1 + 1 + 4 + 1 + 3 = 10
    // A (0) -> E (4) -> C (2) -- cost: 1 + 1 + 5 + 1 + 3 = 11
    int w1 = 1, w2 = 2, w3 = 3, w4 = 4, w5 = 5;
    struct Vertex *A = dag_add_vertex(d, &w1);
    struct Vertex *B = dag_add_vertex(d, &w2);
    struct Vertex *C = dag_add_vertex(d, &w3);
    struct Vertex *D = dag_add_vertex(d, &w4);
    struct Vertex *E = dag_add_vertex(d, &w5);

    int res = 0;
    res -= dag_add_edge(d, A, B, &w1); 
    res -= dag_add_edge(d, A, D, &w1); 
    res -= dag_add_edge(d, A, E, &w1); 
    res -= dag_add_edge(d, B, C, &w1); 
    res -= dag_add_edge(d, D, C, &w1); 
    res -= dag_add_edge(d, E, C, &w1);   

    if (res != 0) {
        fprintf(stderr, "ERROR: test_all_paths - Could not add edge\n");

    }

    int *weight = dag_weight_of_longest_path(d, A, C, get_int, get_int);

    if (*weight != 11) {
        fprintf(stderr, "ERROR: test_longest_path - incorrect weight %d\n", *weight);
    }

    free(weight);
    dag_destroy(d, false);
}

void test_longest_path_large(void) {
    struct Dag *d = dag_create();
    d->add = add_ints;
    d->comp = int_compare;

    int vw[] = {1, 2, 2, 6, 5, 15, 20, 25};
    struct Vertex *A = dag_add_vertex(d, &vw[0]);
    struct Vertex *B = dag_add_vertex(d, &vw[1]);
    struct Vertex *C = dag_add_vertex(d, &vw[2]);
    struct Vertex *D = dag_add_vertex(d, &vw[3]);
    struct Vertex *E = dag_add_vertex(d, &vw[4]);
    struct Vertex *F = dag_add_vertex(d, &vw[5]);
    struct Vertex *G = dag_add_vertex(d, &vw[6]);
    struct Vertex *H = dag_add_vertex(d, &vw[7]);

    int ew[] = {1, 2, 2, 5, 6, 3, 2, 7, 8, 4};
    int res = dag_add_edge(d, A, B, &ew[0]); 
    res -= dag_add_edge(d, A, D, &ew[1]); 
    res -= dag_add_edge(d, B, C, &ew[2]); 
    res -= dag_add_edge(d, B, D, &ew[3]); 
    res -= dag_add_edge(d, B, E, &ew[4]); 
    res -= dag_add_edge(d, C, E, &ew[5]); 
    res -= dag_add_edge(d, C, H, &ew[6]); 
    res -= dag_add_edge(d, D, E, &ew[7]); 
    res -= dag_add_edge(d, E, F, &ew[8]); 
    res -= dag_add_edge(d, E, G, &ew[9]); 

//      These are all the paths from a to g
//        a -> d -> e -> g (weight 1 + 2 + 6 + 7 + 5 + 4 + 20 = 45)
//        a -> b -> d -> e -> g (weight 1 + 1 + 2 + 5 + 6 + 7 + 5 + 4 + 20 = 51)
//        a -> b -> e -> g (weight 1 + 1 + 2 + 6 + 5 + 4 + 20 = 39)
//        a -> b -> c -> e -> g (weight 1 + 1 + 2 + 1 + 2 + 3 + 5 + 4 + 20 = 39)
    int *longPath = dag_weight_of_longest_path(d, A, G, get_int, get_int);

    if (*longPath != 51) 
        fprintf(stderr, "ERROR: test_longest_path_large: invalid longest path: %d\n", *longPath);

    dag_destroy(d, false);
    free(longPath);
}

void test_small_topological_ordering(void) {
     struct Dag *d = dag_create();

    int w1 = 1, w2 = 2, w3 = 3, w4 = 4;
    struct Vertex *A = dag_add_vertex(d, &w1);
    struct Vertex *B = dag_add_vertex(d, &w2);
    struct Vertex *C = dag_add_vertex(d, &w3);
    struct Vertex *D = dag_add_vertex(d, &w4);

    int res = 0;
    res -= dag_add_edge(d, A, B, &w1); 
    res -= dag_add_edge(d, A, C, &w1);   
    res -= dag_add_edge(d, B, D, &w1);   

    struct list *l = dag_topological_ordering(d);
    struct node *it = list_first(l);

    fprintf(stdout, "Order: ");
    while (it != NULL) {
        struct Vertex *v = it->value;
        fprintf(stdout, "%d ", v->id);

        it = list_next(it);
    }   

    fprintf(stdout, "\n");

    dag_destroy(d, false);
    dag_destroy_path(l);
}

// Same graph as test_longest_path_large
void test_topological_ordering_large(void) {
    struct Dag *d = dag_create();
    d->add = add_ints;
    d->comp = int_compare;

    int vw[] = {1, 2, 2, 6, 5, 15, 20, 25};
    struct Vertex *A = dag_add_vertex(d, &vw[0]);
    struct Vertex *B = dag_add_vertex(d, &vw[1]);
    struct Vertex *C = dag_add_vertex(d, &vw[2]);
    struct Vertex *D = dag_add_vertex(d, &vw[3]);
    struct Vertex *E = dag_add_vertex(d, &vw[4]);
    struct Vertex *F = dag_add_vertex(d, &vw[5]);
    struct Vertex *G = dag_add_vertex(d, &vw[6]);
    struct Vertex *H = dag_add_vertex(d, &vw[7]);

    int ew[] = {1, 2, 2, 5, 6, 3, 2, 7, 8, 4};
    int res = dag_add_edge(d, A, B, &ew[0]); 
    res -= dag_add_edge(d, A, D, &ew[1]); 
    res -= dag_add_edge(d, B, C, &ew[2]); 
    res -= dag_add_edge(d, B, D, &ew[3]); 
    res -= dag_add_edge(d, B, E, &ew[4]); 
    res -= dag_add_edge(d, C, E, &ew[5]); 
    res -= dag_add_edge(d, C, H, &ew[6]); 
    res -= dag_add_edge(d, D, E, &ew[7]); 
    res -= dag_add_edge(d, E, F, &ew[8]); 
    res -= dag_add_edge(d, E, G, &ew[9]); 

    struct list *ordering = dag_topological_ordering(d);
    struct node *it = list_first(ordering);
    fprintf(stdout, "Ordererd graph: ");
    while (it != NULL) {
        struct Vertex *v = it->value;
        fprintf(stdout, "%d ", v->id);

        it = list_next(it);
    }   

    fprintf(stdout, "\n");

    dag_destroy_path(ordering);
    dag_destroy(d, false);
    
}