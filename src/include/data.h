#ifndef DATA_H
#define DATA_H

#include <stdint.h>

enum {
  KEYWORD,
  IDENTIFIER,
  OPERATOR,
  PUNCTUATION,
  LITERAL,
  COMMENT,
  WHITESPACE,
  NEWLINE,
  END
};

typedef struct TOKEN_DATA_H {
  char token[32];
} token_data_t;

typedef struct STACK_DATA_H {} stack_data_t;

#endif
