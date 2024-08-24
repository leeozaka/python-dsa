#include <stdio.h>

#include "include/data.h"
#include "include/dll.h"
#include "include/ll.h"

int main() {
  dll_t *dll = dll_create();
  ll_t *ll = ll_create();

  data_t data1 = {1};
  data_t data2 = {2};

  ll_insert(data1, &ll);
  printf("%llu\n", ll_position(data2, &ll->head));

  printf("Size: %llu\n", dll->size);
  printf("Size: %llu\n", ll->size);
}
