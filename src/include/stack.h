#ifndef STACK_H
#define STACK_H

#include "../include/data.h"
#include "stack_data.h"

typedef struct STACK_NODE_ stack_node_t;
typedef struct STACK_H_ stacks_t;

typedef struct STACK_NODE_ {
    stack_data_t *data;
    struct STACK_NODE_ *next;
} stack_node_t;

typedef struct STACK_H_ {
    stack_node_t *top;
    uint64_t size;
} stacks_t;

stacks_t *stack_create();
stack_node_t* stack_node_create(stack_node_t *data);
uint8_t push(stack_data_t *data, stacks_t **stack);
uint8_t pop(stacks_t **stack);
uint8_t peek(stack_node_t **data, stacks_t *stack);
uint8_t isEmpty(stacks_t *stack);
uint64_t stack_size(stacks_t *stack);

#endif
