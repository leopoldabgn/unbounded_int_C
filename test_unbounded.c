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

// On teste la plupart des fonctions ici
static void test_unbounded_operations() {
    int val1[] = {-1, 0, 1, 10, -10, 600, 999, -9999};
    int val2[] = {6, 120, -12, 20, -16, -100, 1, -1};
    char ops[] = {'+', '-', '*'};

    int s1 = sizeof(val1) / sizeof(val1[0]);
    int s2 = sizeof(ops) / sizeof(ops[0]);

    unbounded_int a, b, res;
    int res2;
    char* buffer;

    for(int i=0;i<s1;i++) {
        printf("[LOG] Test #%d :\n\n", i+1);
        a = ll2unbounded_int(val1[i]);
        b = ll2unbounded_int(val2[i]);

        printf("COMPARAISONS : %d et %d\n\n", val1[i], val2[i]);
        printf("unbounded_int : %d  %d\n", unbounded_int_cmp_unbounded_int(a, b), unbounded_int_cmp_ll(a, val2[i]));
        printf("          int : %d\n\n", cmp(val1[i], val2[i]));

        for(int j=0;j<s2;j++) {
            printf("CALCUL : %d %c %d\n", val1[i], ops[j], val2[i]);

            res = calculate(a, ops[j], b);
            printf("\nunbounded_int : ");
            buffer = unbounded_int2string(res);
            printf("%s\n", buffer);
            free(buffer);
            res2 = calc(val1[i], ops[j], val2[i]);
            printf("          int : %d\n\n", res2);

            if(unbounded_int_cmp_ll(res, res2) == 0)
                puts("----- TEST [OK] -----\n");
            else
                puts("----- TEST [ERREUR] -----\n");
            
            destroy_unbounded_int(res);
        }

        destroy_unbounded_int(a);
        destroy_unbounded_int(b);
    }
    char* str1[] = {"1000000000000000000000000000000000000000000000000000000000000000000000000000000",
                    "9999999999999999999999999999999999999999999999999999999999999999999999999999999"};
    char* str2[] = {"9000000000000000000000000000000000000000000000000000000000000000000000000000000",
                    "1"};
    s1 = sizeof(str1) / sizeof(str1[0]);

    for(int i=0;i<s1;i++) {
        a = string2unbounded_int(str1[i]);
        b = string2unbounded_int(str2[i]);
        for(int j=0;j<s2;j++) {
            printf("CALCUL :\n%s\n%c\n%s\n", str1[i], ops[j], str2[i]);
            res = calculate(a, ops[j], b);
            printf("\nunbounded_int :\n");
            buffer = unbounded_int2string(res);
            printf("%s\n\n", buffer);
            free(buffer);
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

    unbounded_int a = string2unbounded_int("-99901");
    unbounded_int b = unbounded_int_dividing_2(a);

    print_unbounded_int(a);
    print_unbounded_int_left(a);
    print_unbounded_int(b);
    print_unbounded_int_left(b);

    destroy_unbounded_int(a);
    destroy_unbounded_int(b);
    
    //////////
    // Ces operations coutent beaucoup de memoire

    unbounded_int nb = string2unbounded_int("1999991");
    char* res = decimal_to_binary(nb);

    printf("res = %s\n", res);

    printf("\nBinary to Decimal: ");
    unbounded_int z = binary_to_decimal(res);
    print_unbounded_int(z);
    printf("\n");

    printf("isvalid = %d\n", is_valid_uint(nb)); // 1

    free(res);
    destroy_unbounded_int(nb);
    destroy_unbounded_int(z);

    // printf("\n\n");
    // char * bin_res = binary_division("100110", "10");
    // unbounded_int result_div = binary_to_decimal(bin_res);

    // print_unbounded_int(result_div);


    unbounded_int int_a = string2unbounded_int("257");
    unbounded_int int_b = string2unbounded_int("2");
    unbounded_int int_res = unbounded_int_division(a, b);


    printf("\nDIV TEST: ");
    print_unbounded_int(int_res);
    unbounded_int int_res_2 = unbounded_int_modulo(a, b);
    printf("\nMOD TEST: ");
    print_unbounded_int(int_res_2);

    destroy_unbounded_int(int_a);
    destroy_unbounded_int(int_b);
    destroy_unbounded_int(int_res);
    destroy_unbounded_int(int_res_2);

    unbounded_int x = {.signe='*'};
    unbounded_int y = {.signe='+'}; // Mais il faut aussi que len > 0 !
    printf("isvalid = %d\n", is_valid_uint(x)); // 0
    printf("isvalid = %d\n", is_valid_uint(y)); // 0

    printf("\n");
    printf("\t\t[LOG]: Finished\n");

    return EXIT_SUCCESS;
}