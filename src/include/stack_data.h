#ifndef STACK_DATA_H
#define STACK_DATA_H

#include <stdint.h>

typedef struct STACK_DATA_ {
    char id[32];
    int value;
    struct STACK_DATA_ *p;
} stack_data_t;

#endif
