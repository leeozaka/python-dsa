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
#include "include/value.h"

#define NO_DEPTH 0

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
  stack_data.value = new_value();

  value_t *val = new_value();

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
      if (*node->ll->head->next->data->token == '"') {

        ll_node_t *printnode = node->ll->head->next;
        char *printstr = node->ll->head->next->data->token;
        char *printc;

        uint8_t percent = 0, empty = 0;

        percent = *printnode->data->token == '%';
        empty = *printnode->data->token == '\0';

        while (printnode && !percent && !empty) {
          if (!printnode) {
            perror("invalid print statement 1");
            exit(69);
          }

          printnode = printnode->next;
          percent = *printnode->data->token == '%';
          empty = *printnode->data->token == '\0';
        }

        printnode = printnode->next;

        // printf("printstr: %s\n", printstr);
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
            val = bringval(printnode->data->token, mem, NO_DEPTH);

            if (val->identity == V_INT) {
              fprintf(stdout, "%d", val->v.i);
            } else {
              fprintf(stdout, "%d", atoi(printnode->data->token));
            }

            // fprintf(stdout, "%d", i ? *i : atoi(printnode->data->token));
            printc++;
            printnode = printnode->next;
            break;

          case 'n':
            fprintf(stdout, "\n");
            printc++;
            break;

          case 's':
            val = bringval(printnode->data->token, mem, NO_DEPTH);

            if (val->identity == V_STRING) {
              fprintf(stdout, "%s", val->v.str);
            } else {
              fprintf(stdout, "%s", printnode->data->token);
            }
            break;
          }
        }
      } else {
        // i think this resolves segfault problems
        // should get the name of variable and find it in the memory
        while (strcmp(node->ll->head->next->data->token, "") != 0) {
          val = bringval(node->ll->head->next->data->token, mem, NO_DEPTH);
          if (val) {
            fprintf(stdout, "%d ", val->v.i);
          } else {
            fprintf(stdout, "%s", node->ll->head->next->data->token);
          }
          node->ll->head->next = node->ll->head->next->next;
        }
        printf("\n");
      }
      continue;
    }

    if (strcmp(node->ll->head->data->token, "def") == 0) {
      continue;
    }
    if (strcmp(node->ll->head->data->token, "") == 0) {
      continue;
    }

    if (strcmp(node->ll->head->data->token, "") != 0) {
      strcpy(stack_data.data, node->ll->head->data->token);
      if (*node->ll->head->next->data->token == '=') {
        if (*node->ll->head->next->next->data->token == '"') {
          char *str = (char *)malloc(
              sizeof(char) * strlen(node->ll->head->next->next->data->token));

          strcpy(str, node->ll->head->next->next->data->token);

          char *dest = (char *)malloc(strlen(str) + 1);

          char *d = dest;
          while (*str != '\0') {
            if (*str != '\"') {
              *d++ = *str;
            }
            str++;
          }
          *d = '\0';

          stack_data.value->identity = V_STRING;
          strcpy(stack_data.value->v.str, dest);
        } else {
          stack_data.value->identity = V_INT;
          stack_data.value->v.i = atoi(node->ll->head->next->next->data->token);
        }

        stack_data.address = NULL;

        if (!push(stack_data, &mem)) {
          printf("attempt to redeclare variable %s\n", stack_data.data);
        }
      }

      else {
        dll_node_t *function = findFunction(stack_data.data, main);
        if (!function) {
          printf("attempt to call function %s, which does not exist\n",
                 stack_data.data);
          exit(69);
        }
        // function found: stack actual node and go to function
        // stack_data.data = '\0';
        // memset(stack_data.data, 0, sizeof(stack_data.data));
        strcpy(stack_data.data, "FCALL");
        // stack_data.value = 0;

        stack_data.value->identity = 0;

        stack_data.address = node;
        // certify everything right
        // if not right here, we gonna lose the address of the return
        assert(push(stack_data, &mem));
        // let's push the args to the stack

        ll_node_t *callarg = node->ll->head->next;

        // stack every argument
        for (ll_node_t *arg = function->ll->head->next->next;
             strcmp(arg->data->token, "") != 0; arg = arg->next) {
          stack_data_t arg_data;

          strcpy(arg_data.data, arg->data->token);

          // if the argument is a variable, we need to fetch its value
          // if it is a value, we need to convert it to int

          arg_data.value = bringval(callarg->data->token, mem, NO_DEPTH);

          arg_data.address = NULL;
          push(arg_data, &mem);

          // we need to fetch every argument from the call
          assert(callarg = callarg->next);
        }

        // memshow(mem);

        // at this point we have stacked every argument
        function_handler(function, &mem, NO_DEPTH);
        pop(&mem);
      }

      continue;
    }
  }
  memshow(mem);
  return 0;
}
