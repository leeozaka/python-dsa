#include <stdint.h>
#include "data.h"

//doubly linked list
/*
  get() – Return an element from the list at any given position.
    insert() – Insert an element at any position of the list.
    insertAt()
    remove() – Remove the first occurrence of any element from a non-empty list.
    removeAt() – Remove the element at a specified location from a non-empty list.
    replace() – Replace an element at any position by another element.
    size() – Return the number of elements in the list.
    isEmpty() – Return true if the list is empty, otherwise return false.
    isFull() – Return true if the list is full, otherwise return false.
 *
 */

#ifndef DLL_H_ 
#define DLL_H_ 

typedef struct DLL {
    data_t data; 
    uint64_t size; 
    struct DLL *prev;
    struct DLL *next;
} dll_t;

uint8_t insert(data_t data, dll_t **head);
uint8_t insertAt();
uint8_t remove();
uint8_t removeAt();
uint8_t replace();
uint64_t size();
uint8_t isEmpty();
uint8_t isFull(dll_t **head);

#endif 
