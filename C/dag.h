#ifndef DAG_H
#define DAG_H

#include "list.h"

// Functions for comparing weights of the same type must follow this format.
typedef enum WeightComp (*weight_comp_func)(void *, void *);
// Functions for adding weights of the same type must follow this format.
typedef void* (*add_weight_func)(void *, void *);
// Functions for interpreting weights must follow this format.
typedef void* (*get_weight_func)(void *);

// Defines the results of a weight comparison.
enum WeightComp {
    GREATER_THAN,
    LESS_THAN,
    EQUAL
};

// These structs are defined in dag.c to hide internal representation.
struct Vertex;
struct Edge;
struct Dag;

/**
 * Creates a new dag.
 * return - the new dag on success; null on error.
 */
struct Dag *dag_create(add_weight_func add_func, weight_comp_func comp_func);

/**
 * Adds a new vertex to the graph. The Vertex will have weight w
 * reutrns - the new vertex if it was created successfully, NULL otherwise.
 */
struct Vertex *dag_add_vertex(struct Dag *d, void *w);

/**
 * Adds an edge between a and b to the graph. The edge will be w.
 * Failure to add the edge could be because the edge would introduce a cycle.
 * 
 * return - 0 if the edge was added; -1 on failure.
 */
int dag_add_edge(struct Dag *d, struct Vertex *a, struct Vertex *b, void *w);

/**
 * Gets the weight of the given vertex.
 * return - the weight of the given vertex.
 */
void *dag_v_get_weight(struct Vertex *v);

/**
 * Gets the ID of the given vertex
 * return - the ID of the given vertex.
 */
int dag_v_get_id(struct Vertex *v);

/**
 * Searches for an edge between vertex a and vertex b, and returns if it 
 * exists.
 * d - dag containing a and b.
 * a - start vertex.
 * b - destination vertex.
 * 
 * return - the edge from a to b if it exists; null otherwise.
 */
struct Edge *dag_find_edge(struct Dag *d, struct Vertex *a, struct Vertex *b);

/**
 * Checks if there is some path between vertex a and vertex b.
 * d - graph containing the vertices
 * a - starting vertex
 * b - destination vertex
 * returns: 1 if connected; 0 if not connected; -1 if an error occurred.
 */
int dag_is_connected(struct Dag *d, struct Vertex *a, struct Vertex *b);

/**
 * Traverses the graph and creates a list of paths between the vertices 
 * that is then returned.
 * d - dag containing the vertices a and b.
 * a - Starting vertex
 * b - Goal vertex
 * return - The path between a and b. This list must be destroy with
 *          dag_all_paths_list_destroy()
 */
struct list *dag_get_all_paths(struct Dag *d, 
                               struct Vertex *a, struct Vertex *b);

/**
 * Computes the weight of the longest path between the vertices a and b.
 * d - graph containing the vertices and edges.
 * a - Path start
 * b - Path end
 * f - function for interpreting the weight of the vertices
 * g - function for interpreting the weight of the edges.
 * return - the weight of the longest path between a and b. NULL is returned if 
 * f or g are NULL, or if `add` and `compare` functions are not defined.
 */
void *dag_weight_of_longest_path(struct Dag *d,
                                struct Vertex *a, struct Vertex *b,
                                get_weight_func f, get_weight_func g);

/**
 * Performs a topological ordering, using Kahn's algorithm.
 * dag - graph containing the vertices to sort.
 * return - A list containing the sorted elements. This list must be 
 *          freed by calling dag_destroy_path() to avoid memory leaks.
 */
struct list *dag_topological_ordering(struct Dag *d);

/**
 * Cleans up dynamically allocated resources. This will destroy the graph,
 * vertices and edges.
 * d - cleans up the resources used by the dag.
 * free_weight - if set to true, this function will free the weights used
 *               by the vertices and edges. Must be set to false if the weights
 *               are not dynamically allocated.
 */
int dag_destroy(struct Dag *d, bool free_weight);

/**
 * Destroys a path/frees all resources used by the list.
 * path - path to destroy.
 */
void dag_destroy_path(struct list *path);

/**
 * Destroys the path list returned by dag_get_all_paths(). all_paths is a list
 * of lists, and this function will iterate through all elements and free them.
 * all_paths - the list of paths to free.
 */
void dag_all_paths_list_destroy(struct list *all_paths);

#endif