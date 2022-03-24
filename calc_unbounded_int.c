#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void help();
int isInputOption(const char* str);
int isOutputOption(const char* str);

int main(int argc, char* argv[]) {
    FILE* source  = NULL,
          *output = NULL;

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

    if(source == NULL)
        source = stdin;
    if(output == NULL)
        output = stdout;

    char c;
    while((c = fgetc(source)) != EOF)
        printf("%c", c);
    
    fclose(source);
    fclose(output);
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