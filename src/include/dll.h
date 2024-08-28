#ifndef DLL_H_
#define DLL_H_

#include "data.h"
#include "ll.h"
#include <stdlib.h> 

typedef struct DLL_NODE_ dll_node_t;
typedef struct DLL dll_t;

typedef struct DLL_NODE_ {
  ll_t *ll;
  struct DLL_NODE_ *prev;
  struct DLL_NODE_ *next;
} dll_node_t;

typedef struct DLL {
  dll_node_t *head;
  dll_node_t *tail;
  size_t size;
} dll_t;

dll_t *dll_create();
dll_node_t *node_create(ll_t *ll);

dll_node_t *insert(ll_t *ll, dll_t **dll);
uint8_t insertAt(ll_t *ll, size_t index, dll_t **dll);
uint8_t remove_dll(ll_t *ll, dll_node_t **head);
uint8_t removeAt(size_t index, dll_t **dll);

ll_t *get(size_t index, dll_t *head);
uint8_t replace(ll_t *ll, size_t index, dll_node_t **head);
size_t position(ll_t *ll, dll_node_t **head);

dll_node_t *findFunction(const char *name, dll_t *head);

#endif
