#ifndef VALUE_H
#define VALUE_H

#include <stdlib.h>
#define STR_SIZE 256

enum {
    V_NULL,
    V_INT,
    V_FLOAT,
    V_CHAR,
    V_STRING,
    V_LIST
};

typedef union uval {
  char c;
  int i;
  float f;
  void *l;
  // actually,
  // should reallocate using
  // proper way but i'm not
  // going this way this time
  char str[128];
} u_value;

typedef struct _value {
  char identity;
  // going to play with some pointer arithimetics
  u_value v;
} value_t;


value_t* new_value();
u_value new_value_context();

#endif
