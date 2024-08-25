#ifndef LL_H
#define LL_H

#include "data.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct LL_NODE_ ll_node_t;

typedef struct LL {
  ll_node_t *head;
  ll_node_t *tail;
  size_t size;
} ll_t;

typedef struct LL_NODE_ {
  token_data_t *data;
  struct LL_NODE_ *next;
} ll_node_t;

ll_t *ll_create();
ll_node_t *ll_node_create(token_data_t data);

uint8_t ll_insert(token_data_t data, ll_t **ll);
uint8_t ll_insertAt(token_data_t data, size_t index, ll_t **ll);
uint8_t ll_remove(token_data_t data, ll_node_t **head);
uint8_t ll_removeAt(size_t index, ll_t **ll);

token_data_t *ll_get(size_t index, ll_t *head);

uint8_t ll_replace(token_data_t data, size_t index, ll_node_t **head);
size_t ll_position(token_data_t data, ll_node_t **head);

void ll_show(ll_t *ll);

#endif
