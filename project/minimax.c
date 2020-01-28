/* Author: Jonathan Ismail, Declan Simkins, Rebecca Walton
 * Project: Konane (Project 2)
 * Course: CMPT 355
 * Date: Mar. 25, 19
 * File: minimax.c
 * Description: implements minimax algo
 */

#include <ctype.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "hashtable.h"
#include "minimax.h"
#include "moves.h"
#include "model.h"
#include "time.h"

#define TIMEOUT INT_MAX - 1
#define THINKINGTIME 10

extern model_s *model;
extern int verbosity;

int timer_done = 0;

/* Thread to monitor time elapsed */
void *move_timer( void *ptr ) {
    struct timespec *req_time = malloc(sizeof(struct timespec));
    req_time->tv_sec = THINKINGTIME - 1;
    req_time->tv_nsec = 500000000L;

    struct timespec *rem_time = malloc(sizeof(struct timespec));
    nanosleep(req_time, rem_time);

    timer_done = 1;
    free(req_time);
    free(rem_time);

    // src: https://tread.wordpress.com/2007/02/26/valgrind-and-pthread_create-memory-leak/
    pthread_detach(pthread_self());
    pthread_exit(NULL);
}

// Forward declarations for minimax helpers
int min_value(node_s *node, int alpha, int beta, int depth_limit);
int max_value(node_s *node, int alpha, int beta, int depth_limit);

// Returns a letter's position in the alphabet
// Used for comprehending actions, which use letters as column indices
// Does not assume ASCII or sequential letters in encoding
int letter_to_int(char letter) {
    const char * const ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *letter_ind = strchr(ALPHABET, toupper(letter));
    return letter_ind - ALPHABET;
}

/* Returns the best value for min from the current node */
int min_value(node_s *node, int alpha, int beta, int depth_limit) {
    if (verbosity >= 1) {
        fprintf(stderr, "min call %d\n", depth_limit);
    }
  
    if (timer_done) {
        return TIMEOUT;
    }

    if (depth_limit == 0) {
        return node->value;
    }
    
    int v = INT_MAX, num_moves = 0;
    char other_colour = 'W';
    if (model->colour == 'W') {
      other_colour = 'B';
    }

    // generate successors of this node (both chess moves and nodes)
    char **moves = generate_moves(node, &num_moves, other_colour);
    if (moves == NULL) {
      return TIMEOUT;
    }
    
    node_s **nodes = generate_nodes(moves, num_moves, node, model->colour);
    free_moves(moves, num_moves);
    if (nodes == NULL) {
      return TIMEOUT;
    }
  
    node->children = nodes;
    node->num_children = num_moves;

    for (int i = 0; i < num_moves; i++) {
      if (timer_done) {
	return TIMEOUT;
      }
      
      int max_val = max_value(nodes[i], alpha, beta, depth_limit - 1);
      if (max_val == TIMEOUT) {
	return TIMEOUT;
      }
      
      v = fmin(v, max_val);

      // already have better option for min
      if (v <= alpha) {
	return v;
      }

      beta = fmin(beta, v);
    }

    node->parent->value = beta;

    if (verbosity >= 1) {
      fprintf(stderr, "min alpha, beta: %d, %d\n", alpha, beta);
    }
    
    return v;
}

/* Returns the best value for min from the current node */
int max_value(node_s *node, int alpha, int beta, int depth_limit) {
  if (verbosity >= 1) {
    fprintf(stderr, "max call %d\n", depth_limit);
  }

  if (timer_done) {
    return TIMEOUT;
  }

  if (depth_limit == 0) {
    return node->value;
  }

  // generate successors of this node (both chess moves and nodes)
  int v = INT_MIN, num_moves = 0;
  char **moves = generate_moves(node, &num_moves, model->colour);
  if (moves == NULL) {
    return TIMEOUT;
  }

  node_s **nodes = generate_nodes(moves, num_moves, node, model->colour);
  free_moves(moves, num_moves);
  if (nodes == NULL) {
    return TIMEOUT;
  }
  
  node->children = nodes;
  node->num_children = num_moves;

  for (int i = 0; i < num_moves; i++) {
    if (timer_done) {
      return TIMEOUT;
    }
    
    int min_val = min_value(nodes[i], alpha, beta, depth_limit - 1);
    if (min_val == TIMEOUT) {
      return TIMEOUT;
    }
    
    v = fmax(v, min_val);

    // already seen smaller value so don't need to explore
    if (v >= beta) {
      return v;
    }

    alpha = fmax(alpha, v);

    if (verbosity >= 1) {
      fprintf(stderr, "max alpha, beta: %d, %d\n", alpha, beta);
    }
  }

  node->parent->value = alpha;
  return v;
}

/* Implementation of alpha beta search. 
   Returns:
     INT_MAX -> num moves is 1 or 0
     TIMEOUT -> timeout occurred
     best -> index of best node
 */
