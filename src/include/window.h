#pragma once

#include "c.h"
#include <stdint.h>

typedef struct text_info info_t;

typedef struct WINDOW_TEXT_POS {
  uint8_t x;
  uint8_t y;
} wtext;

info_t checkwnd();
uint8_t window_verify();
void window_draw(int curline);
void appendConsoleLine(char *line);
