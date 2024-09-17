#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/data.h"
#include "include/dll.h"
#include "include/function.h"
#include "include/ll.h"
#include "include/stack.h"
#include "include/strctrl.h"

#define NO_DEPTH 0

#ifdef _WIN64
// only reason to include this is to use the segfault
// message in win development fuck
#include <signal.h>

void segvHandler() {
  printf("Segmentation fault\n");
  exit(EXIT_FAILURE);
}
#endif

int debug = 1;

int main() {
#ifdef _WIN64
  signal(SIGSEGV, segvHandler);
#endif

  int type;
  size_t index = 0, line_number = 0;
  char token_text[TOKENSIZE] = "", *line, rep[TOKENSIZE][300];
  token_data_t token;

  struct dirent *entry;
  const char *directory_path = "./";
  DIR *dir = opendir(directory_path);

  if (dir == NULL) {
    return -1;
  }

  // allocate memory for up to 100 files
  // but problably you will not have 100 files in the directory lol
  char **files = (char **)malloc(100 * sizeof(char *));
  for (int i = 0; i < 100; i++) {
    files[i] = (char *)malloc(100 * sizeof(char));
  }

  // unsigned because we are not going to have negative files
  unsigned int files_count = 0;
  while ((entry = readdir(dir)) != NULL) {
    if (strstr(entry->d_name, ".py") != NULL) {
      strcpy(files[files_count], entry->d_name);
      files_count++;
    }
  }

  for (unsigned int i = 0; i < files_count; i++) {
    printf("  %d. %s\n", i + 1, files[i]);
  }

  unsigned int file_number;
  if (files_count == 1) {
    file_number = 1;
  } else {
    printf("Enter the file number to run: ");
    fscanf(stdin, "%d", &file_number);
  }

  FILE *stream;
  if (file_number <= files_count && file_number > 0) {
    stream = fopen(files[file_number - 1], "r");
    assert(stream);
    if (debug)
      printf("Running %s\n", files[file_number - 1]);

  } else {
    printf("Invalid file number\n");
    exit(-1);
  }

  closedir(dir);

  // clean up thrash
  for (int i = 0; i < 100; i++) {
    free(files[i]);
  }
  free(files);

  line = (char *)calloc(STRSIZE, sizeof(char));
  dll_t *main = dll_create();

  stacks_t *mem = stack_create();

  // not verifying the maximum lines that u could enter here + i dont care
  while (fgets(line, STRSIZE, stream)) {
    strcpy(rep[line_number], line);
    line_number++;
    if (classifier(line) == COMMENT || classifier(line) == NEWLINE) {
      continue;
    }
    dll_node_t *actual = insert(ll_create(line_number), &main);

    while (*line != '\0' && *line != '\n') {
      if (startsWithFourSpaces(line)) {
        strcpy(token.token, "");
        ll_insert(token, &actual->ll);
        line += 4;
        continue;
      }

      type = classifier(line);
      if (type == BRACKET || type == PARENTHESIS) {
        token_text[index++] = *line;
        token_text[index] = '\0';
        line++;
        strcpy(token.token, token_text);
        ll_insert(token, &actual->ll);
        memset(token_text, 0, sizeof(token_text));
        index = 0;
        continue;
      }

      if (type == WHITESPACE || type == PUNCTUATION) {
        line++;
        continue;
      }

      if (type != QUOTE)
        while (type != WHITESPACE && type != PUNCTUATION && type != NEWLINE &&
               type != COMMENT && type != BRACKET && type != PARENTHESIS) {
          token_text[index++] = *line;
          line++;

          type = classifier(line);
        }
      else {
        token_text[index++] = *line;
        line++;
        while (classifier(line) != QUOTE) {
          token_text[index++] = *line;
          line++;
        }
        token_text[index++] = *line;
        line++;
      }

      strcpy(token.token, token_text);

      ll_insert(token, &actual->ll);

      memset(token_text, 0, sizeof(token_text));
      index = 0;

      // if (*(line + 1) != '\n') {
      //   line++;
      // }
    }
  }

  // file read loop end
  fclose(stream);

  if (debug) {
    for (dll_node_t *node = main->head; node; node = node->next) {
      printf(" ");
      ll_show(node->ll);
    }

    printf("\n execution stage \n\n");
  }

  function_handler(main, &mem, NO_DEPTH, NULL);

  if (debug) {
    printf("\n\n memory dump \n\n");
    if (mem)
      memshow(mem);

    printf("\t end \n");
  }
  return 0;
}
