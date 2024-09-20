#include "../include/for.h"
#include "../include/function.h"
#include "../include/strctrl.h"
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
    // int size = 0;
    // if (find_operator(node->ll->head, &size)) {
    //   printf("operator position list: ");
    //
    //   int *list = find_operator(node->ll->head, &size);
    //   int *list2 = list;
    //   printf("size: %d\n", size);
    //
    //   int iterator = 0;
    //   ll_node_t *current = node->ll->head;
    //
    //   while (current != NULL) {
    //     printf("%s%s%s ", current->data->token,
    //            iterator == *list2 ? "[operator] " : "",
    //            current->next ? "->" : "");
    //     current = current->next;
    //
    //     if (iterator == *list2) {
    //       list2++;
    //     }
    //
    //     iterator++;
    //   }
    //
    //   printf("\n");
    //   continue;
    // }

    if (strcmp(FIRST_NODE->data->token, "for") == 0) {
      if (debugFor)
        printf("\t[for]\n");

      size_t x = for_handler(function, mem, 1, node);
      if (debugFor)
        printf("lines: %zu\n", x);

      for (size_t i = 0; i < x; i++) {
        if (node->next)
          node = node->next;
      }

      if (debugFor)
        printf("\t[for end]\n");
      // printf("for unavailable\n");
      // exit(EXIT_FAILURE);
      continue;
    }

    if (strcmp(FIRST_NODE->data->token, "print") == 0) {
      print(node, *mem);
      continue;
    }

    if (strcmp(FIRST_NODE->data->token, "def") == 0) {
      printf("def");
      continue;
    }

    if (strcmp(FIRST_NODE->data->token, "") == 0) {
      printf("empty");
      continue;
    }

    if (strcmp(FIRST_NODE->data->token, "return") == 0) {
      // printf("return");
      // if (strcmp(SECOND_NODE->data->token, "") != 0) {
      //   if (debugFunction)
      //     memshow(*mem);
      //   if (f_node != NULL) {
      //     if (bringval(SECOND_NODE->data->token, *mem)) {
      //       memcpy(retval, bringval(SECOND_NODE->data->token, *mem),
      //              sizeof(value_t));
      //
      //       if (debugFunction) {
      //         if (retval->identity == V_STRING) {
      //           printf("returning value: %s\n", retval->v.str);
      //         } else {
      //           printf("returning value: %d\n", retval->v.i);
      //         }
      //       }
      //       strcpy(stack_data.value->v.str, retval->v.str);
      //       stack_data.value->identity = retval->identity;
      //     } else {
      //       if (classifier(SECOND_NODE->data->token) == LITERAL) {
      //         stack_data.value->v.i = atoi(SECOND_NODE->data->token);
      //         stack_data.value->identity = V_INT;
      //         retval = stack_data.value;
      //       } else {
      //         printf("variable %s not found\n", SECOND_NODE->data->token);
      //         exit(EXIT_FAILURE);
      //       }
      //     }
      //   } else {
      //     printf("return should be in a function\n");
      //     exit(EXIT_FAILURE);
      //   }
      // } else {
      //   printf("should return value\n");
      //   exit(EXIT_FAILURE);
      // }
      // continue;
      break;
    }

    dll_node_t *function_find = NULL;
    if (THIRD_NODE) {
      strcpy(stack_data.data, THIRD_NODE->data->token);
      function_find = findFunction(stack_data.data, function);
    }

    if (debugFor && function_find) {
      printf("function name: %s\n", function_find->ll->head->next->data->token);
      printf("function position: %p\n", function_find);
    }

    // TODO: this part isn't working properly due to parenthesis in the
    // tokens
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
          stack_data.value->identity = V_INT;
          stack_data.value->v.i = atoi(COMPARE(THIRD_NODE));
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
      memshow(*mem);
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
      if (debugFor)
        printf("\n\tfunction in\n\n");
      function_handler(function, mem, 1, function_find);
      if (debugFor)
        printf("\n\tfunction returned\n\n");
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
