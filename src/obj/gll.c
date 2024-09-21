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

  printf(" debug: new %p\n", new->l);
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
// 1. we check if the first value is a number
// 2. we stack every value and operator and resolve the priority operations
// first
// 3. we go through all the list
// 4. if we have any operator left, we resolve it
// 5. we return the value of the expression
value_t *calcexpr(gll_t *L) {
  value_t *value = new_value();
  value_s_t *op = NULL;
  value_s_t *val = NULL;

  // stacking every value and operator
  // and resolving the priority operations first
  while (L) {
    // in the first pass we should check if the first node is a number
    // if it's not, it's not a valid expression
    if (value->identity == V_NULL) {
      if (L->value->identity == V_INT || L->value->identity == V_FLOAT) {
        value->identity = L->value->identity;
        value->v.i = L->value->v.i;
      } else {
        printf("Error: invalid token\n");
        exit(EXIT_FAILURE);
      }
      L = L->tail;
      continue;
    }

    // here, we check if the list isn't null,
    // we check if we have any operator
    // then we check if the operator is one to resolve during the loop
    // if it is, we resolve the operation and pop the operator and the value,
    // attaching the result to the main value
    if (L && op &&
        (*op->value->v.str == '*' || *op->value->v.str == '/' ||
         strcmp(op->value->v.str, "//") == 0 ||
         strcmp(op->value->v.str, "**") == 0)) {

      if (value->identity == V_INT || value->identity == V_FLOAT) {
        if (*op->value->v.str == '*') {
          value->v.i *= val->value->v.i;
        } else if (*op->value->v.str == '/') {
          value->v.i /= val->value->v.i;
        } else if (strcmp(op->value->v.str, "//") == 0) {
          value->v.i /= val->value->v.i;
        } else if (strcmp(op->value->v.str, "**") == 0) {
          value->v.i = pow(value->v.i, val->value->v.i);
        }
      }

      value_pop(&val);
      value_pop(&op);
      continue;
    }

    // if isn't a priority operator, we push the operator and the value to the
    // stacks
    if (L->value->identity == V_OPERATOR) {
      value_push(&op, L->value);
      L = L->tail;

      // if we get a null val, it's an invalid expr
      if (!L) {
        printf("Error: invalid expression\n");
        exit(EXIT_FAILURE);
      }

      // here we push the next value to the stack
      // if it's a number, because we need to resolve the operation
      // later
      // maybe this function shouldn't be as this
      if (L->value->identity == V_INT || L->value->identity == V_FLOAT) {
        value_push(&val, L->value);
      }
    }

    L = L->tail;
  }

  // if we have any operator left, we resolve it
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

void show_gll(gll_t *head) {
  gll_t *current = head;

  while (current) {
    if (current->type == 0) {
      printf("H ");
      show_gll(current->head);
    } else {
      if (current->value->identity == V_INT) {
        printf("%d ", current->value->v.i);
      } else if (current->value->identity == V_FLOAT) {
        printf("%f ", current->value->v.f);
      } else if (current->value->identity == V_OPERATOR) {
        printf("%s ", current->value->v.str);
      } else if (current->value->identity == V_STRING) {
      }
    }

    current = current->tail;
  }
  printf("\n");
}

value_t *retexpr(ll_node_t *list, stacks_t *mem) {
  value_t *value = NULL;
  gll_t *L = NULL, *atual;

  gll_s_t *s = NULL;
  gll_q_t *q = NULL;

  // printf("list: ");
  // ll_node_t *laux = list;
  // while (laux) {
  //   printf("%s ", laux->data->token);
  //   laux = laux->next;
  // }

  while (list) {
    // printf("token: %s\n", list->data->token);
    if (!L) {
      if (!existsvar(list->data->token, mem))
        L = atual = criaT(new_value_infer_type(list->data->token));
      else {
        L = atual = criaT(bringval(list->data->token, mem));
      }
      if (L->value->identity == V_NULL) {
        printf("Error: invalid token\n");
        exit(EXIT_FAILURE);
      }
    } else {
      if (strcmp(list->data->token, "(") == 0) {
        atual->tail = criaH();
        // push do endereço superior
        push_gll_stack(&s, atual);
        list = list->next;
        // e desce
        if (!existsvar(list->data->token, mem))
          atual->head = criaT(new_value_infer_type(list->data->token));
        else
          atual->head = criaT(bringval(list->data->token, mem));
        atual = atual->head;
      } else if (strcmp(list->data->token, ")") == 0) {
        // pop devolve o atual pro nivel superior
        atual = pop_gll_stack(&s);
      } else {
        if (!existsvar(list->data->token, mem))
          atual->tail = criaT(new_value_infer_type(list->data->token));
        else
          atual->tail = criaT(bringval(list->data->token, mem));
        atual = atual->tail;
      }
    }
    list = list->next;
  }

  push_gll_stack(&s, L);

  enqueue(&q, L);

  // gll_q_t *auxq = q;
  // printf(" debug gll_queue\n");
  // while (auxq) {
  //   printf(" %p\n", auxq->l);
  //   auxq = auxq->next;
  // }

  // empilhando níveis
  while (!gll_q_isEmpty(q)) {
    atual = dequeue(&q);

    while (atual) {
      if (atual->head) {
        push_gll_stack(&s, atual);
        enqueue(&q, atual->head);
      }
      atual = atual->tail;
    }
  }

  while (!gll_s_isEmpty(s)) {
    atual = pop_gll_stack(&s);
    if (atual != L) {
      atual->value = calcexpr(atual->head);
      if (atual->value->identity == V_NULL) {
        printf("Error: invalid token\n");
        exit(EXIT_FAILURE);
      }
      if (atual->value->identity == V_INT ||
          atual->value->identity == V_FLOAT) {
        atual->type = 1;
      }
    } else {
      value = calcexpr(atual);
    }
  }

  return value;
}
