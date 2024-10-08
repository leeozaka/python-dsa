#pragma once
#include "dll.h"
#include "print.h"
#include "stack.h"

size_t if_handler(dll_t *function, stacks_t **mem, int depth,
                   dll_node_t *f_node);
