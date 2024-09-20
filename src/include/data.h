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
  DOT,
  LITERAL,
  COMMENT,
  WHITESPACE,
  NEWLINE,
  PARENTHESIS,
  BRACKET,
  END,
  QUOTE
};

typedef struct TOKEN_DATA_H {
  char token[TOKENSIZE];
} token_data_t;

#endif
