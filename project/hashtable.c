/* Author: Jonathan Ismail, Declan Simkins, Rebecca Walton
 * Project: Konane (Project 2)
 * Course: CMPT 355
 * Date: Mar. 28, 19
 * File: hashtable.c
 * Description: header file hashtable
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "hashtable.h"
#include "node.h"
#include "model.h"
#include "minimax.h"

// Forward declaration of model
extern model_s *model;

hashtable_s *init_hashtable() {
    hashtable_s *table = malloc(sizeof(hashtable_s));
    if (table == NULL)
        return NULL;

    table->items = malloc(DEFAULT_HASHTABLE_SIZE * sizeof(item_s));
    if (table->items == NULL)
        return NULL;

    for (int i = 0; i < DEFAULT_HASHTABLE_SIZE; i++)
        table->items[i] = NULL;

    table->size = DEFAULT_HASHTABLE_SIZE;
    table->elements = 0;

    return table;
}

item_s *init_item(node_s *node, char *act_state)
{
    item_s *new_item = malloc(sizeof(item_s));
    new_item->node = node;
    new_item->act_state = act_state;
    new_item->next = NULL;

    return new_item;
}

void update_size(hashtable_s *table)
{
    int old_size = table->size;
    table->size = (table->size + floor((table->size / 2)));
    table->items = realloc(table->items, sizeof(item_s) * table->size);
    for (int i = old_size; i < table->size; i++) {
        table->items[i] = NULL;
    }
}

// flatten from 2d to 1d
char *flatten_state(char **state)
{
    char *flat = malloc(model->size * model->size + 1);
    int temp_i = 0;
    for (int i = 0; i < model->size; i++) {
        for (int j = 0; j < model->size; j++, temp_i++) {
            flat[temp_i] = state[i][j];
        }
    }
    flat[temp_i] = '\0';
    return flat;
}

char *make_action_state_str(char **state, char *action)
{
    char *flat_state = flatten_state(state);
    char *act_state_str = malloc((strlen(action) + strlen(flat_state)) * sizeof(char) + 1);
    act_state_str = strcpy(act_state_str, action);
    act_state_str = strcat(act_state_str, flat_state);

    free(flat_state);
    return act_state_str;
}

node_s *get(hashtable_s *table, node_s *key, char *action)
{
    char *act_state = make_action_state_str(key->state, action);
    unsigned long hashIndex = hash_f(act_state) % table->size;
    
    //move in array until an empty
    item_s *current = table->items[hashIndex];
    while(current != NULL) {
        if((void *) current != (void *) table
        && strcmp(current->act_state, act_state) == 0) {
            free(act_state);
            return current->node;
        }
      
        current = current->next;
    }
    
    free(act_state);
    return NULL;
}

int insert(hashtable_s *table, node_s *key, char *action)
{
    char *act_state = make_action_state_str(key->state, action);
    unsigned long hashIndex = hash_f(act_state) % table->size;

    //move in array until an empty or deleted cell
    item_s *current = table->items[hashIndex];
    while(current != NULL && (void *) current != (void *) table) {
        if (strcmp(act_state, current->act_state) == 0) {
            free(act_state);
	        return 0;
        }

        current = current->next;
    }

    item_s *new_item = init_item(apply_action(key, action), act_state);
    new_item->next = table->items[hashIndex];
    table->items[hashIndex] = new_item;
    table->elements++;
    
    // If there's less than 10% space remaining
    if (table->size - table->elements < table->size / 10)
        update_size(table);
    return 1;
}

void free_hashtable(hashtable_s *table) {
  if (table == NULL) {
    return;
  }

  for (int i = 0; i < table->size; i++) {
    if (table->items[i] != NULL && (void *) table->items[i] != (void *) table) {
      free_item(table->items[i]);
    }
  }

  free(table->items);
  free(table);
}

void free_item(item_s *item)
{
    if (item == NULL)
        return;
    
    free_node(item->node);
    free(item->act_state);
    free_item(item->next);
    free(item);
}

// DJB2 by Dan Bernstein, http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash_f(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
