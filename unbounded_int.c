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
    unbounded_int u1 = string2unbounded_int("-0000100");
    unbounded_int u2 = string2unbounded_int("-00000000015");
    unbounded_int u3 = ll2unbounded_int(1000);

    printf("unbounded_int_cmp_unbounded_int(u1, u2) = %d\n", unbounded_int_cmp_unbounded_int(u1, u2));
    printf("unbounded_int_cmp_unbounded_int(u2, u1) = %d\n", unbounded_int_cmp_unbounded_int(u2, u1));

    char* u1_str = unbounded_int2string(u1);
    long long test = 911;
    printf("unbounded_cmp between unbounded_int %s and long long %lld = %d\n",
           u1_str, test, unbounded_int_cmp_ll(u1, test));
    free(u1_str);

    print_unbounded_int(u1);
    print_unbounded_int(u2);

    unbounded_int sub = unbounded_int_difference(u1, u2);
    // unbounded_int som = unbounded_int_somme_aux(u1, u2);
    //print_unbounded_int(som);
    //destroy_unbounded_int(som);
    
    // Je lis dans les deux sens pour verifier que tous
    // les liens precedent/suivant, premier/dernier fonctionne.
    print_unbounded_int(sub);
    print_unbounded_int_left(sub);

    print_unbounded_int(u3);

    destroy_unbounded_int(sub);
    // destroy_unbounded_int(som);
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

    return delete_useless_zero(number); // On supprime les 0 inutiles si besoin.
}

char* longToStr(long long i) {
    int length = snprintf( NULL, 0, "%lld", i);
    char* str = malloc(length + 1);

    snprintf(str, length + 1, "%lld", i);
    // str[length] = '\0'; INFO: A terminating null character is automatically appended after the content written.

    return str;
}

unbounded_int ll2unbounded_int(long long i) {
    /*
        @param i: integer  
        @return: integer represented in unbounded_int structure 
        This function converts a long long in a char*, then calls string2unbounded_int which returns a struct.

        @see: snprintf(): A terminating null character is automatically appended after the content written.
    */

    char* str = longToStr(i);

    unbounded_int res = string2unbounded_int(str);
    free(str);
    return res;
}

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b) {
    /*
        @param: a -> integer represented by unbouded_int struct.
        @param: b -> integer represented by unbouded_int struct.
        @return: -1 if a < b; 0 if a == b; 1 if a > b

        @see (delete after reading): Je crois que si on pouvait faire  a - b ce serait plus simple, mais car
        dans les exercices cela n'est pas encore fait en théorie, cette méthode de soustraction sera crée dans l'exo 7 donc
        je pense qu'il veut que je compare élément par élement au lieu de voir le résultat de a - b
    */

   if (a.signe == '+' && b.signe == '-') return 1;
   if (b.signe == '+' && a.signe == '-') return -1;

   if(a.len > b.len) return 1;
   if(a.len < b.len) return -1;

    for(chiffre *n_a= a.premier, *n_b = b.premier; n_a != NULL && n_b != NULL; n_a=n_a->suivant, n_b = n_b->suivant) {
        if(n_a->c > n_b->c) return 1;
        if(n_a->c < n_b->c) return -1;
    }
    return 0;
}

int unbounded_int_cmp_ll(unbounded_int a, long long b) {
    char* str = longToStr(b);
    unbounded_int bu = string2unbounded_int(str); // On convertit b en unbounded_int
    free(str); // On libere la memoire de str
    
    int result = unbounded_int_cmp_unbounded_int(a, bu); // On compare a et bu
    destroy_unbounded_int(bu); // On detruit bu.

    return result;
}

static unbounded_int unbounded_int_somme_aux(unbounded_int a, unbounded_int b) {
    /*
        @consider: a, b > 0.
        @todo: need to add lenght. 
    */
    unbounded_int error = (unbounded_int){.signe='*'};
    if(a.signe == '*' || b.signe == '*')
        return error;

    unbounded_int sommeInt = {.premier=NULL, .dernier=NULL, .signe='+'};
    int retenue = 0;
    chiffre *c_n = NULL, *c_prev_n = NULL, *a_n=a.dernier, *b_n=b.dernier;
    
    for(; a_n != NULL && b_n != NULL; a_n=a_n->precedent, b_n=b_n->precedent) {
        int tmp = (a_n->c-'0') + (b_n->c-'0') + retenue;
        retenue = tmp / 10;
        c_n = malloc(sizeof(chiffre));
        if(c_n == NULL) return error;
        if(a_n == a.dernier || b_n == b.dernier) {sommeInt.dernier = c_n;}
        c_n->c = (tmp % 10) + '0';
        c_n->suivant = c_prev_n;
        if(c_prev_n != NULL) c_prev_n->precedent = c_n;
        c_prev_n = c_n;
        
    }

    if (a.len > b.len) {
        loop_and_add(a_n, retenue, c_n->precedent, error);
    }
    if (b.len > a.len) {
        loop_and_add(b_n, retenue, c_n->precedent, error);
    }

    sommeInt.premier = c_n;
    sommeInt.premier->precedent = NULL;

    return sommeInt;
}

