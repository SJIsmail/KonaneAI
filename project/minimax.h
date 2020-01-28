/* Author: Jonathan Ismail, Declan Simkins, Rebecca Walton
 * Project: Konane (Project 2)
 * Course: CMPT 355
 * Date: Mar. 25, 19
 * File: minimax.c
 * Description: header file for minimax
 */

#ifndef MINIMAX_H
#define MINIMAX_H

#include "node.h"

// Returns best possible action given node
int minimax_decision(node_s *node, int depth_limit, char **move);

// Call alpha beta search with increasing depths 
char *iterative_deepening_minimax(node_s *node);

// Returns result of applying action to given node
node_s *result(node_s *node, char *action, char colour);

// Apply a move in chess notation to the board
node_s *apply_action(node_s *node, char *action);

// Sets heuristic for given node and model->colour
void evaluate(node_s *node, char colour);

// Returns the number of available moves for the given colour in the given node
int available_moves(char colour, node_s *node);

#endif
