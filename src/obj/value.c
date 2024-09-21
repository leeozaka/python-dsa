#include "../include/value.h"
#include <ctype.h>
#include <string.h>

value_t *new_value() {
  value_t *n = (value_t *)calloc(1, sizeof(value_t));
  n->v = new_value_context();

  return n;
}

u_value new_value_context() {
  u_value *v = (u_value *)calloc(1, sizeof(u_value));
  return *v;
}

value_t *new_value_infer_type(char * str) {
    value_t *n = new_value();

    if (*str == '"') {
        // logic to strings here
        return NULL;
    }
    else if (isInt(str)) {
        n->identity = V_INT;
        n->v.i = atoi(str);
    }

    return n;
}

uint8_t isInt(const char *str) {
  for (size_t i = 0; i < strlen(str); i++) {
    if (!isdigit(str[i])) {
      return 0;
    }
  }

  return 1;
}
