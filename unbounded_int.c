#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "unbounded_int.h"

void destroy_unbounded_int(unbounded_int u);
void print_unbounded_int(unbounded_int u);
int isSign(char c);
int isNumber(const char *e);

int main() {
    unbounded_int u = string2unbounded_int("-22323986786976");
    //printf("%ld\n", u.len);
    //printf("%c\n", u.premier->c);
    print_unbounded_int(u);
    destroy_unbounded_int(u);
    return EXIT_SUCCESS;
}

void print_unbounded_int(unbounded_int u) {
    if(u.signe == '*') {
        puts("ERROR NUMBER");
        return;
    }
    printf("%c", u.signe);
    for(chiffre* c=u.premier;c != NULL;c=c->suivant) {
        printf("%c", c->c);
    }
    puts(""); // Retour a la ligne.
}

void destroy_unbounded_int(unbounded_int u) {
    if(u.signe == '*' || u.len == 0)
        return;
    chiffre* c = u.premier, *next = u.premier;
    for(;c != NULL;c=next) {
        next = next->suivant;
        free(c);
    }
}

int isSign(char c) {
    return (c == '+' || c == '-') ? 1 : 0;
}

int isNumber(const char *e) {
    char* c=(char *)e;
    if(*c == '\0')
        return 0;
    if(isSign(*c)) {
        if(c[1] == '\0')
            return 0;
        c++;
    }
    for(;*c != '\0';c++)
        if(!isdigit(*c))
            return 0;
    return 1;
}

unbounded_int string2unbounded_int(const char *e) {
    size_t len = strlen(e);
    unbounded_int error = {.signe='*'};
    if(!isNumber(e))
        return error;

    int i=0;
    unbounded_int number = {.premier = NULL, .dernier = NULL};
    chiffre *prev = NULL, *actual = NULL;

    if(isSign(e[i])) {
        number.signe = e[i++];
        number.len = len-1;
    }
    else {
        number.signe = '+';
        number.len = len;
    }

    for(;i<len;i++) {
        actual = malloc(sizeof(chiffre));
        if(actual == NULL)
            return error;
        if(i == 0 || (i == 1 && !isdigit(e[i-1]))) {
            number.premier = actual;
        }
        actual->c = e[i];
        actual->precedent = prev; // prev peut valoir NULL. Pas de pb.
        if(prev != NULL)
            prev->suivant = actual;
        prev = actual;
    }
    if(actual != NULL)
        actual->suivant = NULL;
    number.dernier = actual;

    return number;
}