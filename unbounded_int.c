#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "unbounded_int.h"

/* FONCTIONS AUXILIAIRES */

static int isSign(char c);
static int isNumber(const char *e);
static unbounded_int delete_useless_zero(unbounded_int nb);
static unbounded_int unbounded_int_somme_aux(unbounded_int a, unbounded_int b);
static unbounded_int unbounded_int_difference_aux(unbounded_int a, unbounded_int b);
static int unbounded_greater_equal_zero(unbounded_int a);
static int unbounded_lesser_equal_zero(unbounded_int a);
static unbounded_int unbounded_int_produit_aux(unbounded_int a, unbounded_int b);

// static chiffre* loop_and_add(chiffre* x_n, int retenue, chiffre* c_prev_n, size_t* len);

void print_unbounded_int(unbounded_int u) {
    if(u.signe == '*') {
        puts("ERROR NUMBER");
        return;
    }
    if(u.signe == '-')
        printf("%c", u.signe);
    for(chiffre* c=u.premier;c != NULL;c=c->suivant) {
        printf("%c", c->c);
    }
    puts(""); // Retour a la ligne.
}

void print_unbounded_int_left(unbounded_int u) {
    if(u.signe == '*') {
        puts("ERROR NUMBER");
        return;
    }
    
    for(chiffre* c=u.dernier;c != NULL;c=c->precedent) {
        printf("%c", c->c);
    }
    printf("%c\n", u.signe);
}

void destroy_unbounded_int(unbounded_int u) {
    if(u.signe == '*')
        return;
    int len = 0;
    chiffre* c = u.premier, *next = u.premier;
    for(;c != NULL;c=next) {
        next = next->suivant;
        free(c);
        len++;
    }
    if(u.len != len) {
        puts("##############################");
        puts("ERREUR: la len n'est pas pas la BONNE VALEUR !");
        printf("Elle vaut %ld alors que la vrai len semble etre %d\n", u.len, len);
        puts("##############################");
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

        @bug: cas où a, b < 0!
    */

    if(a.signe == '*' || b.signe == '*')
        return -2;
    // cas particulier. Si les deux valent 0.
    // En effet, il peuvent avoir un signe + ou -
    if(a.len == 1 && a.premier->c == '0' &&
       b.len == 1 && b.premier->c == '0')
       return 0;

    // Le if avant evite de renvoyer un mauvais resultat si a
    // et b valent 0.
    if (a.signe == '+' && b.signe == '-') return 1;
    if (b.signe == '+' && a.signe == '-') return -1;

    // merci @leo!
    int sign = a.signe == '+' ? 1 : -1;

    if(a.len > b.len)
        return sign;   
    if(a.len < b.len)
        return -sign;

    for(chiffre *n_a= a.premier, *n_b = b.premier; n_a != NULL && n_b != NULL; n_a=n_a->suivant, n_b = n_b->suivant) {
        if(n_a->c > n_b->c) return sign;
        if(n_a->c < n_b->c) return -sign;
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
        @info: auxiliary function used in unbouded_int_somme.
        @todo: refactor operations when a.len > b.len (b.len > a.len) into a function
    */

    unbounded_int error = (unbounded_int){.signe='*'};
    if(a.signe == '*' || b.signe == '*')
        return error;

    unbounded_int sommeInt = {.premier=NULL, .dernier=NULL, .signe='+', .len=0};
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
        sommeInt.len++;
    }

    /*
    chiffre* res;

    if (a.len > b.len)
        res = loop_and_add(a_n, retenue, c_prev_n, &sommeInt.len);
    else if (b.len > a.len)
        res = loop_and_add(b_n, retenue, c_prev_n, &sommeInt.len);
    */

    if (a.len > b.len) { 
        for(;a_n != NULL; a_n = a_n->precedent) {
            int tmp = (a_n->c-'0') + retenue;
            retenue = tmp / 10;
            c_n = malloc(sizeof(chiffre));
            if(c_n == NULL) return error;
            c_n->c = (tmp % 10) + '0';
            c_n->suivant = c_prev_n;
            c_prev_n->precedent = c_n;
            c_prev_n = c_n;
            sommeInt.len++;
        }
    }
    else if (b.len > a.len) {
        for(;b_n != NULL; b_n = b_n->precedent) {
            int tmp = (b_n->c-'0') + retenue;
            retenue = tmp / 10;
            c_n = malloc(sizeof(chiffre));
            if(c_n == NULL) return error;
            c_n->c = (tmp % 10) + '0';
            c_n->suivant = c_prev_n;
            c_prev_n->precedent = c_n;
            c_prev_n = c_n;
            sommeInt.len++;
        }
    }
    
    if(retenue > 0) {
        c_n = malloc(sizeof(chiffre));
        if(c_n == NULL) 
            return error;

        c_n->c = retenue + '0';
        c_n->suivant = c_prev_n;
        c_prev_n->precedent = c_n;
        sommeInt.len++;
    }

    sommeInt.premier = c_n;
    sommeInt.premier->precedent = NULL;

    return sommeInt;
}

/*
static chiffre* loop_and_add(chiffre* x_n, int retenue, chiffre* c_prev_n, size_t* len) {
    // @info: fonction auxiliaire, utilisé par unbounded_int_somme_aux(a, b)
    chiffre* c_n = NULL;
    for(; x_n != NULL; x_n = x_n->precedent) {
        int tmp = (x_n->c-'0') + retenue;
        retenue = tmp / 10;
        c_n = malloc(sizeof(chiffre));
        if(c_n == NULL) return NULL;
        c_n->c = (tmp % 10) + '0';
        c_n->suivant = c_prev_n;
        c_prev_n->precedent = c_n; // c_prev_n forcement != NULL
        c_prev_n = c_n;
        *len += 1;
    }

    return c_n; // On renvoie le nombre de tour de boucle
}
*/

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b) {
    /*
        @info: 
            a + b = {
                * a + b if a, b >= 0;
                * -(|a| + |b|) if a, b <= 0
                * a - |b| if a >= 0, b < 0
                * b - |a| if b >= 0, a < 0
            }
    */
    if(a.signe == '*' || b.signe == '*')
        return (unbounded_int){.signe='*'};
    if(unbounded_greater_equal_zero(a) && unbounded_greater_equal_zero(b)) {
        return unbounded_int_somme_aux(a, b);
    }
    if(unbounded_lesser_equal_zero(a) && unbounded_lesser_equal_zero(b)) {
        a.signe = '+';
        b.signe = '+';
        unbounded_int a_plus_b = unbounded_int_somme_aux(a, b);
        a_plus_b.signe = '-';
        return a_plus_b;
    }
    if(unbounded_greater_equal_zero(a) && b.signe == '-') {
        b.signe = '+';
        return unbounded_int_difference(a, b);
    }else {
        a.signe = '+';
        return unbounded_int_difference(b, a);
    }
}

