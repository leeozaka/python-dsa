#ifndef GLL_H
#define GLL_H

#include "data.h"
#include "ll.h"
#include "value.h"
#include <stdint.h>

enum { LIST, ATOM };

struct reg_lista {
  struct list *head;
  struct list *tail;
};

union info_lista {
  value_t *info;
  struct reg_lista lista;
};

// typedef struct list {
//   uint8_t type;
//   union info_lista no;
// } gll_t;

union info {
  float value;
  char op[3];
  char func[20];
};

typedef struct listagen {
  uint8_t type;
  union info info;
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

gll_t *criaT(value_t *info);
gll_t *cons(gll_t *H, gll_t *T);

gll_s_t *init_gll_s();
gll_q_t *init_gll_q();

uint8_t isNull(gll_t *L);
uint8_t isAtom(gll_t *L);

gll_t *head(gll_t *L);
gll_t *tail(gll_t *L);

value_t retexpr(ll_node_t *ll);

void show(gll_t *L);
void showAtom(gll_t *L);
void list_delete(gll_t **L);
void duplicate(gll_t *L);

uint8_t isEqual(gll_t *L1, gll_t *L2);

#endif
