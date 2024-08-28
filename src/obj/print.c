#include "../include/print.h"
#include "../include/dll.h"
#include "../include/stack.h"
#include "../include/ll.h"

#include <stdio.h>

void print(ll_t *list, stacks_t *mem) {
    ll_node_t *printnode = list->head;
  char *printstr = printnode->next->data->token;
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

    // printnode = printnode->next;
    printnode = printnode->next;
    percent = *printnode->next->data->token == '%';
    empty = *printnode->next->data->token == '\0';
  }

  printnode = printnode->next;

  int *i = NULL;

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
      i = bringval(printnode->data->token, mem, 0);

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
}
