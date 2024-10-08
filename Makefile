SRC = $(wildcard src/*.c)
SRC += $(wildcard src/*/*.c)
OBJ = ${SRC:.c=.o}
OUT = main

CC = clang
CFLAGS = -std=c11 -O3 -Wall -Wextra
clang = $(shell which clang 2> /dev/null)
ifeq (, $(clang))
	CC = gcc
endif

$(OUT): $(OBJ)
	@$(CC) $^ -o $@
	@printf "\e[33mLinking\e[90m %s\e[0m\n" $@
	@printf "\e[34mDone!\e[0m\n"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "\e[36mCompile\e[90m %s\e[0m\n" $@

clean:
	@rm -f $(OUT) $(OBJ)
	@printf "\e[34mAll clear!\e[0m\n"


