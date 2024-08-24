#include <stdlib.h>

struct stringdin {
    char letra;
    struct stringdin *prox;
};

typedef struct stringdin StrDin;

void reinicia(StrDin** str) {
    StrDin* aux;

    while (*str) {
        aux = *str;
        *str = (*str)->prox;
        free(aux);
    }
}

void reiniciarec(StrDin** str) {
    if (*str != NULL) {
        reiniciarec(&(*str)->prox);
        free(*str);
    }
}

void tam(StrDin *str, int* cont) {
    
}


int tam2(StrDin *str) {
    int i = 0;

    if (str) {
        i+=tam2(str->prox);
        return 1;
    }

    return i;
}
