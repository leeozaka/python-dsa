#ifndef DATA_H
#define DATA_H

#include <stdint.h>

#define STRSIZE 256
#define TOKENSIZE 32

enum {
  KEYWORD,
  IDENTIFIER,
  OPERATOR,
  PUNCTUATION,
  LITERAL,
  COMMENT,
  WHITESPACE,
  NEWLINE,
  END,
  QUOTE
};

typedef struct TOKEN_DATA_H {
  char token[TOKENSIZE];
} token_data_t;

typedef struct STACK_DATA_H {} stack_data_t;

#endif
