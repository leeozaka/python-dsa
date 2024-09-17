#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/gll.h"

gll_t *criaT(value_t *info) {
  gll_t *L = (gll_t *)malloc(sizeof(gll_t));
  L->type = 1;
  L->no.info = info;
  return L;
}

gll_t *cons(gll_t *H, gll_t *T) {
  if (isAtom(T)) {
    printf("Cons: second argument couldn't be an atom\n");
    return NULL;
  } else {
    gll_t *L = (gll_t *)calloc(1, sizeof(gll_t));
    L->type = LIST;
    L->no.lista.head = H;
    L->no.lista.tail = T;
    return L;
  }
}

uint8_t isNull(gll_t *L) { return L == NULL; }
uint8_t isAtom(gll_t *L) { return !isNull(L) && L->type == ATOM; }

gll_t *head(gll_t *L) {
  if (isNull(L) || isAtom(L)) {
    printf("Head: argument must be a non-empty list\n ");
    return NULL;
  }
  return L->no.lista.head;
}

gll_t *tail(gll_t *L) {
  if (isNull(L) || isAtom(L)) {
    printf("Tail: argument should be a non-empty list\n");
    return NULL;
  }
  return L->no.lista.tail;
}

void show(gll_t *L) {
  if (!isNull(L)) {
    if (isAtom(L))
      if (L->no.info->identity == V_INT)
        printf("%d", L->no.info->v.i);
    if (L->no.info->identity == V_STRING)
      printf("%s", L->no.info->v.str);
    else {
      printf("[");
      while (!isNull(L)) {
        show(head(L));
        L = tail(L);
        if (!isNull(L))
          printf(",");
      }
      printf("]");
    }
  }
}

void showAtom(gll_t *L);

void list_delete(gll_t **L) {
  if (!isNull(*L)) {
    if (isAtom(*L)) {
      free(*L);
    } else {
      list_delete(&(*L)->no.lista.head);
      list_delete(&(*L)->no.lista.tail);
      free(*L);
    }
  }
}

void duplicate(gll_t *L);

uint8_t isEqual(gll_t *L1, gll_t *L2);
