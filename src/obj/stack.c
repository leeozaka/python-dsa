#include <stdlib.h>
#include <stdio.h>

#include <stdint.h>
#include "../include/stack.h"

stacks_t *stack_create() {
    stacks_t *stack = (stacks_t *)malloc(sizeof(stack_t));
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

stack_node_t* stack_node_create(stack_node_t *data) {
    stack_node_t *node = (stack_node_t *)malloc(sizeof(stack_node_t));
    node->data = data;
    node->next = NULL;
    return node;
}

uint8_t push(stack_node_t *data, stacks_t **stack) {
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

uint8_t isEmpty(stacks_t *stack) {
    return stack->size == 0;
}

uint64_t stack_size(stacks_t *stack) {
    return stack->size;
}
