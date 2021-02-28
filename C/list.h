#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

/*
 * list.c
 * Tor Vallin (c18tvn).
 * Declaration of a singly linked list. 
 */

typedef struct list list;
typedef struct node node;
typedef int val_cmp_func(const void *, const void *);

struct node {
    void *value;
    struct node *next;
};

struct list {
    struct node *head;
};

/**
 * list_create() - Creates a new empty list.
 * Returns: The newly created list or NULL if malloc fails when allocating 
 *          memory for the list.
 */
list *list_create(void);

/**
 * list_first() - Returns the first node in the list.
 * @l: list to retrieve the node from.
 * 
 * Returns: The first node in the list l.
 */
node *list_first(list *l);

/**
 * list_is_empty() - Check if the given list, l, is empty.
 * @l: The list to check.
 * 
 * Returns: True if the list is empty; false otherwise.
 */
bool list_is_empty(list *l);

/**
 * list_next() - Returns the node after the given node, n.
 * n: Predecessor of the node to be returned.
 * 
 * Returns: The node after the node, n.
 */
node *list_next(node *n);

/**
 * list_inspect() - Inspects the value stored in node n.
 * @n: Node to be inspected.
 * 
 * Returns: The value stored in node n.
 */
void *list_inspect(node *n);

/**
 * list_insert_after() - Inserts a new node into the list, l, after n.     
 * @l: The list where the node shall be inserted.
 * @n: The predecessor of the new node.
 * @val: Value of the node to be inserted.          
 * 
 * If n is NULL, the new node will be inserted first in the list.
 * 
 * Returns: The newly created & inserted node; NULL on failure.
 */
node *list_insert_after(list *l, node *n, void *val);

/**
 * list_remove_after() - Removes the node after the node n.
 * @l: List where the node to be removed is stored in.
 * @n: Predecessor of the node to be removed.
 * 
 * If n is null,the first node is removed (the node after the head).
 * 
 * Returns: 0 if the node after n was removed; -1 otherwise.
 */
int list_remove_after(list *l, node *n);

/**
 * list_destroy() - Frees memory used by the list.
 * l: List structure to be freed.
 * 
 * Note that the function only frees the head node and the list structure
 * itself, data stored in the nodes must be freed manually.
 * 
 * Returns: Nothing.
 */
void list_destroy(list *l);

#endif