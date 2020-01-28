/* Author: Jonathan Ismail, Declan Simkins, Rebecca Walton
 * Project: Konane (Project 2)
 * Course: CMPT 355
 * Date: Mar. 25, 19
 * File: parse.c
 * Description: File for parsing the board from the given file
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "model.h"
#include "parse.h"

extern model_s *model;

/* Parses the line representing the gameboard from the file */
int parse_line(char *line, int line_number) {
  if (line == NULL) {
    return 0;
  }

  int chars_seen = 0;
  for (int i = 0; i < strlen(line); i++) {
    // ignore any whitespace
    if (isspace(line[i])) {
      continue;
    }

    // check if seen more chars that we should have
    chars_seen++;
    if (chars_seen > model->size) {
      return 0;
    }

    // check that only valid characters are present
    if (line[i] != 'B' && line[i] != 'W' && line[i] != 'O') {
      return 0;
    }

    // initialize model with value
    model->current_state->state[line_number][i] = line[i];
  }

  // check if the line was long enough
  if (chars_seen < model->size) {
    return 0;
  }

  return 1;
}

/* Parse the current state and set the model to contain the new state */
int parse() {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  // go to start of file
  rewind(model->file);

  // parse the board
  for (int i = 0; i < model->size; i++) {
    read = getline(&line, &len, model->file);
    if (read == -1) {
      perror("getline");
      return 0;
    }

    int correct = parse_line(line, i);
    if (correct == 0) {
      free(line);
      fprintf(stderr, "Invalid board configuration.\n");
      return 0;
    }
  }

  free(line);
  return 1;
}
