#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "unbounded_int.h"

int main() {
    /*
    unbounded_int u = string2unbounded_int("-22379308999827643656");
    //printf("%ld\n", u.len);
    //printf("%c\n", u.premier->c);
    print_unbounded_int(u);
    char* str = unbounded_int2string(u);
    printf("%s\n", str);
    free(str);
    destroy_unbounded_int(u);
    */
    unbounded_int u1 = string2unbounded_int("666");
    unbounded_int u2 = string2unbounded_int("66");
    unbounded_int u3 = ll2unbounded_int(1000);

    unbounded_int sub = unbounded_int_difference(u1, u2);

    // Je lis dans les deux sens pour verifier que tous
    // les liens precedent/suivant, premier/dernier fonctionne.
    print_unbounded_int(sub);
    print_unbounded_int(u3);

    print_unbounded_int_left(sub);

    destroy_unbounded_int(sub);
    destroy_unbounded_int(u1);
    destroy_unbounded_int(u2);
    destroy_unbounded_int(u3);

    return EXIT_SUCCESS;
}

static void print_unbounded_int(unbounded_int u) {
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

static void print_unbounded_int_left(unbounded_int u) {
    if(u.signe == '*') {
        puts("ERROR NUMBER");
        return;
    }
    
    for(chiffre* c=u.dernier;c != NULL;c=c->precedent) {
        printf("%c", c->c);
    }
    printf("%c\n", u.signe);
}

static void destroy_unbounded_int(unbounded_int u) {
    if(u.signe == '*' || u.len == 0)
        return;
    chiffre* c = u.premier, *next = u.premier;
    for(;c != NULL;c=next) {
        next = next->suivant;
        free(c);
    }
}

static int isSign(char c) {
    return (c == '+' || c == '-') ? 1 : 0;
}

static int isNumber(const char *e) {
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

unbounded_int ll2unbounded_int(long long i) {
    /*
        @param i: integer  
        @return: integer represented in unbounded_int structure 

        This function converts a long long in a char*, then calls string2unbounded_int which returns a struct.
    */

    int length = snprintf( NULL, 0, "%lld", i);
    char* str = malloc( length + 1 );

    snprintf(str, length + 1, "%lld", i);
    unbounded_int res = string2unbounded_int(str);
    free(str);
    return res;
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

void switchUInt(unbounded_int *a, unbounded_int *b) {
    unbounded_int* tmp = a;
    a = b;
    b = tmp;
}

void switchSign(unbounded_int *u) {
    if(u != NULL)
        u->signe = u->signe == '-' ? '+' : '-';
}

unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b) {
    unbounded_int error = (unbounded_int){.signe='*'};
    if(a.signe == '*' || b.signe == '*')
        return error;
    unbounded_int subInt = {.premier=NULL, .dernier=NULL, .signe='+'};

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ICI IL FAUDRA VERIFIER SI a > b ou b <= a //
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // ON SUPPOSE a > b
    chiffre* pa = a.dernier, *pb = b.dernier,
            *cSub = NULL, *prev = NULL;
    int r = 0, sub = 0;
    
    for(;pa != NULL && pb != NULL;pa=pa->precedent, pb=pb->precedent) {
        cSub = malloc(sizeof(chiffre));
        if(cSub == NULL)
            return error;
        if(pa == a.dernier || pb == b.dernier)
            subInt.dernier = cSub;
        sub = pa->c - pb->c + r; // Pas besoin de convertir pa->c et pb->c en int car : '5'-'3' == 5-3
        r = sub < 0 ? -1 : 0;
        cSub->c = sub < 0 ? (sub+10)+'0' : sub+'0';
        cSub->suivant = prev; // prev peut etre NULL
        if(prev != NULL)
            prev->precedent = cSub;
        prev = cSub;
    }

    for(;pa != NULL;pa = pa->precedent) {
        cSub = malloc(sizeof(chiffre));
        if(cSub == NULL)
            return error;
        cSub->c = pa->c + r;
        r=0;
        cSub->suivant = prev; // prev peut etre NULL
        if(prev != NULL)
            prev->precedent = cSub;
        prev = cSub;
    }
    subInt.premier = prev;
    subInt.premier->precedent = NULL;

    return subInt;
}