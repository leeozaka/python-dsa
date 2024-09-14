#ifndef FOR_H
#define FOR_H

#include "dll.h"
#include "print.h"
#include "stack.h"

size_t for_handler(dll_t *function, stacks_t **mem, int depth,
                   dll_node_t *f_node);

#endif
