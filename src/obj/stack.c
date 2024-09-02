#include <stdio.h>
#include <stdlib.h>

#include "../include/stack.h"
#include <stdint.h>
#include <string.h>

stacks_t *stack_create() {
  stacks_t *stack = (stacks_t *)malloc(sizeof(stacks_t));
  stack->top = NULL;
  stack->size = 0;
  return stack;
}

stack_node_t *stack_node_create(stack_data_t data) {
  stack_node_t *node = (stack_node_t *)malloc(sizeof(stack_node_t));
  node->data = (stack_data_t *)calloc(1, sizeof(stack_data_t));
  node->data->value = new_value();

  // node->data->value = data.value;
  memcpy(node->data->value, data.value, sizeof(value_t));
  node->data->address = data.address;
  strcpy(node->data->data, data.data);

  node->next = NULL;
  return node;
}

uint8_t exists(stack_data_t data, stacks_t *stack) {
  stack_node_t *node = stack->top;
  while (node) {
    if (strcmp(node->data->data, data.data) == 0) {
      return 1;
    }
    node = node->next;
  }
  return 0;
}

value_t *bringval(const char *var, stacks_t *stack, int depth) {
  stack_node_t *node = stack->top;

  while (node) {
    if (strcmp(node->data->data, var) == 0) {
        depth++;
      return node->data->value;
    }
    // if (strcmp(node->data->data, "FCALL") == 0) {
    //   return NULL;
    // }
    node = node->next;
  }
  return NULL;
}

uint8_t push(stack_data_t data, stacks_t **stack) {
  // if (exists(data, *stack)) {
  //   return 0;
  // }

  stack_node_t *node = stack_node_create(data);
  node->next = (*stack)->top;
  (*stack)->top = node;
  (*stack)->size++;
  return 1;
}

uint8_t pop(stacks_t **stack) {
  if ((*stack)->size == 0) {
    return 0;
  }

  stack_node_t *node = (*stack)->top;
  (*stack)->top = node->next;
  free(node);
  (*stack)->size--;
  return 1;
}

uint8_t peek(stack_node_t **data, stacks_t *stack) {
  if (stack->size == 0) {
    return 0;
  }

  *data = stack->top;
  return 1;
}

uint8_t isEmpty(stacks_t *stack) { return stack->size == 0; }

size_t stack_size(stacks_t *stack) { return stack->size; }

void memshow(stacks_t *stack) {
  stack_node_t *node = stack->top;
  printf("Stack size: %zu\n", stack->size);
  printf("Data - Value - Address \n");
  while (node) {
    if (node->data->value->identity == V_INT) {
      printf("%s %d %p\n", node->data->data, node->data->value->v.i,
             node->data->address);
    } else if (node->data->value->identity == V_STRING) {
      printf("%s %s %p\n", node->data->data, node->data->value->v.str,
             node->data->address);
    }
    node = node->next;
  }
}

void clear_stack(stacks_t **stack) {
    if ((*stack)->size == 0) {
        return;
    }
    while (strcmp((*stack)->top->data->data, "FCALL") != 0) {
        pop(stack);
    }
}
