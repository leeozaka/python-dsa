#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/data.h"
#include "include/dll.h"
#include "include/function.h"
#include "include/ll.h"
#include "include/stack.h"
#include "include/strctrl.h"

#define NO_DEPTH 0

int debug = 1;

int main() {
  int type;
  size_t index = 0, line_number = 0;
  char token_text[TOKENSIZE] = "", *line, rep[TOKENSIZE][300];
  token_data_t token;

  line = (char *)calloc(STRSIZE, sizeof(char));
  dll_t *main = dll_create();

  stacks_t *mem = stack_create();

  FILE *stream = fopen("test.py", "r");
  assert(stream);

  while (fgets(line, STRSIZE, stream)) {
    strcpy(rep[line_number], line);
    line_number++;
    if (classifier(line) == COMMENT || classifier(line) == NEWLINE) {
      continue;
    }
    dll_node_t *actual = insert(ll_create(), &main);
    actual->relline = line_number;

    while (*line) {
      if (startsWithFourSpaces(line)) {
        strcpy(token.token, "");
        ll_insert(token, &actual->ll);
        line += 4;
        continue;
      }

      type = classifier(line);
      if (type == WHITESPACE || type == PUNCTUATION) {
        line++;
        continue;
      }

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

      if (*(line + 1) != '\n') {
        line++;
      }
    }
  }

  // file read loop end
  fclose(stream);

  if (debug)
    for (size_t i = 0; i < line_number; i++) {
      printf("%zu | %s", i + 1, rep[i]);
    }

  if (debug) {
    for (dll_node_t *node = main->head; node; node = node->next) {
      printf("line: %zu - ", node->relline);
      ll_show(node->ll);
    }

    printf("\n execution stage \n\n");
  }

  function_handler(main, &mem, NO_DEPTH, NULL);

  if (debug) {
    printf("\n\n memory dump \n\n");
    mem ? memshow(mem) : 0;

    printf("\t end \n");
  }
  return 0;
}
