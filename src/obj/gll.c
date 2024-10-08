#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/gll.h"
#include "../include/stack.h"

gll_s_t *init_gll_s() { return (gll_s_t *)calloc(1, sizeof(gll_s_t)); }

void push_gll_stack(gll_s_t **s, gll_t *gll) {
  gll_s_t *new = init_gll_s();

  if (!*s) {
    new->l = gll;
    *s = new;
    return;
  }

  new->l = gll;
  new->next = (*s);
  (*s) = new;
}

gll_t *pop_gll_stack(gll_s_t **s) {
  if (!*s) {
    return NULL;
  }

  gll_s_t *aux = *s;
  gll_t *gll = (*s)->l;

  *s = (*s)->next;

  free(aux);
  return gll;
}

int gll_s_isEmpty(gll_s_t *s) { return s == NULL; }

void value_push(value_s_t **s, value_t *value) {
  value_s_t *new = (value_s_t *)calloc(1, sizeof(value_s_t));
  new->value = new_value();

  new->value = memcpy(new->value, value, sizeof(value_t));

  if (!*s) {
    *s = new;
    return;
  }

  new->next = (*s);
  (*s) = new;
}

void value_pop(value_s_t **s) {
  if (!*s) {
    return;
  }

  value_s_t *aux = *s;
  *s = (*s)->next;
  free(aux);
}

gll_q_t *init_gll_q() { return (gll_q_t *)calloc(1, sizeof(gll_q_t)); }

void enqueue(gll_q_t **q, gll_t *gll) {
  gll_q_t *new = init_gll_q();
  new->l = gll;

  if (!*q) {
    (*q) = new;
    return;
  }

  gll_q_t *aux = (*q);
  while (aux->next)
    aux = aux->next;
  aux->next = new;
}

gll_t *dequeue(gll_q_t **q) {
  if (!*q) {
    return NULL;
  }

  gll_q_t *aux = *q;
  *q = (*q)->next;

  gll_t *qaux = (*aux).l;

  free(aux);
  return qaux;
}

int gll_q_isEmpty(gll_q_t *q) { return q == NULL; }

gll_t *criaH() {
  gll_t *L = (gll_t *)calloc(1, sizeof(gll_t));
  L->type = 0;
  L->value = new_value();

  return L;
}

gll_t *criaT(value_t *value) {
  gll_t *L = (gll_t *)calloc(1, sizeof(gll_t));
  L->type = 1;
  L->value = value;

  return L;
}

// how this function works:
// 0. we check if the first value is a number
// 1. we stack every value and operator and resolve the priority operations
// first
// 2. we go through all the list
// 3. if we have any operator left, we resolve it
// 4. we return the value of the expression
value_t *calcexpr(gll_t *L) {
  value_t *value = new_value();
  value_s_t *op = NULL;
  value_s_t *val = NULL;

  // stacking every value and operator
  // and resolving the priority operations first
  while (L) {
    if (L->value->identity == V_INT || L->value->identity == V_FLOAT) {
      value_push(&val, L->value);
      L = L->tail;
      continue;
    }

    if (L->value->identity == V_OPERATOR) {
      value_push(&op, L->value);
      L = L->tail;

      // here, we check if the list isn't null,
      // we check if we have any operator
      // then we check if the operator is one to resolve during the loop
      // if it is, we resolve the operation and pop the operator and the value,
      // attaching the result to the main value
      if (op && (*op->value->v.str == '*' || *op->value->v.str == '/' ||
                 strcmp(op->value->v.str, "//") == 0 ||
                 strcmp(op->value->v.str, "**") == 0)) {

        if (value->identity == V_NULL) {
          value->identity = val->value->identity;
          value->v.i = val->value->v.i;
          value_push(&val, L->value);
        } else {
          value_push(&val, L->value);
        }

        if (value->identity == V_INT || value->identity == V_FLOAT) {
          if (*op->value->v.str == '*') {
            value->v.i = val->value->v.i * value->v.i;
          } else if (*op->value->v.str == '/') {
            value->v.i = val->value->v.i / value->v.i;
          } else if (strcmp(op->value->v.str, "//") == 0) {
            value->v.i = val->value->v.i % value->v.i;
          } else if (strcmp(op->value->v.str, "**") == 0) {
            value->v.i = pow(value->v.i, val->value->v.i);
          }
        }

        value_pop(&val);
        value_pop(&op);
        L = L->tail;
      }
    }
  }

  // if we have any operator left, we resolve it
  if (value->identity == V_NULL) {
    value->identity = val->value->identity;
    value->v.i = val->value->v.i;
    value_pop(&val);
    L = L->tail;
  }

  while (op) {
    if (value->identity == V_INT) {
      if (*op->value->v.str == '+') {
        value->v.i = val->value->v.i + value->v.i;
      } else if (*op->value->v.str == '-') {
        value->v.i = val->value->v.i - value->v.i;
      }

      value_pop(&val);
      value_pop(&op);
    }
  }

  return value;
}

