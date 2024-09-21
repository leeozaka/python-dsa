#ifndef VALUE_H
#define VALUE_H

#include <ctype.h>
#include <string.h>

#include <stdint.h>
#include <stdlib.h>

#define STR_SIZE 256

enum { V_NULL, V_INT, V_FLOAT, V_CHAR, V_STRING, V_LIST, V_OPERATOR };

typedef union uval {
  char c;
  int i;
  float f;
  // void *l;
  // actually,
  // should reallocate using
  // proper way but i'm not
  // going this way this time
  char str[128];
} u_value;

typedef struct _value {
  char identity;
  u_value v;
} value_t;

value_t *new_value();
u_value new_value_context();
value_t *new_value_infer_type(char *str);
uint8_t isInt(const char *str);

#endif
