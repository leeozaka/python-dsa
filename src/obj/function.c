#include "../include/function.h"
#include "../include/for.h"
#include "../include/gll.h"
#include "../include/strctrl.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define NO_DEPTH 0

int debugFunction = 0;

void function_handler(dll_t *function, stacks_t **mem, int depth,
                      dll_node_t *f_node) {
  dll_t *body;
  if (f_node != NULL) {
    body = dll_create();
  } else {
    body = function;
  }
  dll_node_t *exec = f_node;

  stack_data_t stack_data;
  stack_data.value = new_value();

  value_t *retval = new_value();
  uint8_t rebuild_flag = 0;

  if (f_node) {
    if (exec->next)
      exec = exec->next;

    while (strcmp(exec->ll->head->data->token, "") == 0) {
      ll_t *src = exec->ll;
      ll_t *ll = ll_copy(src);

      for (int i = depth; i > 0; i--) {
        ll->head = ll->head->next;
      }
      insert(ll, &body);
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
      if (depth)
        printf("  ");
      ll_show(node->ll);
    }
  }

  ll_t *original = NULL;
  // memory loop start
  for (dll_node_t *node = body->head; node; node = node->next) {
    if (strcmp(FIRST_NODE->data->token, "def") == 0) {
      continue;
    }

    if (strcmp(FIRST_NODE->data->token, "") == 0) {
      continue;
    }

    // using the full list type because operator contains the first
    // element found inside the original list
    ll_t *operator= find_operator(node->ll->head);

    if (operator) {

      original = ll_copy(node->ll);

      value_t *value = retexpr(operator->head, *mem);
      clean_operator(operator->head, value, &node->ll->head);
    }

    if (strcmp(FIRST_NODE->data->token, "for") == 0) {
      if (debugFunction)
        printf("\t[for]\n");

      size_t x = for_handler(function, mem, 1, node);
      if (debugFunction)
        printf("lines: %zu\n", x);

      for (size_t i = 0; i < x; i++) {
        if (node->next)
          node = node->next;
      }

      if (debugFunction)
        printf("\t[for end]\n");
      continue;
    }

    if (strcmp(FIRST_NODE->data->token, "print") == 0) {
      print(node, *mem);
      continue;
    }

    if (strcmp(FIRST_NODE->data->token, "return") == 0) {
      if (strcmp(SECOND_NODE->data->token, "") != 0) {
        if (debugFunction)
          memshow(*mem);
        if (f_node != NULL) {
          if (bringval(SECOND_NODE->data->token, *mem)) {
            memcpy(retval, bringval(SECOND_NODE->data->token, *mem),
                   sizeof(value_t));

            if (debugFunction) {
              if (retval->identity == V_STRING) {
                printf("returning value: %s\n", retval->v.str);
              } else {
                printf("returning value: %d\n", retval->v.i);
              }
            }
            strcpy(stack_data.value->v.str, retval->v.str);
            stack_data.value->identity = retval->identity;
          } else {
            if (classifier(SECOND_NODE->data->token) == LITERAL) {
              stack_data.value->v.i = atoi(SECOND_NODE->data->token);
              stack_data.value->identity = V_INT;
              retval = stack_data.value;
            } else {
              printf("variable %s not found\n", SECOND_NODE->data->token);
              exit(EXIT_FAILURE);
            }
          }
        } else {
          printf("return should be in a function\n");
          exit(EXIT_FAILURE);
        }
      } else {
        printf("should return value\n");
        exit(EXIT_FAILURE);
      }
      continue;
      break;
    }

    dll_node_t *function_find = NULL;
    if (THIRD_NODE) {
      strcpy(stack_data.data, THIRD_NODE->data->token);
      function_find = findFunction(stack_data.data, function);
    }

    if (debugFunction && function_find) {
      printf("function name: %s\n", function_find->ll->head->next->data->token);
      printf("function position: %p\n", function_find);
    }

    if (*SECOND_NODE->data->token == '=' && !function_find) {
      strcpy(stack_data.data, FIRST_NODE->data->token);
      if (*THIRD_NODE->data->token == '"') {
        char *str =
            (char *)calloc(1, sizeof(char) * strlen(THIRD_NODE->data->token));

        strcpy(str, THIRD_NODE->data->token);

        char *dest = (char *)calloc(1, strlen(str) + 1);

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
        if (!exists(stack_data, *mem)) {
          if (isInt(THIRD_NODE->data->token)) {
            stack_data.value->identity = V_INT;
            stack_data.value->v.i = atoi(COMPARE(THIRD_NODE));
          } else {
            stack_data.value->identity = V_FLOAT;
            stack_data.value->v.f = atof(COMPARE(THIRD_NODE));
          }
        } else {
          // avoiding memory leak
          memcpy(stack_data.value, bringval(THIRD_NODE->data->token, *mem),
                 sizeof(value_t));
        }
      }

      stack_data.address = NULL;

      // remade this part to redeclare variables,
      // the code were already in the stack.c file but it was commented
      // if (!push(stack_data, mem)) {
      //   printf("attempt to redeclare variable %s\n", stack_data.data);
      // }

      assert(push(stack_data, mem));
      // memshow(*mem);
    } else {
      if (!function_find) {
        function_find = findFunction(FIRST_NODE->data->token, function);
        if (!function_find) {
          printf("function %s not found", FIRST_NODE->data->token);
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
      ll_node_t *callarg = FIRST_NODE;
      // while (callarg &&
      //        strcmp(callarg->data->token,
      //               function_find->ll->head->next->next->data->token) != 0) {
      //   callarg = callarg->next;
      // }
      while (callarg && strcmp(callarg->data->token, "(")) {
        callarg = callarg->next;
      }
      if (callarg->next)
        callarg = callarg->next;
      else
        exit(EXIT_FAILURE);
      // printf("callarg %s\n", callarg->data->token);

      // stack every argument
      ll_node_t *arg = function_find->ll->head;

      while (arg && strcmp(arg->data->token, "(") != 0) {
        arg = arg->next;
      }
      if (arg->next)
        arg = arg->next;
      else
        exit(EXIT_SUCCESS);
      // printf("arg %s\n", arg->data->token);

      if (*callarg->data->token != ')') {
        for (; arg->next; arg = arg->next) {
          stack_data_t arg_data;

          // first we need to find the argument name
          strcpy(arg_data.data, callarg->data->token);

          // if the argument is a variable, we need to fetch its value
          if (exists(arg_data, *mem)) {
            arg_data.value = bringval(callarg->data->token, *mem);
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
          callarg = callarg->next;
        }
      }

      // at this point we have stacked every argument
      if (debugFunction)
        printf("\n\tfunction in\n\n");
      function_handler(function, mem, 1, function_find);
      if (debugFunction)
        printf("\n\tfunction returned\n\n");
    }
    if (rebuild_flag) {
      // node->original;
      node->ll = original;
      rebuild_flag = 0;
    }
  }

  if (depth)
    clear_stack(mem);

  if (!isEmpty(*mem)) {
    uint8_t flag = 1;
    if (peek(*mem)->data->value->identity == V_NULL &&
        retval->identity != V_NULL) {

      // carrying return value until here
      stack_node_t *ret = peek(*mem);

      *ret->data->value = *retval;

      dll_node_t *var_name = ret->data->address;
      if (debugFunction)
        printf("f_name: %s\n", var_name->ll->head->data->token);

      stack_node_t *mem_to_return_value =
          bring(var_name->ll->head->data->token, *mem);

      if (debugFunction && mem_to_return_value) {
        printf("mem_to_return_value: %p\n", mem_to_return_value);
        printf("mem_to_return_value: %s\n", mem_to_return_value->data->data);
      }

      if (mem_to_return_value)
        mem_to_return_value->data->value = retval;

      if (debugFunction)
        printf("bring? %s\n",
               mem_to_return_value ? mem_to_return_value->data->data : "NULL");

      if (debugFunction)
        printf("returning to %s\n",
               !findFunction(var_name->ll->head->data->token, function)
                   ? "variable"
                   : "function");

      if (!mem_to_return_value &&
          !findFunction(var_name->ll->head->data->token, function)) {
        stack_data_t chicolandia_variaveis;
        chicolandia_variaveis.value = retval;
        strcpy(chicolandia_variaveis.data,
               (peek(*mem))->data->address->ll->head->data->token);

        if (debugFunction)
          printf("chicolandia_variaveis.data: %s\n",
                 chicolandia_variaveis.data);

        chicolandia_variaveis.address = NULL;
        pop(mem);
        push(chicolandia_variaveis, mem);
        flag = 0;
      }
    }

    if (flag && depth) {
      pop(mem);
    }

    if (debugFunction) {
      printf("mem is empty? %d\n", isEmpty(*mem));
      if (*mem)
        memshow((*mem));
    }
  }
}
