#ifndef FUNCTION_H
#define FUNCTION_H

#include "dll.h"
#include "print.h"
#include "stack.h"

void function_handler(dll_t *function, stacks_t **mem, int depth,
                      dll_node_t *f_node);

#endif
