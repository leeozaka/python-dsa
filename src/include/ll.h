#pragma once
#include "data.h"
#include "value.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct LL_NODE_ ll_node_t;

typedef struct LL {
  ll_node_t *head;
  ll_node_t *tail;
  size_t size;
  size_t relline;
} ll_t;

typedef struct LL_NODE_ {
  token_data_t *data;
  struct LL_NODE_ *next;
} ll_node_t;

ll_t *ll_create(size_t line);
ll_node_t *ll_node_create(token_data_t data);

ll_t *ll_copy(ll_t *ll);

uint8_t ll_insert(token_data_t data, ll_t **ll);
uint8_t ll_insertAt(token_data_t data, size_t index, ll_t **ll);
void ll_remove(ll_t **ll);
uint8_t ll_removeAt(size_t index, ll_t **ll);

ll_node_t *ll_get(ll_node_t *head, size_t index);

uint8_t ll_replace(token_data_t data, size_t index, ll_node_t **head);
size_t ll_position(token_data_t data, ll_node_t **head);
ll_t *find_operator(ll_node_t *head);
uint8_t clean_operator(ll_node_t *op_head, value_t *value,
                       ll_node_t **dest_node);

void ll_show(ll_t *ll);
