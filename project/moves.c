/* Author: Jonathan Ismail, Declan Simkins, Rebecca Walton
 * Project: Konane (Project 2)
 * Course: CMPT 355
 * Date: Mar. 25, 19
 * File: moves.c
 * Description: File for genetating moves
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "moves.h"
#include "model.h"
#include "node.h"

extern model_s *model;
const char *x_val[] = { "A", "B", "C", "D", "E", "F", "G", "H" };

/* Return the number of open squares */
int num_open(node_s *node) {
  if (node == NULL || node->state == NULL) {
    return -1;
  }
  int open = 0;
  for (int i = 0; i < model->size; i++) {
    for (int j = 0; j < model->size; j++) {
      if (node->state[i][j] == 'O') {
	open++;
      }
    }
  }

  return open;
}

char get_other_colour(char current_colour) {
  char colour = 'W';
  if (colour == current_colour) {
    colour = 'B';
  }

  return colour;
}

char **generate_moves(node_s *node, int *num_moves, char current_colour) {
  char **moves;
  char *initial_moves[] = { "D5", "E4", "E5", "D4" };

  int open = num_open(node);
  if (open == 0 || open == 1) {
    // first move of the game or second move of the game
    moves = malloc(2 * sizeof(char *));
    if (moves == NULL) {
      return NULL;
    }
    
    int start = 0;
    if (current_colour == 'W') {
      start = 2;
    }

    for (int i = 0; i < 2; i++) {
      moves[i] = malloc(3 * sizeof(char*));
      if (moves[i] == NULL) {
	free_moves(moves, *num_moves);
	return NULL;
      }

      strcpy(moves[i], initial_moves[i + start]);
    }

    *num_moves = 2;
    return moves;
  }

  moves = malloc((model->size / 2 * 4) * sizeof(char *));
  if (moves == NULL) {
    return NULL;
  }
  
  int allocated = 10;
  for (int i = 0; i < model->size; i++) {
    for (int j = 0; j < model->size; j++) {
      if (node->state[i][j] == current_colour) {
	
	int pos = i - 2;

	/* Add moves until cannot continue moving upwards  */
	char colour = get_other_colour(current_colour);
	while (pos >= 0 && node->state[pos][j] == 'O' && node->state[pos + 1][j] == colour) {
	  moves[*num_moves] = malloc(6 * sizeof(char));
	  if (moves[*num_moves] == NULL) {
	    free_moves(moves, *num_moves);
	    return NULL;
	  }
    
	  sprintf(moves[*num_moves], "%s%d-%s%d", x_val[j], model->size - i, x_val[j], model->size - pos);
	  pos -= 2;
	  *num_moves += 1;
	}

	pos = i + 2;

	/* Add moves until cannot continue moving downwards  */
	while (pos < model->size && node->state[pos][j] == 'O' && node->state[pos - 1][j] == colour) {
	  moves[*num_moves] = malloc(6 * sizeof(char));
	  if (moves[*num_moves] == NULL) {
	    return NULL;
	  }
    
	  sprintf(moves[*num_moves], "%s%d-%s%d", x_val[j], model->size - i, x_val[j], model->size - pos);

	  pos += 2;
	  *num_moves += 1;
	}

	pos = j - 2;

	/* Add moves until cannot continue moving left  */
	while (pos >= 0 && node->state[i][pos] == 'O' && node->state[i][pos + 1] == colour) {
	  moves[*num_moves] = malloc(6 * sizeof(char));
	  if (moves[*num_moves] == NULL) {
	    return NULL;
	  }
	    
	  sprintf(moves[*num_moves], "%s%d-%s%d", x_val[j], model->size - i, x_val[pos], model->size - i);
	  pos -= 2;
	  *num_moves += 1;
	}

	pos = j + 2;

	/* Add moves until cannot continue moving right  */
	while (pos < model->size  && node->state[i][pos] == 'O' && node->state[i][pos - 1] == colour) {
	  moves[*num_moves] = malloc(6 * sizeof(char));
	  if (moves[*num_moves] == NULL) {
	    return NULL;
	  }
	  sprintf(moves[*num_moves], "%s%d-%s%d", x_val[j], model->size - i, x_val[pos], model->size - i);
	  pos += 2;
	  *num_moves += 1;
	}
      }

      if (*num_moves > (allocated - 8)) {
	char **temp = realloc(moves, (allocated + 10) * sizeof(node_s*));
	if (temp == NULL) {
	  free_moves(moves, *num_moves);
	  return NULL;
	}

	moves = temp;
	allocated += 10;
      }
    }
  }
  return moves;
}

void print_moves(char **moves, int num_moves) {
  fprintf(stderr, "Moves: %d\n", num_moves);
  if (num_moves > 0) {
    fprintf(stderr, "%s", moves[0]);
  }
  
  for (int i = 1; i < num_moves; i++) {
    fprintf(stderr, ", %s", moves[i]);
  }

  fprintf(stderr, "\n");
}

void free_moves(char **moves, int num_moves) {
  for (int i = 0; i < num_moves; i++) {
    free(moves[i]);
  }

  free(moves);
}
