#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "unbounded_int.h"

typedef struct variable {
    char* name;
    unbounded_int value;
    struct variable* next;
    struct variable* previous;
}variable;

typedef struct {
    variable* first;
    variable* last;
}list;

static void help();
static int isInputOption(const char* str);
static int isOutputOption(const char* str);
void destroy_list(list l);
void destroy_variable(variable* var);

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

static int isInputOption(const char* str) {
    return strcmp(str, "-i") == 0 ? 1 : 0;
}

static int isOutputOption(const char* str) {
    return strcmp(str, "-o") == 0 ? 1 : 0;
}

static void help() {
    puts("Help :");
    puts("\t./calc_unbounded_int -i <source> -o <output>");
}

void destroy_variable(variable* var) {
    if(var == NULL)
        return;
    free(var->name);
    destroy_unbounded_int(var->value);
    free(var);
}

void destroy_list(list l) {
    variable* tmp = l.first;
    for(variable* var=l.first;var != NULL;var=tmp) {
        tmp = tmp->next;
        free(var);
    }
}
