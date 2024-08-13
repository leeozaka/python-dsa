#ifndef LL_H
#define LL_H

#include "data.h"
#include <stdint.h>

typedef struct LL_NODE_ ll_node_t;

typedef struct LL {
    ll_node_t *head;
    ll_node_t *tail;
    uint64_t size;
} ll_t;

typedef struct LL_NODE_ {
    data_t *data;
    struct LL_NODE_ *next;
} ll_node_t;

ll_t *ll_create();
ll_node_t *ll_node_create(data_t data);
data_t *ll_get(uint64_t index, ll_t *head);
uint8_t ll_insert(data_t data, ll_t **ll);
uint8_t ll_insertAt(data_t data, uint64_t index, ll_t **ll);
uint8_t ll_remove(data_t data, ll_node_t **head);
uint8_t ll_removeAt(uint64_t index, ll_t **ll);
uint8_t ll_replace(data_t data, uint64_t index, ll_node_t **head);
uint64_t ll_position(data_t data, ll_node_t **head);
#endif
