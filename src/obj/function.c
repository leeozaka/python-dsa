#include "../include/function.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define NO_DEPTH 0

int debugFunction = 0;

void function_handler(dll_t *function, stacks_t **mem, int depth,
                      dll_node_t *f_node) {
  dll_t *body;
  if (f_node != NULL)
    body = dll_create();
  else {
    body = function;
  }
  dll_node_t *exec = f_node;

  stack_data_t stack_data;
  stack_data.value = new_value();

  value_t *val = new_value();
  value_t *retval = new_value();

  if (f_node) {
    if (exec->next)
      exec = exec->next;

    while (strcmp(exec->ll->head->data->token, "") == 0) {
      ll_t *ll = exec->ll;
      for (int i = depth; i > 0; i--) {
        ll->head = ll->head->next;
      }
      insert(exec->ll, &body);
      exec = exec->next;
    }
  } else {
    exec = function->head;
  }

  if (debugFunction)
    printf("depth: %d\n", depth);

  if (debugFunction) {
    printf("function body:\n");
    for (dll_node_t *node = body->head; node; node = node->next) {
      for (int i = 0; i < depth; i++) {
        printf("  ");
      }
      ll_show(node->ll);
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

        if (debugFunction)
          printf("printnode: %s\n", printnode->data->token);

        for (printc = (char *)printstr; *printc != '\0';) {
          while (*printc != '%' && *printc != '\0' && *printc != '"' &&
                 *printc != '\\') {
            putchar(*printc);
            printc++;
          }
          if (*printc == '%' || *printc == '\\') {
            printc++;
            switch (*printc) {
            case 'd':
              val = bringval(printnode->data->token, *mem, depth);

              if (val->identity == V_INT) {
                fprintf(stdout, "%d", val->v.i);
              } else {
                fprintf(stdout, "%d", atoi(printnode->data->token));
              }

              printc++;
              printnode = printnode->next;
              break;
            case 'n':
              fprintf(stdout, "\n");
              printc++;

              break;

            case 's':
              val = bringval(printnode->data->token, *mem, depth);
              if (debugFunction)
                printf("val == string? %d\n", val->identity == V_STRING);

              if (val) {
                switch (val->identity) {
                case V_INT:
                  fprintf(stdout, "%d", val->v.i);
                  break;
                case V_STRING:
                  fprintf(stdout, "%s", val->v.str);
                  break;
                }
              }

              printc++;
              printnode = printnode->next;
              break;
            }
          } else {
            printc++;
          }
        }
      } else {
        while (strcmp(node->ll->head->next->data->token, "") != 0) {
          val = bringval(node->ll->head->next->data->token, *mem, depth);
          if (val) {
            switch (val->identity) {
            case V_INT:
              fprintf(stdout, "%d ", val->v.i);
              break;
            case V_STRING:
              fprintf(stdout, "%s", val->v.str);
              break;
            }
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

    if (strcmp(node->ll->head->data->token, "return") == 0) {
      if (strcmp(node->ll->head->next->data->token, "") != 0) {
        if (debugFunction)
          memshow(*mem);
        if (f_node != NULL) {
          if (debugFunction)
            printf("returning value: %s\n", node->ll->head->next->data->token);
          retval = bringval(node->ll->head->next->data->token, *mem, depth);
          if (retval) {
            if (debugFunction)
              printf("returning value: %d\n", retval->v.i);
            stack_data.value = retval;
            stack_data.value->identity = retval->identity;
          }
        } else {
          printf("return should be in a function\n");
        }
      } else {
        printf("should return value\n");
      }
      break;
    }

    dll_node_t *function_find = NULL;
    if (node->ll->head->next->next) {
      strcpy(stack_data.data, node->ll->head->next->next->data->token);
      function_find = findFunction(stack_data.data, body);
    }

    if (*node->ll->head->next->data->token == '=' && !function_find) {
      strcpy(stack_data.data, node->ll->head->data->token);
      if (*node->ll->head->next->next->data->token == '"') {
        char *str = (char *)malloc(
            sizeof(char) * strlen(node->ll->head->next->next->data->token));

        strcpy(str, node->ll->head->next->next->data->token);

        char *dest = (char *)malloc(strlen(str) + 1);

        // gambiarration to remove quotes
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

      if (!push(stack_data, mem)) {
        printf("attempt to redeclare variable %s\n", stack_data.data);
      }
    } else {
      // dll_node_t *function_find = findFunction(stack_data.data, body);
      // printf("function_find: %p\n", function_find);

      if (!function_find) {
          function_find = findFunction(node->ll->head->data->token, body);
          if (!function_find) {
            perror("function not found");
            exit(69);
          }
      }

      // function found: stack actual node and go to function
      strcpy(stack_data.data, "FCALL");
      stack_data.value->identity = V_NULL;
      stack_data.value->v.i = 0;

      stack_data.address = node;
      // certify everything right
      // if not right here, we gonna lose the address of the return
      assert(push(stack_data, mem));

      // let's push the args to the stack
      ll_node_t *callarg = node->ll->head;
      while (strcmp(callarg->data->token,
                    function_find->ll->head->next->data->token) != 0) {
        callarg = callarg->next;
      }
      callarg = callarg->next;

      // stack every argument
      ll_node_t *arg = function_find->ll->head;
      while (strcmp(arg->data->token,
                    function_find->ll->head->next->data->token) != 0) {
        arg = arg->next;
      }
      arg = arg->next;

      for (; strcmp(arg->data->token, "") != 0; arg = arg->next) {
        stack_data_t arg_data;

        // first we need to find the argument name
        strcpy(arg_data.data, callarg->data->token);

        // if the argument is a variable, we need to fetch its value
        if (exists(arg_data, *mem)) {
          arg_data.value = bringval(callarg->data->token, *mem, depth);
        } else {
          // if not, we need to fetch the value from the call
          if (*callarg->data->token == '"') {
            arg_data.value->identity = V_STRING;
            strcpy(arg_data.value->v.str, callarg->data->token);
          } else {
            arg_data.value->identity = V_INT;
            arg_data.value->v.i = atoi(callarg->data->token);
          }
        }

        // then we need to stack the argument
        strcpy(arg_data.data, arg->data->token);
        arg_data.address = NULL;
        push(arg_data, mem);

        // we need to fetch every argument from the call
        assert(callarg = callarg->next);
      }

      // at this point we have stacked every argument
      function_handler(body, mem, depth + 1, function_find);
    }
  }

  clear_stack(mem);
  if (peek(*mem)->data->value->identity == V_NULL) {
    // carrying return value until here
    // next step is to implement the return statement

    if (f_node) {
      stack_node_t *ret = peek(*mem);
      if (debugFunction) {
        printf("returning value: %d\n", ret->data->value->v.i);
      }

      *ret->data->value = *retval;
      free(retval);

      dll_node_t *var_name = ret->data->address;
      if (debugFunction)
        printf("f_name: %s\n", var_name->ll->head->data->token);

      stack_node_t *mem_to_return_value =
          bring(var_name->ll->head->data->token, *mem);
      if (debugFunction)
        printf("mem_to_return_value: %s\n", mem_to_return_value->data->data);
      mem_to_return_value->data->value = ret->data->value;

      pop(mem);
      if (debugFunction)
        memshow(*mem);
    }
  }
}
