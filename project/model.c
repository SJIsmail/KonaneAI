/* Author: Jonathan Ismail, Declan Simkins, Rebecca Walton
 * Project: Konane (Project 2)
 * Course: CMPT 355
 * Date: Mar. 25, 19
 * File: model.c
 * Description: File for creating and destroying model of the "world"
 */

#include <stdio.h>
#include <stdlib.h>

#include "model.h"
#include "node.h"

extern model_s *model;

/* Initialize model with the given colour and file */
model_s *init_model(char *filename, char colour, int size, node_s *parent) {
  model_s *model = malloc(sizeof(model_s));
  if (model == NULL) {
    return NULL;
  }

  FILE *file  = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Could not open file: %s\n", filename);
    return NULL;
  }

  model->file = file;
  model->colour = colour;
  model->size = size;
  model->current_state = init_node(size, NULL, parent);
  model->hashtable = init_hashtable();
  return model;
}

/* Free the given model */
void free_model(model_s *model) {
  if (model == NULL)
    return;

  /* if (model->current_state != NULL) { */
  /*   free_node(model->current_state->parent); */
  /* } */

  free_node(model->current_state);
  free_hashtable(model->hashtable);

  fclose(model->file);

  free(model);
}

/* Print the given model */
void print_model(model_s *model) {
  if (model == NULL) {
    return;
  }

  printf("Colour: %c\n", model->colour);
  printf("Board Size: %d\n", model->size);
  printf("Board:\n");

  print_node(model->current_state);
}