static unbounded_int loop_and_add(chiffre* x_n, int retenue, chiffre* c_n, unbounded_int error) {
    /*
        @info: fonction auxiliaire, utilisé par unbounded_int_somme_aux(param1, param2)
    */
    chiffre* prev = x_n;
    for(; x_n != NULL; x_n = x_n->precedent) {
        int tmp = x_n->c + retenue;
        retenue = tmp / 10;
        c_n = malloc(sizeof(chiffre));
        if(c_n == NULL) return error;
        c_n->c = tmp % 10;
        c_n->suivant = prev;
        prev->precedent = c_n;
        prev = c_n;
    }

    if(retenue != 0) {
        c_n = malloc(sizeof(chiffre));
        if(c_n == NULL) return error;
        c_n->c = retenue;
        c_n->suivant = prev;
        prev->suivant = c_n;
    }
    unbounded_int x = {.premier=NULL, .dernier=NULL, .signe='+'};
    return x;
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

// Permet de retirer les '0' inutile au debut d'un nombre.
static unbounded_int delete_useless_zero(unbounded_int nb) {
    if(nb.signe == '*' || nb.len <= 1)
        return nb;
    chiffre* c = nb.premier;
    for(;c->c == '0' && c != nb.dernier;) {
        c = c->suivant; // c->suivant forcement different de NULL ici.
        free(c->precedent);
        c->precedent = NULL;
        nb.len--;
    }
    nb.premier = c; // c ne peut pas etre NULL ici.
    return nb;
}

unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b) {
    if(a.signe == '*' || b.signe == '*')
        return (unbounded_int){.signe='*'};
    if(a.signe == '+' && b.signe == '-') {
        b.signe = '+';
        return b; // return unbounded_int_somme(a, b);
    }
    else if(a.signe == '-' && b.signe == '+') {
        a.signe = '+';
        unbounded_int sum; // = unbounded_int_somme(b, a);
        sum.signe = '-';
        return sum;
    }
    else if(a.signe == '-' && b.signe == '-') {
        a.signe = '+';
        b.signe = '+';
        return unbounded_int_difference(b, a);
    }
    // a, b >= 0
    if(unbounded_int_cmp_unbounded_int(a, b) >= 0)
        return unbounded_int_difference_aux(a, b);
    // Si a < b. Alors on fait -(b-a)
    unbounded_int result = unbounded_int_difference_aux(b, a);
    result.signe = '-';
    return result;
}

static unbounded_int unbounded_int_difference_aux(unbounded_int a, unbounded_int b) {
    unbounded_int error = (unbounded_int){.signe='*'};
    if(a.signe == '*' || b.signe == '*')
        return error;
    unbounded_int subInt = {.premier=NULL, .dernier=NULL, .signe='+', .len=a.len};

    // ON SUPPOSE a > b
    chiffre* pa = a.dernier, *pb = b.dernier,
            *cSub = NULL, *prev = NULL;
    int r = 0, sub = 0;
    
    // On verifie seulement si pb != NULL car pa a une taille >= pb
    for(;pb != NULL;pa=pa->precedent, pb=pb->precedent) {
        cSub = malloc(sizeof(chiffre));
        if(cSub == NULL)
            return error;
        if(pa == a.dernier) // Pas besoin de verifier si pb == b.dernier
            subInt.dernier = cSub;
        sub = pa->c - pb->c + r; // Pas besoin de convertir pa->c et pb->c en int car : '5'-'3' == 5-3
        r = sub < 0 ? -1 : 0;
        cSub->c = sub < 0 ? (sub+10)+'0' : sub+'0';
        cSub->suivant = prev; // prev peut etre NULL
        if(prev != NULL)
            prev->precedent = cSub;
        prev = cSub;
    }

    // Ici, pb == NULL. On parcourt (ou pas) les chiffres restants de a
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

    return delete_useless_zero(subInt);;
}