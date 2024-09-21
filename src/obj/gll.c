#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/gll.h"

gll_s_t *init_gll_s() { return (gll_s_t *)calloc(1, sizeof(gll_s_t)); }

void push(gll_s_t **s, gll_t *gll) {
  gll_s_t *new = init_gll_s();
  if (!*s) {
    new->l = gll;
    return;
  }
  new->next = (*s);
  (*s) = new;
}

gll_q_t *init_gll_q() { return (gll_q_t *)calloc(1, sizeof(gll_q_t)); }

void enqueue(gll_q_t **q, gll_t *gll) {
  gll_q_t *new = init_gll_q();
  if (!*q) {
    (*q) = new;
    new->l = gll;
    return;
  }

  gll_q_t *aux = (*q);
  while (aux->next)
    aux = aux->next;
  aux->next = new;
}

void dequeue(gll_q_t **q) {
  if (!*q) {
    return;
  }
  gll_q_t *aux = *q;
  *q = (*q)->next;
  free(aux);
}

gll_t *criaT(value_t *info) {
  gll_t *L = (gll_t *)calloc(1, sizeof(gll_t));
  L->type = 1;
  L->value = info;

  return L;
}

gll_t *CriaNo(gll_t *H, gll_t *T) {
  if (isAtom(T)) {
    printf("Cons: second argument couldn't be an atom\n");
    return NULL;
  } else {
    gll_t *L = (gll_t *)calloc(1, sizeof(gll_t));
    L->type = LIST;
    // L->no.lista.head = H;
    // L->no.lista.tail = T;
    L->head = H;
    L->tail = T;
    return L;
  }
}

uint8_t isNull(gll_t *L) { return L == NULL; }
uint8_t isAtom(gll_t *L) { return !isNull(L) && L->type == ATOM; }

// gll_t *head(gll_t *L) {
//   if (isNull(L) || isAtom(L)) {
//     printf("Head: argument must be a non-empty list\n ");
//     return NULL;
//   }
//   return L->no.lista.head;
// }

// gll_t *tail(gll_t *L) {
//   if (isNull(L) || isAtom(L)) {
//     printf("Tail: argument should be a non-empty list\n");
//     return NULL;
//   }
//   return L->no.lista.tail;
// }

// void show(gll_t *L) {
//   if (!isNull(L)) {
//     if (isAtom(L))
//       if (L->no.info->identity == V_INT)
//         printf("%d", L->no.info->v.i);
//     if (L->no.info->identity == V_STRING)
//       printf("%s", L->no.info->v.str);
//     else {
//       printf("[");
//       while (!isNull(L)) {
//         show(head(L));
//         L = tail(L);
//         if (!isNull(L))
//           printf(",");
//       }
//       printf("]");
//     }
//   }
// }

// void showAtom(gll_t *L);

// void list_delete(gll_t **L) {
//   if (!isNull(*L)) {
//     if (isAtom(*L)) {
//       free(*L);
//     } else {
//       list_delete(&(*L)->no.lista.head);
//       list_delete(&(*L)->no.lista.tail);
//       free(*L);
//     }
//   }
// }

value_t retexpr(ll_node_t *list) {
  float valor;
  gll_t *L = NULL, *atual;

  gll_s_t *s = init_gll_s();
  gll_q_t *q = init_gll_q();

  while (list) {
    if (!L) {
      L = atual = criaT(new_value_infer_type(list->data->token));
    } else {
      if (*list->data->token == 'c') {
        atual->tail = CriaNo('0', NULL);
        // push do endereço superior
        push(&s, atual);
        list = list->next;
        // e desce
        atual->head = CriaNo(list->termo);
        atual = atual->head;
      } else {
        if (list->termo[0]) {
          // pop devolve o atual pro nivel superior
          pop(&p, &atual);
        } else {
          atual->tail = CriaNo(list->termo);
          atual = atual->tail;
        }
      }
    }

    list = list->next;
  }

  push(&s, L);
  enqueue(&q, L);

  // empilhando níveis
  while (!isEmpty(f)) {
    dequeue(&q, &atual);
    while (atual) {
      if (atual->cabeca) {
        push(&p);
        enqueue(&f, atual->cabeca);
      }
      atual = atual->cauda;
    }
  }

  while (!isEmpty(P)) {
    pop(&p, &atual);
    if (atual != L) {
      atual->info.valor = calcula(atual->cabeca);
    } else {
      valor = calcula(atual);
    }
  }

  return valor;
}

// void duplicate(gll_t *L);

// uint8_t isEqual(gll_t *L1, gll_t *L2);