uint8_t isNull(gll_t *L) { return L == NULL; }

void show_gll(gll_t *head, int level) {
  gll_t *current = head;

  while (current) {

    if (current->value->identity == V_INT) {
      printf("%d", current->value->v.i);
    } else if (current->value->identity == V_FLOAT) {
      printf("%f", current->value->v.f);
    } else if (current->value->identity == V_OPERATOR) {
      printf("%s", current->value->v.str);
    } else if (current->value->identity == V_STRING) {
      printf("%s", current->value->v.str);
    }

    if (current->head) {
      printf("(");
      show_gll(current->head, level + 1);
      printf(")");
    }

    current = current->tail;
  }
}

value_t *retexpr(ll_node_t *list, stacks_t *mem) {
  value_t *value = NULL;
  gll_t *L = NULL, *actual;

  gll_s_t *s = NULL;
  gll_q_t *q = NULL;

  while (list) {
    if (!L) {
      if (!existsvar(list->data->token, mem))
        L = actual = criaT(new_value_infer_type(list->data->token));
      else {
        L = actual = criaT(bringval(list->data->token, mem));
      }
      if (L->value->identity == V_NULL) {
        printf("Error: invalid token\n");
        exit(EXIT_FAILURE);
      }
    } else {
      if (strcmp(list->data->token, "(") == 0) {
        actual->tail = criaH();
        // push do endereço superior
        push_gll_stack(&s, actual);
        list = list->next;
        // e desce
        if (!existsvar(list->data->token, mem))
          actual->head = criaT(new_value_infer_type(list->data->token));
        else {
          actual->head = criaT(bringval(list->data->token, mem));
        }
        actual = actual->head;
      } else {
        if (strcmp(list->data->token, ")") == 0) {
          // pop devolve o atual pro nivel superior
          actual = pop_gll_stack(&s);
        } else {
          if (!existsvar(list->data->token, mem)) {
            actual->tail = criaT(new_value_infer_type(list->data->token));
          } else
            actual->tail = criaT(bringval(list->data->token, mem));

          actual = actual->tail;
        }
      }
    }
    list = list->next;
  }

  // show_strict_gll(L);

  // show_gll(L, 0);
  // printf("\n");

  push_gll_stack(&s, L);
  enqueue(&q, L);

  // stacking levels
  while (!gll_q_isEmpty(q)) {
    actual = dequeue(&q);

    while (actual) {
      if (actual->head) {
        push_gll_stack(&s, actual);
        enqueue(&q, actual->head);
      }
      actual = actual->tail;
    }
  }

  // calc nested expressions
  while (!gll_s_isEmpty(s)) {
    actual = pop_gll_stack(&s);
    if (actual != L) {
      actual->tail->value = calcexpr(actual->head);
    } else {
      value = calcexpr(actual);
    }
  }

  return value;
}
