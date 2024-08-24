#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/data.h"
#include "include/dll.h"
#include "include/ll.h"

char *text_to_tokenize_1 = "    def hello(a, b):";
char *text_to_tokenize_2 = "  print(a + b)";
char *text_to_tokenize_3 = "  return a + b";

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

int classifier(char *c) {
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
  case '[':
  case ']':
  case '{':
  case '}':
  case ',':
  case ':':
  case '.':
    return PUNCTUATION;
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case '&':
  case '|':
  case '^':
  case '~':
  case '<':
  case '>':
  case '=':
  case '!':
    return OPERATOR;
  case '0' ... '9':
    return LITERAL;
  default:
    return IDENTIFIER;
  }
}

// Function to check if a string starts with exactly 4 spaces
// feito pelo gpt kkkkk
int startsWithFourSpaces(const char *str) {
  // Check if the string is at least 4 characters long
  if (strlen(str) < 4) {
    return 0; // Not enough characters
  }

  // Check the first 4 characters
  for (int i = 0; i < 4; ++i) {
    if (str[i] != ' ') {
      return 0; // Found a non-space character
    }
  }

  return 1; // The string starts with exactly 4 spaces
}

int main() {
  dll_t *main = dll_create();
  token_data_t token;
  char token_text[36] = "";

  insert(ll_create(), &main);

  // ll_insert(token, &main->head->ll);
  // ll_insert(token1, &main->head->ll);
  // ll_insert(token3, &main->head->ll);
  // ll_insert(token2, &main->head->ll);

  // init first linked list for main dll
  insert(ll_create(), &main);

  size_t i = 0, j = 0;

  // char *text_to_tokenize_1 = "    def hello(a, b):";
  while (*text_to_tokenize_1) {
    // if (strcmp(text_to_tokenize_1, "    ") == 0) {
    if (startsWithFourSpaces(text_to_tokenize_1)) {
      strcpy(token.token, "    ");
      ll_insert(token, &main->head->ll);
      text_to_tokenize_1 += 4;
      continue;
    }

    int type = classifier(text_to_tokenize_1);
    while (type != WHITESPACE && type != PUNCTUATION && type != NEWLINE) {
      token_text[j] = *text_to_tokenize_1;
      j++;
      text_to_tokenize_1++;

      type = classifier(text_to_tokenize_1);
    }

    token_text[j] = '\0';
    strcpy(token.token, token_text);

    ll_insert(token, &main->head->ll);

    strcpy(token_text, "");
    j = 0;
    text_to_tokenize_1++;
  }

  ll_show(main->head->ll);
  return 0;
}
