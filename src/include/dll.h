#include <stdint.h>
#include "data.h"

//doubly linked list

#ifndef DLL_H_ 
#define DLL_H_ 

typedef struct DLL_NODE_ dll_node_t;
typedef struct DLL dll_t;

typedef struct DLL_NODE_ {
    data_t *data; 
    struct DLL_NODE_ *prev;
    struct DLL_NODE_ *next;
} dll_node_t;

typedef struct DLL {
    dll_node_t *head;
    dll_node_t *tail;
    uint64_t size;
} dll_t;

dll_node_t* node_create();
data_t* get(uint64_t index, dll_t *head);
uint8_t insert(data_t data, dll_t **dll);
uint8_t insertAt(data_t data, uint64_t index, dll_node_t **head);
uint8_t remove(data_t data, dll_node_t **head);
uint8_t removeAt(uint64_t index, dll_t **dll);
uint8_t replace(data_t data, uint64_t index, dll_node_t **head);
uint64_t position(data_t data, dll_node_t **head);

#endif 
