#include "../include/dll.h"
#include <stdlib.h>


dll_node_t* node_create() {
    dll_node_t* new = calloc(1, sizeof(dll_node_t));
    new->data = calloc(1, sizeof(data_t));

    return new;
}

data_t* get(uint64_t index, dll_t *dll) {
    if (index >= dll->size) {
        return NULL;
    }

    dll_node_t *current = dll->head;
    for (uint64_t i = 0; i < index; i++) {
        current = current->next;
    }

    return current->data;
}

uint8_t insert(data_t data, dll_t **dll) {
    dll_node_t *new = node_create();
    new->data = &data;
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

    return 1;
}

uint8_t insertAt(data_t data, uint64_t index, dll_node_t **head) {
    if (index == 0) {
        dll_node_t *new = node_create();
        new->data = &data;
        new->next = *head;
        new->prev = NULL;
        (*head)->prev = new;
        *head = new;
        return 1;
    }

    dll_node_t *current = *head;
    for (uint64_t i = 0; i < index; i++) {
        if (current->next == NULL) {
            return 0;
        }
        current = current->next;
    }

    dll_node_t *new = node_create();
    new->data = &data;
    new->next = current;
    new->prev = current->prev;
    current->prev->next = new;
    current->prev = new;

    return 1;
}

uint8_t remove(data_t data, dll_node_t **head) {
    dll_node_t *current = *head;
    while (current != NULL) {
        if (current->data == &data) {
            if (current->prev == NULL) {
                *head = current->next;
                (*head)->prev = NULL;
            } else if (current->next == NULL) {
                current->prev->next = NULL;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }

            free(current->data);
            free(current);
            return 1;
        }
        current = current->next;
    }

    return 0;
}
uint8_t removeAt(uint64_t index, dll_t **dll) {
    if (index >= (*dll)->size) {
        return 0;
    }

    dll_node_t *current = (*dll)->head;
    for (uint64_t i = 0; i < index; i++) {
        current = current->next;
    }

    if (current->prev == NULL) {
        (*dll)->head = current->next;
        (*dll)->head->prev = NULL;
    } else if (current->next == NULL) {
        current->prev->next = NULL;
    } else {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }

    free(current->data);
    free(current);

    return 1;
}
uint8_t replace(data_t data, uint64_t index, dll_node_t **head) {
    dll_node_t *current = *head;
    for (uint64_t i = 0; i < index; i++) {
        if (current->next == NULL) {
            return 0;
        }
        current = current->next;
    }

    current->data = &data;

    return 1;
}

uint64_t position(data_t data, dll_node_t **head) {
    dll_node_t *current = *head;
    uint64_t i = 0;
    while (current != NULL) {
        if (current->data == &data) {
            return i;
        }
        current = current->next;
        i++;
    }

    return -1;
}
