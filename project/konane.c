/* Author: Jonathan Ismail, Declan Simkins, Rebecca Walton
 * Project: Konane (Project 2)
 * Course: CMPT 355
 * Date: Mar. 25, 19
 * File: konane.c
 * Description: Main file for konane AI
 */

#include <stdio.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "model.h"
#include "parse.h"
#include "moves.h"
#include "minimax.h"

#define BOARD_SIZE 8

model_s *model;
int verbosity;

/* Print the usage message */
void usage(char *name) {
  printf("Usage: %s [OPTIONS...] filename start_colour=<B|W> \n"
	 "Konane AI.\n\n"
	 "Options\n"
         "  -v verbosity (increases with more)\n"
	 "  -h this usage message\n", basename(name));
  
}

int main(int argc, char **argv) {
  int c;
  while ((c = getopt(argc, argv, "hv")) != -1) {
    switch (c) {
    case 'h':
      usage(argv[0]);
      return 0;
    case 'v':
      verbosity++;
      break;
    default:
      usage(argv[0]);
      return 1;
    }
  }

  // check both required arguments are there
  int num_args = argc - optind; 
  if (num_args < 2) {
    if (num_args) {
      fprintf(stderr, "Missing file with board configuration or colour of player.\n");
    } else {
      fprintf(stderr, "Missing file with board configuration and colour of player.\n");
    }

    usage(argv[0]);
    return 1;
  }

  // get file to read from and colour of player
  char *filename = argv[optind];
  char *colour = argv[optind + 1];
  if (colour[0] != 'B' && colour[0] != 'W') {
    fprintf(stderr, "Invalid colour. Colour must be B or W\n");
    usage(argv[0]);
    return 1;
  }

  // create model
  node_s *parent = init_node(0, NULL, NULL);
  model = init_model(filename, colour[0], BOARD_SIZE, parent);
  evaluate(model->current_state, model->colour);
  if (model == NULL) {
    free(parent->state);
    free(parent);
    return 1;
  }
    
  // parse the file and generate representation of current game state
  int success = parse();
  if (success == 0) {
    free(parent->state);
    free(parent);
    free_model(model);
    return 1;
  }

  if (verbosity >= 1) {
    print_model(model);
  }
  
  char *move = NULL;
  char quit_key[] = "quit\n";
  size_t len = 0;

  char other_colour = 'W';
  if (model->colour == 'W') {
    other_colour = 'B';
  }

  int first = 1;
  while (1) {
    // search
    char *chosen = iterative_deepening_minimax(model->current_state);
    if (chosen == NULL) {
      printf("You win \n");
      return 0;
    }

    // update state based on action
    node_s *res = apply_action(model->current_state, chosen);
    free_node(model->current_state);
    model->current_state = NULL;
    model->current_state = res;
	  
    if (verbosity >= 1) {
      printf("Chosen move: %s\n", chosen);
      printf("Result:\n");
      print_node(model->current_state);
    } else {
      printf("%s\n", chosen);
    }

    free(chosen);

    // check if we've won
    if (first != 1) {
      int other_moves = available_moves(other_colour, model->current_state);
      int our_moves = available_moves(model->colour, model->current_state);
      if (other_moves == 0) {
	if (our_moves > 0) {
	  printf("We won\n");
	  return 0;
	} else {
	  printf("You won\n");
	  return 0;
	}
      }
    }

    if (verbosity >= 1) {
      printf("Enter a move: ");
    }

    // get other agent's move
    getline(&move, &len, stdin);
    if (strcmp(move, quit_key) == 0) {
      free(move);
      break;
    }

    // update state based on action
    res = apply_action(model->current_state, move);
    res->parent = parent;
    free_node(model->current_state);
    model->current_state = NULL;
    model->current_state = res;
    
    free(move);
    move = NULL;

    if (verbosity >= 1) {
      print_node(model->current_state);
    }

    first = 0;
  }

  free(parent->state);
  free(parent);
  free_model(model);
}
