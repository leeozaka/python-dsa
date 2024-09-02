
#include "../include/value.h"

value_t *new_value() {
  value_t *n = (value_t *)calloc(1, sizeof(value_t));
  n->v = new_value_context();

  return n;
}

u_value new_value_context() {
  u_value *v = (u_value *)calloc(1, sizeof(u_value));
  return *v;
}
