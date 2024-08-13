#ifndef DLL_H_ 
#define DLL_H_ 

#include <stdint.h>
#include "data.h"
#include "ll.h"

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
    uint64_t size;
} dll_t;

dll_t *dll_create();
dll_node_t* node_create(ll_t *ll);
ll_t* get(uint64_t index, dll_t *head);
uint8_t insert(ll_t *ll, dll_t **dll);
uint8_t insertAt(ll_t *ll, uint64_t index, dll_t **dll);
uint8_t remove_dll(ll_t *ll, dll_node_t **head);
uint8_t removeAt(uint64_t index, dll_t **dll);
uint8_t replace(ll_t *ll, uint64_t index, dll_node_t **head);
uint64_t position(ll_t *ll, dll_node_t **head);

#endif 
