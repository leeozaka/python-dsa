#include "../include/print.h"
#include "../include/dll.h"
#include "../include/ll.h"
#include "../include/stack.h"
#include "../include/strctrl.h"
#include "../include/window.h"
#include <string.h>

int printFunction = 0;

void print(dll_node_t *node, stacks_t *mem) {
  char printappend[300] = "";

  value_t *val = new_value();
  if (*THIRD_NODE->data->token == '"') {
    ll_node_t *printnode = THIRD_NODE;
    char *printstr = THIRD_NODE->data->token;
    char *printc;
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
        strcat(printappend, printc);
        // putchar(*printc);
        printc++;
      }
      if (*printc == '%' || *printc == '\\') {
        printc++;
        switch (*printc) {
        case 'd':
          val = bringval(printnode->data->token, mem);

          if (val) {
            char str[12];
            if (val->identity == V_INT) {
              sprintf(str, "%d", val->v.i);
              strcat(printappend, str);
            } else {
              strcat(printappend, printnode->data->token);
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
          strcat(printappend, "\n");
          printc++;
          break;

        case 'f':
          val = bringval(printnode->data->token, mem);

          if (val) {
            char str[12];
            if (val->identity == V_FLOAT) {
              sprintf(str, "%f", val->v.f);
              strcat(printappend, str);

              // fprintf(stdout, "%f", val->v.f);
            } else {
              strcat(printappend, printnode->data->token);
              // fprintf(stdout, "%f", atof(printnode->data->token));
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
              char str[12];
            case V_INT:
              sprintf(str, "%d", val->v.i);
              strcat(printappend, str);

              // fprintf(stdout, "%d", val->v.i);
              break;
            case V_FLOAT:
              sprintf(str, "%f", val->v.f);
              strcat(printappend, str);

              // fprintf(stdout, "%f", val->v.f);
              break;
            case V_STRING:
              strcat(printappend, val->v.str);

              // fprintf(stdout, "%s", val->v.str);
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
        char str[12];
        switch (val->identity) {
        case V_INT:
          sprintf(str, "%d", val->v.i);
          strcat(printappend, str);

          // fprintf(stdout, "%d ", val->v.i);
          break;
        case V_FLOAT:
          sprintf(str, "%f", val->v.f);
          strcat(printappend, str);
          // fprintf(stdout, "%f ", val->v.f);
          break;
        case V_STRING:
          strcat(printappend, val->v.str);
          // fprintf(stdout, "%s", val->v.str);
          break;
        }
      } else {
        strcat(printappend, printnode->data->token);
        // fprintf(stdout, "%s", printnode->data->token);
      }
      printnode = printnode->next;
    }

    appendConsoleLine(printappend);
  }
}
