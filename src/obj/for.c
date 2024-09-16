#include "../include/for.h"
#include "../include/function.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define NO_DEPTH 0

#define ACTUAL_MEM_VALUE bring(for_info.data, *mem)->data->value->v.i

int debugFor = 0;

// for handler going to be a minified version of
// the function handler, and it's going to be controlled
// with the arguments that were passed by the function call itself
size_t for_handler(dll_t *function, stacks_t **mem, int depth,
                   dll_node_t *f_node) {
  dll_t *forBody;
  if (!f_node) {
    printf("should have for node\n");
    return -1;
  }

  size_t line_count = 0;
  int for_range = 0;

  forBody = dll_create();
  dll_node_t *exec = f_node;

  stack_data_t stack_data;
  stack_data_t for_info;

  stack_data.value = new_value();
  for_info.value = new_value();
  for_info.value->identity = V_INT;

  // value_t *retval = new_value();

  // need to check if the variable is already declared
  strcpy(for_info.data, exec->ll->head->next->data->token);

  if (exists(for_info, *mem)) {
    assert(push(for_info, mem));
  }

  if (strcmp(exec->ll->head->next->next->next->data->token, "range") == 0) {
    strcpy(stack_data.data,
           exec->ll->head->next->next->next->next->data->token);
    if (!exists(stack_data, *mem)) {
      for_range = atoi(exec->ll->head->next->next->next->next->data->token);
      assert(exec->ll->head->next->next->next);
    } else {
      for_range =
          bringval(exec->ll->head->next->next->next->next->data->token, *mem)
              ->v.i;
    }
    for_info.value->v.i = 0;
  } else {
    if (debugFor)
      printf("[debug] for range w/o range: %s\n",
             exec->ll->head->next->next->next->data->token);

    strcpy(stack_data.data, exec->ll->head->next->next->next->data->token);
    if (exists(stack_data, *mem)) {
      for_range =
          bringval(exec->ll->head->next->next->next->data->token, *mem)->v.i;
    } else {
      printf("range should be a number\n");
      exit(EXIT_FAILURE);
    }
  }
  if (for_range <= 0) {
    printf("range should be greater than 0\n");
    exit(EXIT_FAILURE);
  }
  for_info.address = NULL;
  if (debugFor)
    printf("[debug] pushing %s to the stack\n", for_info.data);
  assert(push(for_info, mem));

  if (!exec->next) {
    printf("for loop without body\n");
    return -1;
  }

  if (debugFor) {
    printf("[debug] variable: %s\n", for_info.data);
    printf("[debug] value: %d\n", for_info.value->v.i);
    printf("[debug] range: %d\n", for_range);
  }

  // while (strcmp(exec->ll->head->data->token, "") == 0 && exec->next) {
  //   ll_t *ll = exec->ll;
  //   if (!depth)
  //     for (int i = depth; i > 0; i--) {
  //       ll->head = ll->head->next;
  //     }
  //   insert(exec->ll, &forBody);
  //   exec = exec->next;
  // }

  if (f_node) {
    if (exec->next) {
      // ll_show(exec->ll);
      // ll_show(exec->next->ll);
      exec = exec->next;
    }

    // inverted conditions here to avoid segmentation fault
    // as we need to ensure that exec exists before we try to access it
    while (exec && strcmp(exec->ll->head->data->token, "") == 0) {
      ll_t *src = exec->ll;
      ll_t *dest = ll_copy(src);

      for (int i = depth; i > 0; i--) {
        dest->head = dest->head->next;
      }

      line_count++;
      insert(dest, &forBody);
      exec = exec->next;
    }
  }

  if (debugFor) {
    printf("[debug] for body:\n");
    for (dll_node_t *node = forBody->head; node; node = node->next) {
      for (int i = 0; i < depth; i++) {
        printf("  ");
      }
      ll_show(node->ll);
    }
  }

  while (ACTUAL_MEM_VALUE < for_range) {
    for (dll_node_t *node = forBody->head; node; node = node->next) {
      if (strcmp(node->ll->head->data->token, "for") == 0) {
        if (debugFor) {
          printf("\t[debug - for]\n");
        }

        size_t x = for_handler(function, mem, 1, node);
        if (debugFor)
          printf("[debug] lines: %zu\n", x);

        for (size_t i = 0; i < x; i++) {
          if (node->next)
            node = node->next;
        }

        if (debugFor)
          printf("\t[for end]\n");
        continue;
      }

      if (strcmp(node->ll->head->data->token, "print") == 0) {
        print(node, *mem);
        continue;
      }

      if (strcmp(node->ll->head->data->token, "def") == 0) {
        continue;
      }

      if (strcmp(node->ll->head->data->token, "") == 0) {
        continue;
      }

      if (strcmp(node->ll->head->data->token, "return") == 0) {
        // if (strcmp(node->ll->head->next->data->token, "") != 0) {
        //   if (debugFor)
        //     memshow(*mem);
        //   if (f_node != NULL) {
        //     retval = bringval(node->ll->head->next->data->token, *mem);
        //     if (retval) {
        //       if (debugFor) {
        //         if (retval->identity == V_STRING) {
        //           printf("returning value: %s\n", retval->v.str);
        //         } else {
        //           printf("returning value: %d\n", retval->v.i);
        //         }
        //       }
        //       stack_data.value = retval;
        //       stack_data.value->identity = retval->identity;
        //     }
        //   } else {
        //     printf("return should be in a function\n");
        //   }
        // } else {
        //   printf("should return value\n");
        // }
        // continue;

        printf("return not allowed in a for loop yet\n");
        exit(EXIT_FAILURE);
        break;
      }

      dll_node_t *function_find = NULL;
      if (node->ll->head->next->next) {
        strcpy(stack_data.data, node->ll->head->next->next->data->token);
        function_find = findFunction(stack_data.data, function);
      }

      if (debugFor && function_find) {
        printf("[debug] function name: %s\n",
               function_find->ll->head->next->data->token);
        printf("[debug] function position: %p\n", function_find);
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
          if (!exists(stack_data, *mem)) {
            stack_data.value->identity = V_INT;
            stack_data.value->v.i =
                atoi(node->ll->head->next->next->data->token);
          } else {
            stack_data.value =
                bringval(node->ll->head->next->next->data->token, *mem);
          }
        }

        stack_data.address = NULL;
        assert(push(stack_data, mem));
      } else {
        if (!function_find) {
          function_find = findFunction(node->ll->head->data->token, function);
          if (!function_find) {
            printf("function %s not found", node->ll->head->data->token);
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
          assert(callarg = callarg->next);
        }

        // at this point we have stacked every argument
        if (debugFor)
          printf("\n[debug] \tfunction in\n\n");
        function_handler(function, mem, 1, function_find);
        if (debugFor)
          printf("\n[debug] \tfunction returned\n\n");
      }
    }
    for_info.value->v.i++;
    replace_mem(for_info, *mem);
  }
  if (strcmp(peek(*mem)->data->data, for_info.data) == 0) {
    if (debugFor) {
      printf("memory that should be popped: %s\n", for_info.data);
      printf("memory that is on top: %s\n", peek(*mem)->data->data);
    }
    pop(mem);
  } else {
    do {
      pop(mem);
    } while (strcmp(peek(*mem)->data->data, for_info.data) != 0);
  }
  if (debugFor)
    printf("[debug] line count: %zu\n", line_count);
  return line_count;
}
