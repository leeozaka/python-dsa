#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/data.h"
#include "include/dll.h"
#include "include/ll.h"
#include "include/strctrl.h"

int main() {
  int type;
  size_t index = 0;
  char token_text[TOKENSIZE] = "", *line;
  token_data_t token;

  line = (char *)calloc(STRSIZE, sizeof(char));
  dll_t *main = dll_create();

  FILE *stream = fopen("test.py", "r");

  while (fgets(line, STRSIZE, stream)) {
    if (classifier(line) == COMMENT || classifier(line) == NEWLINE) {
      continue;
    }
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
      if (type != QUOTE)
        while (type != WHITESPACE && type != PUNCTUATION && type != NEWLINE) {
          token_text[index++] = *line;
          line++;

          type = classifier(line);
        }
      else {
        token_text[index++] = *line;
        line++;
        while (classifier(line) != QUOTE) {
          token_text[index++] = *line;
          line++;
        }
        token_text[index++] = *line;
        line++;
      }

      strcpy(token.token, token_text);

      ll_insert(token, &actual->ll);

      memset(token_text, 0, sizeof(token_text));
      index = 0;

      if (*(line + 1) == '\n') {
        break;
      }
      line++;
    }

    ll_show(actual->ll);
  }

  fclose(stream);
  return 0;
}
