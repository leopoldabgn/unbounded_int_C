#include <stdlib.h>
#include <stdio.h>

#include "unbounded_int.h"

static int cmp(int a, int b) {
    if(a < b)
        return -1;
    return a > b;
}

static int calc(int a, char op, int b) {
    switch(op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        default:
            return -1;
    }
}

static void test_unbounded_operations() {
    int val1[] = {-1, 0, 1, 10, -10, 600, 999, -9999};
    int val2[] = {6, 120, -12, 20, -16, -100, 1, -1};
    char ops[] = {'+', '-', '*'};

    int s1 = sizeof(val1) / sizeof(val1[0]);
    int s2 = sizeof(ops) / sizeof(ops[0]);

    unbounded_int a, b, res;

    for(int i=0;i<s1;i++) {
        printf("[LOG] Test #%d :\n\n", i+1);
        a = ll2unbounded_int(val1[i]);
        b = ll2unbounded_int(val2[i]);

        printf("COMPARAISONS : %d et %d\n\n", val1[i], val2[i]);
        printf("unbounded_int : %d\n", unbounded_int_cmp_unbounded_int(a, b));
        printf("          int : %d\n\n", cmp(val1[i], val2[i]));

        for(int j=0;j<s2;j++) {
            printf("CALCUL : %d %c %d\n", val1[i], ops[j], val2[i]);

            res = calculate(a, ops[j], b);
            printf("\nunbounded_int : ");
            print_unbounded_int(res);
            printf("          int : %d\n\n", calc(val1[i], ops[j], val2[i]));

            destroy_unbounded_int(res);
        }

        destroy_unbounded_int(a);
        destroy_unbounded_int(b);
    }

}

int main() {
    printf("\n");
    printf("\t\t [LOG]: Running test_unbounded.c\n\n");
    test_unbounded_operations();
    
    unbounded_int u1 = string2unbounded_int("500045421444");
    unbounded_int u2 = string2unbounded_int("-325154544");
    unbounded_int u3 = ll2unbounded_int(1000);


    printf("[LOG]:Test #1\n");

    printf("unbounded_int u1 = string2unbounded_int(500045421444) --> "); print_unbounded_int(u1);
    printf("unbounded_int u2 = string2unbounded_int(-325154544) --> "); print_unbounded_int(u2);
    printf("unbounded_int u3 = ll2unbounded_int(1000) --> "); print_unbounded_int(u3);


    printf("\n");

    printf("[LOG]:Test #2");

    printf("\nunbounded_int_cmp_unbounded_int(u1, u2) = %d\n", unbounded_int_cmp_unbounded_int(u1, u2));
    printf("unbounded_int_cmp_unbounded_int(u2, u1) = %d\n", unbounded_int_cmp_unbounded_int(u2, u1));

    char* u1_str = unbounded_int2string(u1);
    long long test = 911;

    printf("\n");
    printf("[LOG]: Test #3\n");
    printf("unbounded_cmp between unbounded_int %s and long long %lld = %d\n",
           u1_str, test, unbounded_int_cmp_ll(u1, test));
    free(u1_str);

    printf("\n");

    printf("[LOG]:Test #4\n");
    printf("u1 = "); print_unbounded_int(u1);
    printf("u2 = "); print_unbounded_int(u2);
    
    unbounded_int sub = unbounded_int_difference(u1, u2);
    unbounded_int som = unbounded_int_somme(u1, u2);
    

    // Je lis dans les deux sens pour verifier que tous
    // les liens precedent/suivant, premier/dernier fonctionne.
    printf("Soustraction: "); print_unbounded_int(sub);
    // print_unbounded_int(u3);
    printf("Somme: "); print_unbounded_int(som);
    // print_unbounded_int_left(sub);
    // print_unbounded_int_left(som);

    
    unbounded_int prod = unbounded_int_produit(u1, u2);
    printf("Produit: "); print_unbounded_int(prod);

    destroy_unbounded_int(sub);
    destroy_unbounded_int(som);
    destroy_unbounded_int(u1);
    destroy_unbounded_int(u2);
    destroy_unbounded_int(u3);
    destroy_unbounded_int(prod);
    printf("\n");
    printf("\t\t[LOG]: Finished");

    return EXIT_SUCCESS;
}