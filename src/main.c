#include <stdio.h>

#include "include/data.h"
#include "include/dll.h"
#include "include/ll.h"


int main() {
  dll_t *dll = dll_create();
  ll_t *ll = ll_create();

  data_t data1 = {1};
  data_t data2 = {2};

  // insert(data1, &dll);
  // insert(data2, &dll);

  printf("Size: %llu\n", dll->size);
}
