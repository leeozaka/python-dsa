#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/data.h"
#include "include/dll.h"
#include "include/ll.h"

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
  case ';':
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

int main() {
  int type;
  size_t index = 0;
  char token_text[36] = "", *line;
  token_data_t token;

  line = (char *)calloc(256, sizeof(char));
  dll_t *main = dll_create();

  FILE *stream = fopen("test.py", "r");

  while (fgets(line, 256, stream)) {
    dll_node_t *actual = insert(ll_create(), &main);

    while (*line) {
      if (startsWithFourSpaces(line)) {
        strcpy(token.token, "");
        ll_insert(token, &actual->ll);
        line += 4;
        continue;
      }

      if (isspace(*line)) {
        line++;
        continue;
      }

      type = classifier(line);
      while (type != WHITESPACE && type != PUNCTUATION ) {
        token_text[index++] = *line;
        line++;

        type = classifier(line);
      }

      strcpy(token.token, token_text);

      ll_insert(token, &actual->ll);

      memset(token_text, 0, sizeof(token_text));
      index = 0;

      line++;
    }

    ll_show(actual->ll);
  }

  fclose(stream);
  return 0;
}
