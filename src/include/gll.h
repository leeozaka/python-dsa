#ifndef GLL_H
#define GLL_H

#include "value.h"

struct reg_lista {
  struct listagen *cabeca;
  struct listagen *cauda;
};

union info_lista {
  // char info[8];
  value_t *info;
  struct reg_lista lista;
};

typedef struct lgen {
  uint8_t type;
  union info_lista no;
} gll_t;

gll_t *criaT(value_t *info);
gll_t *cons(gll_t *H, gll_t *T);

uint8_t isNull(gll_t *L);
uint8_t isAtom(gll_t *L);

gll_t *head(gll_t *L);
gll_t *tail(gll_t *L);

void show(gll_t *L);
void showAtom(gll_t *L);
void list_delete(gll_t **L);
void duplicate(gll_t *L);

uint8_t isEqual(gll_t *L1, gll_t *L2);

#endif
