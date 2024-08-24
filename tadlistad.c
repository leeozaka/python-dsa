#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct DATA_T_ {
    int i;
} data_t;

typedef struct D_L_LIST_NODE_ { 
    struct D_L_LIST_NODE_ *prox;
    struct D_L_LIST_NODE_ *prev;
    data_t *data; 
} llist_node_t; 

typedef struct D_L_LIST_ {
    llist_node_t *head;
    llist_node_t *tail;
    int size;
} linked_list_t;

linked_list_t* linked_list_init() {
    linked_list_t *new = (linked_list_t*)calloc(1, sizeof(linked_list_t));
    new -> head = NULL;
    new -> tail = NULL;
    new -> size = 0;

    return new;
}

llist_node_t* linked_list_node_init(data_t* data) {
    llist_node_t *new_node = (llist_node_t*)calloc(1, sizeof(llist_node_t));

    new_node -> data = data;
    new_node -> prev = NULL;
    new_node -> prox = NULL;
    return new_node;
}

int insertAtStart(linked_list_t *ll, data_t data) {
    llist_node_t *new_node = linked_list_node_init(&data);
    if (ll -> size == 0) {
        ll -> head = new_node;
        ll -> tail = new_node;
        return ll->size++;
    } 

    new_node -> prox = ll -> head;
    ll -> head -> prev = new_node;
    ll -> head = new_node;
    ll -> size++;
    return 0; 
}

int main () {
}
