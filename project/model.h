/* Author: Jonathan Ismail, Declan Simkins, Rebecca Walton
 * Project: Konane (Project 2)
 * Course: CMPT 355
 * Date: Mar. 25, 19
 * File: model.h
 * Description: Header file for model
 */

#ifndef MODEL_H
#define MODEL_H

#include "stdio.h"
#include "hashtable.h"
#include "node.h"

typedef struct model_s {
    FILE *file;
    char colour;
    node_s *current_state;
    int size;
    hashtable_s *hashtable;
} model_s;

/* Initializes the model */
model_s *init_model(char *, char, int, node_s *);

/* Frees the given model */
void free_model(model_s *);

/* Prints the given model. */
void print_model(model_s *);

#endif /* MODEL_H */
