#include "../include/strctrl.h"

int classifier(char *c) {
  if (strcmp(c, "**") == 0) {
    return OPERATOR;
  }
  if (strcmp(c, "//") == 0) {
    return OPERATOR;
  }
  switch (*c) {
  case ' ':
  case '\t':
    return WHITESPACE;
  case '\n':
    return NEWLINE;
  case '#':
    return COMMENT;
  case '(':
  case ')':
    return PARENTHESIS;
  case '[':
  case ']':
    return BRACKET;
  case '{':
  case '}':
  case ',':
  case ':':
  case ';':
    return PUNCTUATION;
  case '.':
    return DOT;
  case '+':
  case '-':
  case '*':
  case '/':
  // case '%':
  // case '&':
  // case '|':
  // case '^':
  // case '~':
  // case '<':
  // case '>':
  // case '=':
  case '!':
    return OPERATOR;
  case '"':
  case '\'':
    return QUOTE;
  case '0' ... '9':
    return LITERAL;
  default:
    if (isdigit(*c)) {
      return LITERAL;
    }
    if (isalpha(*c) || *c == '_') {
      return IDENTIFIER;
    }
    return IDENTIFIER;
  }
}

int startsWithFourSpaces(const char *str) {
  if (strlen(str) < 4) {
    return 0;
  }

  for (int i = 0; i < 4; ++i) {
    if (str[i] != ' ') {
      return 0;
    }
  }

  return 1;
}
