/* Author: Jonathan Ismail, Declan Simkins, Rebecca Walton
 * Project: Konane (Project 2)
 * Course: CMPT 355
 * Date: Mar. 28, 19
 * File: hashtable.h
 * Description: header file hashtable
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>

#include "node.h"

#define MAXCHAR 100
#define DEFAULT_HASHTABLE_SIZE 50000

typedef struct item_s {
    node_s *node;
    struct item_s *next;
    char *act_state;
} item_s;

typedef struct hashtable_s {
    int size;
    int elements;
    item_s **items;
} hashtable_s;


hashtable_s *init_hashtable();
item_s *init_item(node_s *node, char *act_state);

void update_size(hashtable_s *table);

bool search(hashtable_s *table, node_s *key, char *action);

node_s *get(hashtable_s *table, node_s *key, char *action);

char *flatten_state(char **state);
char *make_action_state_str(char **state, char *action);

int insert(hashtable_s *table, node_s *key, char *action);

item_s *delete(hashtable_s *table, node_s *key, char *action);

void display();

void free_hashtable(hashtable_s *table);
void free_item(item_s *item);

unsigned long hash_f(char *str);

#endif
