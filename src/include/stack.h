#ifndef STACK_H
#define STACK_H

#include "data.h"
#include "dll.h"
#include "value.h"

typedef struct STACK_DATA_H {
  char data[TOKENSIZE];
  // int value;
  value_t *value;
  dll_node_t *address;
} stack_data_t;

typedef struct STACK_NODE_ stack_node_t;
typedef struct STACK_H_ stacks_t;

typedef struct STACK_NODE_ {
  stack_data_t *data;
  struct STACK_NODE_ *next;
} stack_node_t;

typedef struct STACK_H_ {
  stack_node_t *top;
  size_t size;
} stacks_t;

stacks_t *stack_create();
stack_node_t *stack_node_create(stack_data_t data);

uint8_t push(stack_data_t data, stacks_t **stack);
stack_node_t *pop(stacks_t **stack);

void replace_mem(stack_data_t data, stacks_t *stack);
stack_node_t *bring(const char *var, stacks_t *stack);
stack_node_t *peek(stacks_t *stack);
uint8_t isEmpty(stacks_t *stack);
size_t stack_size(stacks_t *stack);

value_t *bringval(const char *var, stacks_t *stack);
void memshow(stacks_t *stack);
uint8_t exists(stack_data_t data, stacks_t *stack);
uint8_t existsvar(const char *var, stacks_t *stack);

void clear_stack(stacks_t **stack);

#endif
