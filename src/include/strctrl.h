#pragma once
#define FIRST_NODE node->ll->head
#define SECOND_NODE node->ll->head->next
#define THIRD_NODE node->ll->head->next->next
#define FOURTH_NODE node->ll->head->next->next->next

#define COMPARE(NODE)                                                          \
  (strcmp(NODE->data->token, "(") == 0 ? NODE->next->data->token               \
                                       : NODE->data->token)

#include <ctype.h>
#include <string.h>

#include "data.h"

int classifier(char *c);
int startsWithFourSpaces(const char *str);
