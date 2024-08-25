#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/data.h"
#include "include/dll.h"
#include "include/ll.h"
#include "include/stack.h"
#include "include/strctrl.h"

int main() {
  int type;
  size_t index = 0;
  char token_text[TOKENSIZE] = "", *line;
  token_data_t token;

  line = (char *)calloc(STRSIZE, sizeof(char));
  dll_t *main = dll_create();

  stacks_t *mem = stack_create();
  stack_data_t stack_data;

  FILE *stream = fopen("test.py", "r");
  assert(stream);

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

      // type = classifier(line + 1);
      // if (type != NEWLINE) {
      //   line++;
      // }
    }

    // ll_show(actual->ll);
  }

  // file read loop end
  fclose(stream);

  printf("----hit00000\n");

  for (dll_node_t *node = main->head; node; node = node->next) {
    ll_show(node->ll);
  }

  // strcpy(stack_data.data, main->head->ll->head->next->data->token);
  // stack_data.value = 0;
  // stack_data.address = main->head;
  // push(stack_data, &mem);

  // strcpy(stack_data.data, "teste");
  // stack_data.value = 1;
  // stack_data.address = NULL;
  // push(stack_data, &mem);

  // memory loop start
  for (dll_node_t *node = main->head; node; node = node->next) {
    if (strcmp(node->ll->head->data->token, "def") == 0) {
      strcpy(stack_data.data, node->ll->head->next->data->token);
      stack_data.value = 0;
      stack_data.address = node;
      push(stack_data, &mem);
      continue;
    }
    if (strcmp(node->ll->head->data->token, "") == 0) {
      continue;
    }
    if (strcmp(node->ll->head->data->token, "") != 0) {
      strcpy(stack_data.data, node->ll->head->data->token);
      stack_data.value = atoi(node->ll->head->next->next->data->token);
      stack_data.address = NULL;
      push(stack_data, &mem);
    }
  }

  memshow(mem);

  return 0;
}
