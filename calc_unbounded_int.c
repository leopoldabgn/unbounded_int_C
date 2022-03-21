#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define DEFAULT_OUTPUT output.txt

void help();
int isInputOption(const char* str);
int isOutputOption(const char* str);

int main(int argc, char* argv[]) {
    FILE* source  = NULL,
          *output = NULL;
    // On a au minimum : ./calc_unbounded_int -i src.txt
    if(argc < 2) {
        puts("Il n'y a pas assez d'arguments !");
        help();
        exit(1);
    }
    
    char* arg;
    for(int i=1;i<argc;i+=2) {
        arg = argv[i];
        if(isInputOption(arg)) {
            if(argc == i+1) {
                puts("No file after -i option.");
                help();
                exit(2);
            }
            arg = argv[i+1];
            source = fopen(arg, "r+");
            if(source == NULL) {
                printf("Erreur lors de l'ouverture du fichier : %s\n", arg);
                help();
                exit(3);
            }
        }
        else if(isOutputOption(arg)) {
            if(argc == i+1) {
                puts("No file after -o option.");
                help();
                exit(4);
            }
            arg = argv[i+1];
            // On ecrase le fichier et son contenu si il existe.
            // Sinon, on en cree un nouveau (w).
            output = fopen(arg, "w");
            if(output == NULL) {
                printf("Erreur lors de la creation du fichier : %s\n", arg);
                help();
                exit(5);
            }
        }
        else {
            printf("Argument inconnu : %s\n", arg);
            help();
            exit(-1);
        }
    }
    
    if(output == NULL) {
        output = fopen("output.txt", "w");
        if(output == NULL) {
            printf("Erreur lors de la creation de %s\n", "output.txt");
            exit(6);
        }
    }
    else if(source == NULL) {
        puts("Merci de préciser un fichier source : -i <source>");
        help();
        exit(7);
    }

    fclose(source);
    fclose(output);

    FILE* file = fopen("calc.txt", "r+");
    if(file == NULL)
        return EXIT_FAILURE;
    char c;
    while((c = fgetc(file)) != EOF) {
        printf("%c", c);
    }
    fclose(file);
    return EXIT_SUCCESS;
}

int isInputOption(const char* str) {
    return strcmp(str, "-i") == 0 ? 1 : 0;
}

int isOutputOption(const char* str) {
    return strcmp(str, "-o") == 0 ? 1 : 0;
}

void help() {
    puts("Help :");
    puts("\t./calc_unbounded_int -i <source> -o <output>");
}