#include <stdlib.h>
#include <stdio.h>

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
    unbounded_int u1 = string2unbounded_int("-10");
    unbounded_int u2 = string2unbounded_int("0");
    unbounded_int u3 = ll2unbounded_int(1000);

    printf("unbounded_int_cmp_unbounded_int(u1, u2) = %d\n", unbounded_int_cmp_unbounded_int(u1, u2));
    printf("unbounded_int_cmp_unbounded_int(u2, u1) = %d\n", unbounded_int_cmp_unbounded_int(u2, u1));

    char* u1_str = unbounded_int2string(u1);
    long long test = 911;
    printf("unbounded_cmp between unbounded_int %s and long long %lld = %d\n",
           u1_str, test, unbounded_int_cmp_ll(u1, test));
    free(u1_str);

    printf("\n");
    print_unbounded_int(u1);
    print_unbounded_int(u2);
    
    unbounded_int sub = unbounded_int_difference(u1, u2);
    unbounded_int som = unbounded_int_somme(u1, u2);

    // Je lis dans les deux sens pour verifier que tous
    // les liens precedent/suivant, premier/dernier fonctionne.
    printf("Soustraction: ");
    print_unbounded_int(sub);
    print_unbounded_int(u3);
    printf("Somme: ");
    print_unbounded_int(som);
    print_unbounded_int_left(sub);
    print_unbounded_int_left(som);

    destroy_unbounded_int(sub);
    destroy_unbounded_int(som);
    destroy_unbounded_int(u1);
    destroy_unbounded_int(u2);
    destroy_unbounded_int(u3);

    return EXIT_SUCCESS;
}