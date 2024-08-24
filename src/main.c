#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/data.h"
#include "include/dll.h"
#include "include/ll.h"

const char *text_to_tokenize_1 = "def hello(a, b):";
const char *text_to_tokenize_2 = "  print(a + b)";
const char *text_to_tokenize_3 = "  return a + b";

enum {
    KEYWORD,
    IDENTIFIER,
    OPERATOR,
    PUNCTUATION,
    LITERAL,
    COMMENT,
    WHITESPACE,
    NEWLINE,
    END
};

int classifier(char *c) {
    switch (*c) {
        case ' ':
        case '\t':
            return WHITESPACE;
        case '\n':
            return NEWLINE;
        case '#':
            return COMMENT;
        case '(':
        case ')':
        case '[':
        case ']':
        case '{':
        case '}':
        case ',':
        case ':':
        case '.':
            return PUNCTUATION;
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '&':
        case '|':
        case '^':
        case '~':
        case '<':
        case '>':
        case '=':
        case '!':
            return OPERATOR;
        case '0' ... '9':
            return LITERAL;
        default:
            return IDENTIFIER;
    
    }
}

int main() { 
    dll_t *main = dll_create();
    insert(ll_create(), &main);
 
    // token_data_t token, token1, token2, token3;
    // strcpy(token.token, "    ");
    // strcpy(token1.token, "Hello, ");
    // strcpy(token3.token, "    ");
    // strcpy(token2.token, "World!");


    // ll_insert(token, &main->head->ll);
    // ll_insert(token1, &main->head->ll);
    // ll_insert(token3, &main->head->ll);
    // ll_insert(token2, &main->head->ll);

    ll_show(main->head->ll);
    
    //init first linked list for main dll
    insert(ll_create(), &main);
     // for (size_t i = 0; i < strlen(text_to_tokenize_1); i++) {
     //     switch (classifier(&text_to_tokenize_1[i])) {
     //         case WHITESPACE:
     //     }
     // } 

}