/*
    @todo: unbounded_greater_equal_zero();
    This seems very redudant.  
    I have to fix it or simplify it - Paris 
*/
static int unbounded_greater_equal_zero(unbounded_int a) {
    if(a.signe == '+' || a.premier->c == '0') {
        return 1;
    }
    return 0;
}

static int unbounded_lesser_equal_zero(unbounded_int a) {
    if(a.signe == '-' || a.premier->c == '0') {
        return 1;
    }else {
        return 0;
    }
}

static unbounded_int unbounded_int_produit_aux(unbounded_int a, unbounded_int b) {
    /*
        @info: I think that here I need to have a pointer pointing to a 
        list of predefined size (a.len + b.len and then I'll need to fill these elements)
        according to the operations.

        chiffre *numbers = [c0, c1, c2, c3, c4]
        After finishing the calculations I can connect each of them and add to an unbounded_int.
        
        Something like; c_0->precend  = c1; c1->precedent = c2 . . . 

        unbounded_int x, where x.premier = *numbers[(a.len + b.len)-1]
        x.dernier = *numbers[0]; etc.
        @todo: handle sign (- & +)
        
    */
    unbounded_int error = (unbounded_int){.signe='*'};
    if(a.signe == '*' || b.signe == '*')
        return error;
    
    int retenue = 0;
    int a_counter = 0;
    int b_counter = 0;

    int *numbers = calloc(a.len+b.len, sizeof(int));
    if(numbers == NULL) return error; 
    chiffre *a_n=a.dernier, *b_n=b.dernier;

    for(; b_n != NULL; b_n = b_n->precedent) {
        retenue = 0;
        if(b_n->c - '0' == '0')
            continue;
        for(; a_n != NULL; a_n = a_n->precedent) {
            int a = a_n->c - '0';
            int b = b_n->c - '0';
            int v = numbers[a_counter+b_counter] + (a * b) + retenue;
            numbers[a_counter+b_counter] = v % 10;
            retenue = v / 10;
            a_counter++;
        }
        a_n = a.dernier;
        numbers[b_counter+a.len] = retenue;
        b_counter++;
        a_counter = 0;
    }

    char *numbers2 = calloc(a.len+b.len + 1, sizeof(char)); 
    int counter = 0;
    
    for(int i=(a.len+b.len)-1; i>=0; i--) {
        numbers2[counter] = numbers[i] + '0';
        counter++;
    }

    free(numbers);
    unbounded_int res = string2unbounded_int(numbers2);
    free(numbers2);

    return res;
}

unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b) {
    /*
        + * + = + (même signe) -> +
        + * - = - (signe différente) -> -
        - * - = + (même signe) -> +
    */

    int res = a.signe ^ b.signe;
    unbounded_int produit = unbounded_int_produit_aux(a, b);
    if(res == 0) produit.signe = '+';
    else produit.signe = '-';

    return produit;
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
        return unbounded_int_somme(a, b); // return unbounded_int_somme(a, b);
    }
    else if(a.signe == '-' && b.signe == '+') {
        a.signe = '+';
        unbounded_int sum = unbounded_int_somme(b, a);
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
        sub = (pa->c-'0') + r;
        r = sub < 0 ? -1 : 0;
        cSub->c = sub < 0 ? (sub+10)+'0' : sub+'0';
        cSub->suivant = prev; // prev peut etre NULL
        if(prev != NULL)
            prev->precedent = cSub;
        prev = cSub;
    }
    subInt.premier = prev;
    subInt.premier->precedent = NULL;

    return delete_useless_zero(subInt);;
}

unbounded_int calculate(unbounded_int a, char op, unbounded_int b) {
    switch(op) {
        case '+':
            return unbounded_int_somme(a, b);
        case '-':
            return unbounded_int_difference(a, b);
        case '*':
            return unbounded_int_produit(a, b);
        default:
            return (unbounded_int){.signe='*'};
    }
}

unbounded_int unbounded_int_copy(unbounded_int u) {
    unbounded_int zero = string2unbounded_int("0");
    unbounded_int res = unbounded_int_somme(u, zero);
    destroy_unbounded_int(zero);
    return res;
}

unbounded_int unbounded_int_dividing_2(unbounded_int a) {
    unbounded_int error = (unbounded_int){.signe='*'};
    if(a.signe == '*')
        return error;
    unbounded_int res = {.premier=NULL, .dernier=NULL, .signe=a.signe, .len=0};
    int impair = 0, tmp;
    chiffre *c_n = NULL, *c_prev_n = NULL, *a_n=a.premier;
    
    for(; a_n != NULL; a_n=a_n->suivant) {
        tmp = ((a_n->c-'0') + impair*10) / 2;
        impair = (a_n->c - '0') % 2;
        c_n = malloc(sizeof(chiffre));
        if(c_n == NULL) return error;
        if(a_n == a.premier) {res.premier = c_n;}
        c_n->c = tmp + '0';
        c_n->precedent = c_prev_n;
        if(c_prev_n != NULL) c_prev_n->suivant = c_n;
        c_prev_n = c_n;
        res.len++;
    }
    c_n->suivant = NULL;
    res.dernier = c_n;

    return delete_useless_zero(res);
}

// Uniquement sur les nombres positifs !
char* decimal_to_binary(unbounded_int nb) {
    if(nb.signe == '*' || nb.signe == '-')
        return NULL;
    int size = 20;
    char* bin = calloc(sizeof(char), size);
    if(bin == NULL)
        return NULL;
    bin[0] = '0'; // important ! si nb vaut 0 cela corrige un bug.

    unbounded_int tmp = unbounded_int_copy(nb);
    unbounded_int tmp2;
    int i=0;

    for(i=0;unbounded_int_cmp_ll(tmp, 0) != 0;i++, tmp=tmp2) {
        if(i == size-1) {
            char* ptr =  realloc(bin, size * 2);
            if(ptr == NULL)
                return NULL;
            bin = ptr;
            size *= 2;
        }
        bin[i] = ((tmp.dernier->c - '0') % 2) + '0';
        tmp2 = unbounded_int_dividing_2(tmp);
        destroy_unbounded_int(tmp);
    }
    destroy_unbounded_int(tmp);

    int realSize = i == 0 ? 1 : i;
    char* cpy = calloc(sizeof(char), realSize + 1); // On met des '\0' partout
    if(cpy == NULL)
        return NULL;

    for(i=0;i<realSize;i++) {
        cpy[realSize-1-i] = bin[i];
    }
    
    free(bin);

    return cpy;
}