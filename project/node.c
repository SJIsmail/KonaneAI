/* Author: Jonathan Ismail, Declan Simkins, Rebecca Walton
 * Project: Konane (Project 2)
 * Course: CMPT 355
 * Date: Mar. 25, 19
 * File: node.c
 * Description: File manages nodes for search
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "node.h"
#include "model.h"
#include "minimax.h"

extern model_s *model;
extern int verbosity;

void free_node(node_s *node) {
  if (node == NULL) {
    return;
  }

  if (node->state != NULL) {
    for (int i = 0; i < model->size; i++) {
      free(node->state[i]);
    }
    free(node->state);
  }

  free(node->action);

  free(node);
}

void print_node(node_s *node) {
  if (node == NULL || node->state == NULL) {
    return;
  }

  fprintf(stderr, "Value: %d\n", node->value);
  fprintf(stderr, "Board: \n");
  for (int i = 0; i < model->size; i++) {
    for (int j = 0; j < model->size; j++) {
      fprintf(stderr, "%c", node->state[i][j]);
    }
    fprintf(stderr, "\n");
  }
}

node_s *init_node(int size, char **state, node_s *parent) {
  node_s *node = malloc(sizeof(node_s));
  if (node == NULL)
    return NULL;

  node->state = calloc(size, sizeof(char *));
  if (node->state == NULL) {
    free(node);
    return NULL;
  }

  for (int i = 0; i < size; i++) {
    node->state[i] = calloc(size, sizeof(char));
    if (node->state[i] == NULL) {
      free_node(node);
      return NULL;
    }
  }

  node->value = 0;
  if (state != NULL) {
    for (int i = 0; i < size; i++) {
      memcpy(node->state[i], state[i], model->size * sizeof(char));
    }
  }

  node->parent = parent;
  node->children = NULL;
  node->action = NULL;
  return node;
}

void free_nodes(node_s **moves, int num_moves) {
  for (int i = 0; i < num_moves; i++) {
    free_node(moves[i]);
    moves[i] = NULL;
  }

  free(moves);
}

void free_nodes_rec(node_s *node) {
  if (node == NULL) {
    return;
  }

  if (node->children != NULL) {
    for (int i = 0; i < node->num_children; i++) {
      free_nodes_rec(node->children[i]);
    }

    free(node->children);
    node->children = NULL;
  }

  free_node(node);
  node = NULL;
}

void free_node_children_rec(node_s *node) {
  if (node == NULL) {
    return;
  }

  if (node->children != NULL) {
    for (int i = 0; i < node->num_children; i++) {
      free_nodes_rec(node->children[i]);
    }

    free(node->children);
    node->children = NULL;
  }
}

int node_s_cmp(const void *elem_1, const void *elem_2) {
    node_s *node_1 = *(node_s **)elem_1;
    node_s *node_2 = *(node_s **)elem_2;

    if (node_1 == NULL) {
        if (node_2 == NULL) {
            return 0;
        }
        return -1;
    } else if (node_2 == NULL) {
        return 1;
    }

    return node_1->value - node_2->value;
}

node_s **generate_nodes(char **moves, int num_moves, node_s *current_node, char colour) {
  if (verbosity >= 1) {
    fprintf(stderr, "The number of nodes to be generated %d\n", num_moves);
  }
  
  node_s **nodes = malloc(sizeof(node_s *) * num_moves);
  if (nodes == NULL) {
    return NULL;
  }

  for (int i = 0; i < num_moves; i++) {
    nodes[i] = apply_action(current_node, moves[i]);
    if (nodes[i] != NULL) {
      evaluate(nodes[i], model->colour);
      nodes[i]->action = malloc(sizeof(char) *(strlen(moves[i]) + 1));
      if (nodes[i]->action == NULL) {
	return NULL;
      }
      strcpy(nodes[i]->action, moves[i]);
    }
  }

  qsort(nodes, num_moves, sizeof(node_s *), node_s_cmp);
  return nodes;
}
