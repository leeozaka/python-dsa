#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/data.h"
#include "include/dll.h"
#include "include/ll.h"
#include "include/stack.h"
#include "include/strctrl.h"

char *conv(unsigned int numero, int base) {
  static char Rep[] = "0123456789ABCDEF";
  static char buffer[50];
  char *ptr;

  ptr = &buffer[49];
  *ptr = '\0';

  do {
    *--ptr = Rep[numero % base];
    numero /= base;
  } while (numero != 0);
  return ptr;
}

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
    }
  }

  // file read loop end
  fclose(stream);

  // for (dll_node_t *node = main->head; node; node = node->next) {
  //   ll_show(node->ll);
  // }

  // memory loop start
  for (dll_node_t *node = main->head; node; node = node->next) {
    if (strcmp(node->ll->head->data->token, "print") == 0) {
    // there's a segfault when passing just a variable to the function
      if (node->ll->head->next->data->token[0] == '"') {
        ll_node_t *printnode = node->ll->head->next;
        char *printstr = node->ll->head->next->data->token;
        char *printc;
        // int i = 0;

        while (strcmp(printnode->data->token, "%") != 0) {
          if (!printnode) {
            perror("invalid print statement");
            exit(69);
          }

          printnode = printnode->next;
        }

        if (printnode->next)
          printnode = printnode->next;
        else {
          perror("invalid print statement");
          exit(69);
        }

        int *i = NULL;

        for (printc = (char *)printstr; *printc != '\0';) {
          while (*printc != '%' && *printc != '\0' && *printc != '"' &&
                 *printc != '\\') {
            putchar(*printc);
            printc++;
          }
          if (*printc == '%' || *printc == '"' || *printc == '\\')
            printc++;
          switch (*printc) {
          case 'd':
            i = bringval(printnode->data->token, mem);

            fprintf(stdout, "%d", i ? *i : atoi(printnode->data->token));
            printc++;
            printnode = printnode->next;
            break;
          case 'n':
            fprintf(stdout, "\n");
            printc++;

            // ainda nao guardo strings em variaveis
          case 's':
            break;
          }
        }
      } else {
          //i think this resolves segfault problems
          //should get the name of variable and find it in the memory
        while (strcmp(node->ll->head->next->data->token, "") != 0) {
            int *i = bringval(node->ll->head->next->data->token, mem);
            if (i) {
              fprintf(stdout, "%d ", *i);
            } else {
              fprintf(stdout, "%s", node->ll->head->next->data->token);
            }
          node->ll->head->next = node->ll->head->next->next;
        } 
      }
    

      continue;
    }

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
      if (!push(stack_data, &mem)) {
        printf("attempt to redeclare variable %s\n", stack_data.data);
      }
      continue;
    }
  }

  memshow(mem);

  return 0;
}
