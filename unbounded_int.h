#ifndef UNBOUNDED_INT_H
#define UNBOUNDED_INT_H

/* STUCTURES */

typedef struct chiffre {
    struct chiffre *suivant;
    char c;
    struct chiffre *precedent;
} chiffre;

typedef struct {
    char signe;/* soit ’+’ soit ’-’  */
    size_t len;/* longueur de la liste */
    chiffre *premier;/* pointeur vers le premier élément de la liste*/
    chiffre *dernier;/*pointeur vers le dernier élément de la liste*/
} unbounded_int;

/* FONCTIONS */

unbounded_int string2unbounded_int(const char *e);
unbounded_int ll2unbounded_int(long long i);
char *unbounded_int2string(unbounded_int i);
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);
int unbounded_int_cmp_ll(unbounded_int a, long long b);
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);
unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b);
unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b);

void print_unbounded_int(unbounded_int u);
void print_unbounded_int_left(unbounded_int u);
void destroy_unbounded_int(unbounded_int u);
unbounded_int calculate(unbounded_int a, char op, unbounded_int b);
unbounded_int unbounded_int_copy(unbounded_int u);
unbounded_int unbounded_int_dividing_2(unbounded_int a);
char* decimal_to_binary(unbounded_int nb);
unbounded_int binary_to_decimal(char* bin);
int is_valid_uint(unbounded_int nb);
unbounded_int binary_division(char *a, char *b);

#endif