#include "../include/ll.h"
#include "../include/strctrl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

token_data_t *ll_get(size_t index, ll_t *ll) {
  if (index >= ll->size) {
    return NULL;
  }

  ll_node_t *current = ll->head;
  for (size_t i = 0; i < index; i++) {
    current = current->next;
  }

  return current->data;
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

uint8_t find_operator(ll_node_t *head) {
  ll_node_t *current = head;
  while (current != NULL) {
    if (classifier(current->data->token) == OPERATOR) {
      return 1;
    }

    current = current->next;
  }

  return 0;
}