int alpha_beta_search(node_s *node, int depth_limit, char **move) {
        int num_moves = 0;
	char **moves = generate_moves(node, &num_moves, model->colour);
	if (moves == NULL) {
	  return TIMEOUT;
	}

	// check if last move
	if (num_moves == 1) {
	  *move = malloc(sizeof(char) * (strlen(moves[0]) + 1));
	  strcpy(*move, moves[0]);
	  free_moves(moves, num_moves);
	  return INT_MAX;
	}
	if (num_moves == 0) {
	  *move = NULL;
	  free_moves(moves, num_moves);
	  return INT_MAX;
	}

	// search
	int best = max_value(node, INT_MIN, INT_MAX, depth_limit);
	if (best == TIMEOUT) {
	  free_moves(moves, num_moves);;
	  return TIMEOUT;
	}

	if (verbosity >= 1) {
	  fprintf(stderr, "Best value: %d\n", best);
	}

	// find location of the best
	best = 0;
	for (int i = 0; i < node->num_children; i++) {
	  if (node->children[i]->value > node->children[best]->value) {
	    best = i;
	  }
	}

	if (verbosity >= 1) {
	  fprintf(stderr, "Best location: %d\n", best);
	}

	// return move in chess notation as well
	*move = malloc((strlen(node->children[best]->action) + 1) * sizeof(char));
	strcpy(*move, node->children[best]->action);
	
	free_moves(moves, num_moves);
	return best;
}

/* Call alpha beta search with increasing depths */
char *iterative_deepening_minimax(node_s *node) {
    // keep track of time
    pthread_t thread_id;
    timer_done = 0;
    pthread_create(&thread_id, NULL, move_timer, NULL);

    char *best_move = NULL, *temp = NULL;

    for (int i = 1; ; i++) {
        int ret_val = alpha_beta_search(node, i, &temp);
        if (ret_val == INT_MAX) {
	    // reached an end game state that is good for us
	    free(best_move);
            best_move = temp;
	    free_node_children_rec(node);
	    return best_move;
	} else if (ret_val == TIMEOUT) {
	  // ran out of time
	  free_node_children_rec(node);
	  return best_move;
        } else {
	  // continue searching
	  free(best_move);
	  best_move = temp;
        }

	free_node_children_rec(node);
    }

    pthread_detach(thread_id);
    return best_move;
}

/* Apply a move in chess notation to the board */
node_s *apply_action(node_s *node, char *action) {
    if (strlen(action) < 2) {
        return NULL;
    }
    node_s *result = init_node(model->size, node->state, node);
    if (result == NULL) {
      return NULL;
    }

    int start_j = letter_to_int(action[0]);
    int start_i = action[1] - '0'; // char to int

    start_i = model->size - start_i; // because board row #s are flipped
    if (action[2] == '\0' || action[2] == '\n') {
        result->state[start_i][start_j] = 'O';
        return result;
    }
 
    int end_j = letter_to_int(action[3]);
    int end_i = model->size - (action[4] - '0');
    int i_direction = 0, j_direction = 0;

    if (start_j == end_j) {
        // vertical move
        if (start_i > end_i)
            i_direction = -1; // upwards move
        else
            i_direction = 1; // downwards move
    } else {
        // horizontal move
        if (start_j > end_j)
            j_direction = -1; // left move
        else
            j_direction = 1; // right move
    }

    int remove_i = start_i, remove_j = start_j;
    while (remove_i != end_i || remove_j != end_j) {
        result->state[remove_i][remove_j] = 'O';
        remove_i += i_direction;
        remove_j += j_direction;
    }

    result->state[end_i][end_j] = node->state[start_i][start_j];
    return result;
}

void evaluate(node_s *node, char colour)
{
    static const char black = 'B';
    static const char white = 'W';
    int moves_b = available_moves(black, node);
    int moves_w = available_moves(white, node);
    
    if (moves_b == 0 && moves_w == 0) {
      node->value = INT_MAX;
    } else if (moves_b == 0 && moves_w != 0) {
      if (colour == 'B') {
	node->value = INT_MIN;
      } else {
	node->value = INT_MAX;
      } 
    } else if (moves_w == 0 && moves_b != 0) {
      if (colour == 'B') {
	node->value = INT_MAX;
      } else {
	node->value = INT_MIN;
      }
    } else {
      if (model->colour == 'B') {
	node->value = moves_b - moves_w;
      } else {
	node->value = moves_w - moves_b;
      }  
    }
}

/* Caluclates the number of moves left for the specified colour*/
int available_moves(char current_colour, node_s *node) {
    int moves = 0;

    for (int i = 0; i < model->size; i++) {
      for (int j = 0; j < model->size; j++) {
	if (node->state[i][j] == current_colour) {
	  int pos = i - 2;

	  /* Add moves until cannot continue moving upwards  */
	  char colour = get_other_colour(current_colour);
	  while (pos >= 0 && node->state[pos][j] == 'O' && node->state[pos + 1][j] == colour) {
	    pos -= 2;
	    moves++;
	  }

	  pos = i + 2;

	  /* Add moves until cannot continue moving downwards  */
	  while (pos < model->size && node->state[pos][j] == 'O' && node->state[pos - 1][j] == colour) {
	    pos += 2;
	    moves++;
	  }

	  pos = j - 2;

	  /* Add moves until cannot continue moving left  */
	  while (pos >= 0 && node->state[i][pos] == 'O' && node->state[i][pos + 1] == colour) {
	    pos -= 2;
	    moves++;
	  }

	  pos = j + 2;

	  /* Add moves until cannot continue moving right  */
	  while (pos < model->size  && node->state[i][pos] == 'O' && node->state[i][pos - 1] == colour) {
	    pos += 2;
	    moves++;
	  }
	}
      }
    }
    return moves;
}
