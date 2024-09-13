#include "../include/dll.h"
#include <stdlib.h>
#include <string.h>

dll_t *dll_create() {
  dll_t *new = calloc(1, sizeof(dll_t));
  new->head = NULL;
  new->tail = NULL;
  new->size = 0;

  return new;
}

dll_node_t *node_create(ll_t *ll) {
  dll_node_t *new = calloc(1, sizeof(dll_node_t));
  new->ll = ll;
  new->prev = NULL;
  new->next = NULL;

  return new;
}

ll_t *get(size_t index, dll_t *dll) {
  if (index >= dll->size) {
    return NULL;
  }

  dll_node_t *current = dll->head;
  for (size_t i = 0; i < index; i++) {
    current = current->next;
  }

  return current->ll;
}

dll_node_t *insert(ll_t *ll, dll_t **dll) {
  dll_node_t *new = node_create(ll);

  new->next = NULL;
  new->prev = NULL;

  if ((*dll)->size == 0) {
    (*dll)->head = new;
    (*dll)->tail = new;
  } else {
    (*dll)->tail->next = new;
    new->prev = (*dll)->tail;
    (*dll)->tail = new;
  }

  (*dll)->size++;

  return new;
}

uint8_t insertAt(ll_t *ll, size_t index, dll_t **dll) {
  if (index == 0) {
    dll_node_t *new = node_create(ll);
    new->next = (*dll)->head;
    if ((*dll)->head != NULL) {
      (*dll)->head->prev = new;
    }
    (*dll)->head = new;
    if ((*dll)->tail == NULL) {
      (*dll)->tail = new;
    }
    (*dll)->size++;
    return 1;
  }

  dll_node_t *current = (*dll)->head;
  for (size_t i = 0; i < index; i++) {
    if (current->next == NULL) {
      return 0;
    }
    current = current->next;
  }

  dll_node_t *new = node_create(ll);
  new->next = current;
  new->prev = current->prev;
  current->prev->next = new;
  current->prev = new;

  (*dll)->size++;

  return 1;
}

uint8_t remove_dll(ll_t *ll, dll_node_t **head) {
  dll_node_t *current = *head;
  while (current != NULL) {
    if (current->ll == ll) {
      if (current->prev == NULL) {
        *head = current->next;
        (*head)->prev = NULL;
      } else if (current->next == NULL) {
        current->prev->next = NULL;
      } else {
        current->prev->next = current->next;
        current->next->prev = current->prev;
      }

      free(current->ll);
      free(current);
      return 1;
    }
    current = current->next;
  }

  return 0;
}
uint8_t removeAt(size_t index, dll_t **dll) {
  if (index >= (*dll)->size) {
    return 0;
  }

  dll_node_t *current = (*dll)->head;
  for (size_t i = 0; i < index; i++) {
    current = current->next;
  }

  if (current->prev == NULL) {
    (*dll)->head = current->next;

    //(*dll)->head->prev = NULL;

    if ((*dll)->head != NULL) {
      (*dll)->head->prev = NULL;
    }
  } else if (current->next == NULL) {
    current->prev->next = NULL;
  } else {
    current->prev->next = current->next;
    current->next->prev = current->prev;
  }

  free(current->ll);
  free(current);

  return 1;
}
uint8_t replace(ll_t *ll, size_t index, dll_node_t **head) {
  dll_node_t *current = *head;
  for (size_t i = 0; i < index; i++) {
    if (current->next == NULL) {
      return 0;
    }
    current = current->next;
  }

  current->ll = ll;

  return 1;
}

size_t position(ll_t *ll, dll_node_t **head) {
  dll_node_t *current = *head;
  size_t i = 0;
  while (current != NULL) {
    if (current->ll == ll) {
      return i;
    }
    current = current->next;
    i++;
  }

  return -1;
}

dll_node_t *findFunction(const char *name, dll_t *dll) {
  dll_node_t *current = dll->head;
  while (current != NULL) {
    if (strcmp(current->ll->head->data->token, "def") == 0) {
      if (strcmp(current->ll->head->next->data->token, name) == 0) {
        return current;
      }
    }
    current = current->next;
  }

  return NULL;
}
