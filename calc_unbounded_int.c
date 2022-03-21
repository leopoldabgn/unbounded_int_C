#include <stdlib.h>
#include <stdio.h>

void help();

int main(int argc, char* argv[]) {
    if(argc < 5) {
        puts("Il n'y a pas assez d'arguments !");
        help();
        exit(1);
    }
    FILE* file = fopen("calc.txt", "r+");
    if(file == NULL)
        return EXIT_FAILURE;
    char c;
    while((c = fgetc(file)) != EOF) {
        printf("%c", c);
    }
    return EXIT_SUCCESS;
}

void help() {
    puts("Help :");
    puts("./calc_unbounded_int -i <source> -o <output>");
}