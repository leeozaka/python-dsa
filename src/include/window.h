#pragma once

#include "c.h"
#include "stack.h"
#include <stdint.h>

typedef struct text_info info_t;

void checkwnd();
uint8_t window_verify();
void window_draw(int curline);
void appendConsoleLine(char *line);
void alert();
void wmemshow(stacks_t *mem);
