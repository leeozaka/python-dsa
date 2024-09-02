#include "../include/function.h"
#include "../include/print.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define NO_DEPTH 0

void function_handler(dll_node_t *function, stacks_t *mem, int depth) {
  dll_t *body = dll_create();
  dll_node_t *exec = function->next;

  stacks_t *function_mem = stack_create();

  stack_data_t stack_data;
  stack_data.value = new_value();

  value_t *val = new_value();
  while (strcmp(exec->ll->head->data->token, "") == 0) {
    ll_t *ll = exec->ll;
    for (int i = depth + 1; i > 0; i--) {
      ll->head = ll->head->next;
    }
    insert(exec->ll, &body);
    exec = exec->next;
  }

  stack_node_t *stack_node = mem->top;
  int _depth = depth + 1;

  // here we are scoping the function memory
  if (strcmp(stack_node->data->data, "FCALL") == 0) {
    _depth--;
  }
  while (stack_node && _depth > 0) {
    push(*stack_node->data, &function_mem);
    stack_node = stack_node->next;

    if (strcmp(stack_node->data->data, "FCALL") == 0) {
      _depth--;
    }
  }

  // memory loop start
  for (dll_node_t *node = body->head; node; node = node->next) {
    if (strcmp(node->ll->head->data->token, "print") == 0) {
      if (*node->ll->head->next->data->token == '"') {

        ll_node_t *printnode = node->ll->head->next;
        char *printstr = node->ll->head->next->data->token;
        char *printc;
        // int i = 0;

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

        // int *i = NULL;

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

          printf("dest: %s\n", dest);

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
        dll_node_t *function = findFunction(stack_data.data, body);
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

          // strcpy(arg_data.data, arg->data->token);

          // if the argument is a variable, we need to fetch its value
          // if it is a value, we need to convert it to int

          arg_data.value = bringval(callarg->data->token, mem, NO_DEPTH);

          // arg_data = bringval(callarg->data->token, mem, 5)
          //                      ? *bringval(callarg->data->token, mem, 5)
          //                      : atoi(callarg->data->token);

          arg_data.address = NULL;
          push(arg_data, &mem);

          // we need to fetch every argument from the call
          assert(callarg = callarg->next);
        }

        // at this point we have stacked every argument
        function_handler(function, mem, NO_DEPTH + 1);
      }

      continue;
    }
  }

  memshow(function_mem);
}
