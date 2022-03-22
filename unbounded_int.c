#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "unbounded_int.h"

int main() {
    unbounded_int u = string2unbounded_int("-22379308999827643656");
    //printf("%ld\n", u.len);
    //printf("%c\n", u.premier->c);
    print_unbounded_int(u);
    char* str = unbounded_int2string(u);
    printf("%s\n", str);
    free(str);
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

char *unbounded_int2string(unbounded_int i) {
    if(i.signe == '*')
        return NULL;
    int sign = i.signe == '-' ? 1 : 0;
    char* str = malloc(sizeof(char) * (i.len+sign+1));
    if(str == NULL)
        return NULL;
    char* ptr = str;
    if(sign) {
        *ptr = '-';
        ptr++;
    }

    for(chiffre* current = i.premier;current != NULL;ptr++, current=current->suivant)
        *ptr = current->c;

    str[i.len + sign] = '\0';
    return str;
}