#include "../include/print.h"
#include "../include/dll.h"
#include "../include/ll.h"
#include "../include/stack.h"
#include "../include/strctrl.h"
#include <string.h>

int printFunction = 0;

void print(dll_node_t *node, stacks_t *mem) {
  value_t *val = new_value();
  if (*THIRD_NODE->data->token == '"') {
    ll_node_t *printnode = THIRD_NODE;
    char *printstr = THIRD_NODE->data->token;
    char *printc;
    // int i = 0;

    // empty = *printnode->data->token == '\0';

    // while (printnode && !percent) {
    //   if (!printnode) {
    //     perror("invalid print statement 1");
    //     exit(69);
    //   }
    //   printnode = printnode->next;
    //     percent = *printnode->data->token == '%';
    // }

    while (printnode) {
      if (*printnode->data->token == '%') {
        break;
      }
      printnode = printnode->next;
    }

    if (printnode)
      printnode = printnode->next;

    // if (printFunction) {
    //   printf("printnode: %s\n", printnode->data->token);
    // }

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
          val = bringval(printnode->data->token, mem);

          if (val) {
            if (val->identity == V_INT) {
              fprintf(stdout, "%d", val->v.i);
            } else {
              fprintf(stdout, "%d", atoi(printnode->data->token));
            }
          } else {
            printf("variable %s not found\n", printnode->data->token);
            exit(EXIT_FAILURE);
          }

          printc++;
          printnode = printnode->next;
          break;

        case 'n':
          fprintf(stdout, "\n");
          printc++;
          break;

        case 'f':
          val = bringval(printnode->data->token, mem);

          if (val) {
            if (val->identity == V_FLOAT) {
              fprintf(stdout, "%f", val->v.f);
            } else {
              fprintf(stdout, "%f", atof(printnode->data->token));
            }
          } else {
            printf("variable %s not found\n", printnode->data->token);
            exit(EXIT_FAILURE);
          }

          printc++;
          printnode = printnode->next;
          break;

        case 's':
          val = bringval(printnode->data->token, mem);
          if (printFunction)
            printf("val == string? %d\n", val->identity == V_STRING);

          if (val) {
            switch (val->identity) {
            case V_INT:
              fprintf(stdout, "%d", val->v.i);
              break;
            case V_FLOAT:
              fprintf(stdout, "%f", val->v.f);
              break;
            case V_STRING:
              fprintf(stdout, "%s", val->v.str);
              break;
            }
          } else {
            printf("variable %s not found\n", printnode->data->token);
            exit(EXIT_FAILURE);
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
    ll_node_t *printnode = node->ll->head->next->next;
    while (strcmp(printnode->data->token, ")") != 0) {
      val = bringval(printnode->data->token, mem);
      if (val) {
        switch (val->identity) {
        case V_INT:
          fprintf(stdout, "%d ", val->v.i);
          break;
        case V_FLOAT:
          fprintf(stdout, "%f ", val->v.f);
          break;
        case V_STRING:
          fprintf(stdout, "%s", val->v.str);
          break;
        }
      } else {
        fprintf(stdout, "%s", printnode->data->token);
      }
      printnode = printnode->next;
    }
    printf("\n");
  }
}
