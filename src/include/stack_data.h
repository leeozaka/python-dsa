#ifndef STACK_DATA_H
#define STACK_DATA_H

#include <stdint.h>

typedef struct STACK_DATA_ {
    char identificador[32];
    int valor;
    struct STACK_DATA_ *ponteiro;
} stack_data_t;

#endif
