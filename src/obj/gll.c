// #include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/gll.h"

gll_t *criaT(value_t *info) {
  gll_t *L = (gll_t *)malloc(sizeof(gll_t));
  L->type = 1;
  strcpy(L->no.info, info);
  return L;

}

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
