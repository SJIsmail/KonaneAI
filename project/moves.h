/* Author: Jonathan Ismail, Declan Simkins, Rebecca Walton
 * Project: Konane (Project 2)
 * Course: CMPT 355
 * Date: Mar. 25, 19
 * File: moves.h
 * Description: Header file for generating moves.
 */

#ifndef MOVES_H
#define MOVES_H

#include "node.h"

/* Generates all the moves possible from a given node */
char **generate_moves(node_s *, int *, char);

/* Prints the given moves */
void print_moves(char **, int);

/* Frees the given moves */
void free_moves(char **, int);

/* Get the opposite colour */
char get_other_colour(char current_colour);

#endif /* MOVES_H */
