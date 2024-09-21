#ifndef GLL_H
#define GLL_H

#include "data.h"
#include "ll.h"
#include "stack.h"
#include "value.h"
#include <stdint.h>

typedef struct listagen {
  uint8_t type;
  value_t *value;
  struct listagen *head, *tail;
} gll_t;

typedef struct lg_stack {
  gll_t *l;
  struct lg_stack *next;
} gll_s_t;

typedef struct lg_queue {
  gll_t *l;
  struct lg_queue *next;
} gll_q_t;

// actually,
// would be better a union
// of stacks but i have no time
typedef struct value_stack {
  value_t *value;
  struct value_stack *next;
} value_s_t;

gll_t *criaH();
gll_t *criaT(value_t *info);

gll_s_t *init_gll_s();
gll_q_t *init_gll_q();

void push_gll_stack(gll_s_t **s, gll_t *gll);
int gll_s_isEmpty(gll_s_t *s);
gll_t *pop_gll_stack(gll_s_t **s);

void value_push(value_s_t **s, value_t *value);
void value_pop(value_s_t **s);

void enqueue(gll_q_t **q, gll_t *gll);
gll_t *dequeue(gll_q_t **q);
int gll_q_isEmpty(gll_q_t *q);

uint8_t isNull(gll_t *L);

value_t *retexpr(ll_node_t *ll, stacks_t *mem);
value_t *calcexpr(gll_t *L);

uint8_t isEqual(gll_t *L1, gll_t *L2);

#endif
