#include "../include/ll.h"
#include "../include/strctrl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TESTING 1

ll_t *ll_create(size_t line) {
  ll_t *new = calloc(1, sizeof(ll_t));
  new->head = NULL;
  new->tail = NULL;
  new->size = 0;
  new->relline = line;

  return new;
}

ll_node_t *ll_node_create(token_data_t data) {
  ll_node_t *new = calloc(1, sizeof(ll_node_t));
  new->data = calloc(1, sizeof(token_data_t));
  *(new->data) = data;

  return new;
}

// ll_node_t *ll_get(size_t index, ll_node_t *ll) {
//   if (index >= ll->size) {
//     return NULL;
//   }
//
//   ll_node_t *current = ll->head;
//   for (size_t i = 0; i < index; i++) {
//     current = current->next;
//   }
//
//   return current->data;
// }

ll_node_t *ll_get(ll_node_t *head, size_t index) {
  if (index == 0) {
    return head;
  }

  ll_node_t *current = head;
  for (size_t i = 0; i < index; i++) {
    if (!current) {
      return NULL;
    }
    current = current->next;
  }

  return current;
}

uint8_t ll_insert(token_data_t data, ll_t **ll) {
  ll_node_t *new = ll_node_create(data);

  new->next = NULL;

  if ((*ll)->size == 0) {
    (*ll)->head = new;
    (*ll)->tail = new;
  } else {
    (*ll)->tail->next = new;
    (*ll)->tail = new;
  }

  (*ll)->size++;

  return 1;
}

size_t ll_position(token_data_t data, ll_node_t **head) {
  ll_node_t *current = *head;
  size_t i = 0;
  while (current != NULL) {
    if (strcmp(current->data->token, data.token) == 0) {
      return i;
    }

    current = current->next;
    i++;
  }

  return -1;
}

void ll_show(ll_t *ll) {
  ll_node_t *current = ll->head;
  printf("%2zu | [ ]: ", ll->relline);
  while (current != NULL) {
    printf("%s %s ", current->data->token, current->next ? "->" : " ");
    current = current->next;
  }
  printf("\n");
}

void ll_remove(ll_t **ll) {
  ll_node_t *current = (*ll)->head;
  while (current != NULL) {
    ll_node_t *next = current->next;
    free(current->data->token);
    free(current->data);
    free(current);
    current = next;
  }

  free(ll);
}

ll_t *ll_copy(ll_t *ll) {
  ll_t *new = ll_create(ll->relline);
  ll_node_t *current = ll->head;
  while (current != NULL) {
    ll_insert(*(current->data), &new);
    current = current->next;
  }

  return new;
}

ll_t *find_operator(ll_node_t *head) {
  ll_t *expression = ll_create(0);

  ll_node_t *current = head;
  int *operators = NULL;
  int positions = 0;
  int *size = calloc(1, sizeof(int));

  while (current != NULL) {
    if (classifier(current->data->token) == OPERATOR ||
        classifier(current->data->token) == PARENTHESIS) {
      operators = realloc(operators, (positions + 1) * sizeof(int));
      operators[*size] = positions;
      *size += 1;
    }
    positions++;
    current = current->next;
  }

  int i = 0;
  current = head;
  while (current) {
    if (classifier(current->data->token) == OPERATOR)
      break;
    current = current->next;
  }

  if (!current || *size == 0) {
    return NULL;
  }

  int first_operator = operators[0];
  int last_operator = operators[*size - 1];

  if (strcmp(ll_get(head, first_operator)->data->token, ")") == 0) {
    printf("Invalid expression\n");
    exit(EXIT_FAILURE);
  }

  if (!ll_get(head, first_operator + 1)) {
    printf("Invalid expression\n");
    exit(EXIT_FAILURE);
  }

  ll_insert(*(ll_get(head, first_operator - 1)->data), &expression);
  while (first_operator <= last_operator) {

    ll_insert(*(ll_get(head, first_operator)->data), &expression);
    first_operator++;
  }

  ll_node_t *current2 = ll_get(head, first_operator);
  if (current2 && strcmp(ll_get(head, first_operator)->data->token, ")") != 0) {
    ll_insert(*(ll_get(head, first_operator)->data), &expression);
  }

  expression->relline = (operators[0]) - 1;

  free(operators);
  free(size);

  return expression;
}
