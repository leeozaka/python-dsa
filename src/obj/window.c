#include "../include/window.h"
#include <corecrt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define TL 218
#define TR 191
#define BL 192
#define BR 217
#define ROW 196
#define COL 179

static const info_t expect = {0, 0, 0, 0, 131, 35};

extern char rep[100][300];
extern int line_number;

static char console[300][300];
static int consoleLineCount = 0;

static const struct errtext {
  char *msg;
  char *confirmdialog;
  char *msg2;
} errtext = {"Por favor, aumente a resolucao da tela para 130x40 ou mais",
             "Aumente a resolucao da tela para continuar",
             "Pressione 'q' para sair"};

info_t checkwnd() {
  textbackground(BLACK);
  textcolor(WHITE);

  info_t window;
  gettextinfo(&window);

  if (!window_verify()) {
    DWORD cNumRead, fdwMode;
    INPUT_RECORD irInBuf;

    HANDLE hStdin;
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE) {
      exit(3);
    }

    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    system("cls");

    SetConsoleMode(hStdin, fdwMode);

    int x = (window.screenwidth - strlen(errtext.msg)) / 2;
    int y = window.screenheight / 2;

    for (int i = 0; i < 3; i++)
      flashbackground(RED, 100);

    do {
      system("cls");
      gettextinfo(&window); // Refresh window information *inside* the loop

      x = (window.screenwidth - strlen(errtext.msg)) / 2;
      y = window.screenheight / 2;

      textcolor(RED);
      gotoxy(x, y);
      cputs(errtext.msg);
      textcolor(WHITE);
      x = (window.screenwidth - strlen(errtext.confirmdialog)) / 2;
      y++;
      gotoxy(x, y);
      cputs(errtext.confirmdialog);
      x = (window.screenwidth - strlen(errtext.msg2)) / 2;
      y++;
      gotoxy(x, y);
      cputs(errtext.msg2);
      ReadConsoleInput(hStdin, &irInBuf, 1, &cNumRead);

      if (irInBuf.EventType == KEY_EVENT) {
        if (irInBuf.Event.KeyEvent.bKeyDown) {
          if (irInBuf.Event.KeyEvent.uChar.AsciiChar == 'q') {
            system("cls");
            exit(1);
          }
        }
      }

      if (irInBuf.EventType == WINDOW_BUFFER_SIZE_EVENT) {
        gettextinfo(&window);
      }

    } while (
        irInBuf.Event.WindowBufferSizeEvent.dwSize.X < expect.screenwidth ||
        irInBuf.Event.WindowBufferSizeEvent.dwSize.Y < expect.screenheight);
  }
  gettextinfo(&window);
  system("cls");

  return window;
}

inline uint8_t window_verify() {
  info_t w;

  gettextinfo(&w);
  return w.screenwidth >= expect.screenwidth + 1 &&
         w.screenheight >= expect.screenheight + 1;
}

int padding_x, padding_y;

void window_draw(int curline) {
  checkwnd();

  info_t w;

  gettextinfo(&w);

  system("cls");

  padding_x = w.screenwidth > expect.screenwidth
                  ? (w.screenwidth - expect.screenwidth) / 2
                  : 0;

  padding_y = w.screenheight > expect.screenheight
                  ? (w.screenheight - expect.screenheight) / 2
                  : 0;

  // draw corners
  gotoxy(padding_x, padding_y);
  fputc(TL, stdout);
  gotoxy(padding_x + expect.screenwidth, padding_y);
  fputc(TR, stdout);
  gotoxy(padding_x, padding_y + expect.screenheight);
  fputc(BL, stdout);
  gotoxy(padding_x + expect.screenwidth, padding_y + expect.screenheight);
  fputc(BR, stdout);

  // draw col
  for (int i = 1; i <= expect.screenheight - 1; i++) {
    gotoxy(padding_x, i + padding_y);
    fputc(COL, stdout);

    gotoxy(padding_x + expect.screenwidth, i + padding_y);
    fputc(COL, stdout);
  }

  for (int i = 1; i <= expect.screenwidth - 1; i++) {
    gotoxy(i + padding_x, padding_y);
    fputc(ROW, stdout);

    gotoxy(i + padding_x, padding_y + expect.screenheight);
    fputc(ROW, stdout);
  }

  // goto the top of the pwindow
  gotoxy(padding_x + 1, padding_y + 1);

  for (int x = 0; x < line_number; x++) {
    if (x == curline - 1) {
      textbackground(RED);
      textcolor(WHITE);
    } else {
      textbackground(BLACK);
      textcolor(WHITE);
    }
    printf("%s", rep[x]);
    gotoxy(padding_x + 1, padding_y + 1 + (x + 1));
  }

  // goto the bottom -10 of the pwindow
  //(this will be our console?)
  gotoxy(padding_x + 1, padding_y + expect.screenheight - 10);
  for (int x = 0; x < consoleLineCount && x < 10; x++) {
    textbackground(BLACK);
    textcolor(WHITE);
    printf("%s", console[x]);
    gotoxy(padding_x + 1, padding_y + expect.screenheight - 10 + (x + 1));
  }

  gotoxy(padding_x + expect.screenwidth, padding_y + expect.screenheight);
}

void appendConsoleLine(char *line) {
  strcpy(console[consoleLineCount], line);
  consoleLineCount+=1;
}
