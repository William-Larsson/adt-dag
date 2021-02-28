#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/*
 * list.c
 * Tor Vallin (c18tvn).
 * Implementation of a singly linked list. 
 */

/**
 * list_create() - Creates a new empty list.
 * Returns: The newly created list or NULL if malloc fails when allocating 
 *          memory for the list.
 */
list *list_create(void) {
    list *l = malloc(sizeof(list));
    if (l == NULL) {
        return NULL;
    }

    l->head = malloc(sizeof(node));
    if (l->head == NULL)  {
        return NULL;
    }

    l->head->next = NULL;

    return l;
}

/**
 * list_first() - Returns the first node in the list.
 * @l: list to retrieve the node from.
 * 
 * Returns: The first node in the list l.
 */
node *list_first(list *l) {    
    return l->head->next;
}

/**
 * list_is_empty() - Check if the given list, l, is empty.
 * @l: The list to check.
 * 
 * Returns: True if the list is empty; false otherwise.
 */
bool list_is_empty(list *l) { 
    return (l->head->next == NULL) ? true : false;
}

/**
 * list_next() - Returns the node after the given node, n.
 * n: Predecessor of the node to be returned.
 * 
 * Returns: The node after the node, n.
 */
node *list_next(node *n) {
    return (n == NULL) ? NULL : n->next;
}

/**
 * list_inspect() - Inspects the value stored in node n.
 * @n: Node to be inspected.
 * 
 * Returns: The value stored in node n.
 */
void *list_inspect(node *n) {
    return (n == NULL) ? NULL : n->value;
}

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
node *list_insert_after(list *l, node *n, void *val) {
    node *new_node = malloc(sizeof(node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->value = val;

    // Insert new_node to first available position (node after head).
    if (n == NULL) {
        new_node->next = l->head->next;
        l->head->next = new_node;
    } else {
        new_node->next = n->next;
        n->next = new_node;
    }

    return new_node;
}

/**
 * list_remove_after() - Removes the node after the node n.
 * @l: List where the node to be removed is stored in.
 * @n: Predecessor of the node to be removed.
 * 
 * If n is null,the first node is removed (the node after the head).
 * 
 * Returns: 0 if the node after n was removed; -1 otherwise.
 */
int list_remove_after(list *l, node *n) {
    // Remove first node.
    if (n == NULL) {
        if (l->head->next != NULL) {
            node *temp = l->head->next;
            l->head->next = l->head->next->next;
            if (temp)
                free(temp);
            return 0;
        }
        return -1;
    }

    // Remove arbitrary node.
    if (n->next == NULL) {
        fprintf(stderr, "list_remove_after(): Cannot remove node after the "
                        "list's last node\n");
        return -1;
    }
    node *old_node = n->next;
    n->next = n->next->next;
     
    free(old_node);
    return 0;
}

// TODO: The list should keep track of last element such that this
// function can be O(1)
node *list_get_last(list *l) {
    node *n = list_first(l);
    node *last = n;
    
    while (n != NULL) {
        last = n;
        n = list_next(n);
    }

    return last;
}

int list_insert_last(list *l, void *val) {
    list_insert_after(l, list_get_last(l), val);
}

/**
 * list_destroy() - Frees memory used by the list.
 * l: List structure to be freed.
 * 
 * Note that the function only frees the head node and the list structure
 * itself, data stored in the nodes must be freed manually.
 * 
 * Returns: Nothing.
 */
void list_destroy(list *l) {
    free(l->head);
    free(l);
}