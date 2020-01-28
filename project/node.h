/* Author: Jonathan Ismail, Declan Simkins, Rebecca Walton
 * Project: Konane (Project 2)
 * Course: CMPT 355
 * Date: Mar. 25, 19
 * File: node.h
 * Description: Header file for node.
 */

#ifndef NODE_H
#define NODE_H
typedef struct node_s {
    char **state;
    int value;
    struct node_s *parent;
    struct node_s **children;
    char *action;
    int num_children;
} node_s;

/* Initializes node */
node_s *init_node(int, char **, node_s *);

/* Prints the given node */
void print_node(node_s *);

/* Frees the given node */
void free_node(node_s *);

/* Frees all the nodes in the list */
void free_nodes(node_s **, int);

/* Frees the entire tree starting at the given node */
void free_nodes_rec(node_s *);

/* Frees the entire tree with the exception of the given node starting at the given node */
void free_node_children_rec(node_s *);

/* Generates all the nodes for the given actions */
node_s **generate_nodes(char **, int, node_s *, char);

int node_s_cmp(const void *elem_1, const void *elem_2);

#endif /* NODE_H */
