#include "../include/ll.h"
#include <stdlib.h>
#include <string.h>

ll_t *ll_create() {
    ll_t *new = calloc(1, sizeof(ll_t));
    new->head = NULL;
    new->tail = NULL;
    new->size = 0;

    return new;
}

ll_node_t *ll_node_create(data_t data) {
    ll_node_t *new = calloc(1, sizeof(ll_node_t));
    new->data = calloc(1, sizeof(data_t));
    *(new->data) = data;

    return new;
}

data_t *ll_get(uint64_t index, ll_t *ll) {
    if (index >= ll->size) {
        return NULL;
    }

    ll_node_t *current = ll->head;
    for (uint64_t i = 0; i < index; i++) {
        current = current->next;
    }

    return current->data;
}

uint8_t ll_insert(data_t data, ll_t **ll) {
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

uint64_t ll_position(data_t data, ll_node_t **head) {
    ll_node_t *current = *head;
    uint64_t i = 0;
    while (current != NULL) {
        // if (*(current->data) == data) {
        //  return i;
        //}

        if (strcmp(current->data->token, data.token) == 0) {
            return i;
        }

        current = current->next;
        i++;
    }

    return -1;

}
