#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

#include "unbounded_int.h"

typedef struct variable {
    char* name;
    unbounded_int value;
    struct variable* next;
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
char* getWord(int begin, char* line, char delimiter);
char* readLine(FILE* source);

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

/*
    char str[60];
    char* word;
    while(fgets(str, 60, source) != NULL) {
        word = getWord(str, ' ');
        printf("%s", word);
        free(word);
    }
*/
    char* line, *word;
    int index = 0;
    while(!feof(source)) {
        line = readLine(source);
        if(line == NULL)
            continue;
        while((word = getWord(index, line, ' ')) != NULL) {
            printf("%s ", word);
            index += strlen(word);
            while(line[index] == ' ' && line[index] != '\0')
                index++;
            free(word);
        }
        puts("");
        free(line);
    }

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

variable* create_variable(char* name, unbounded_int val) {
    if(name == NULL || val.signe == '*')
        return NULL;
    variable* var = malloc(sizeof(variable));
    if(var == NULL)
        return NULL;
    var->name = name;
    var->value = val;
    var->next = NULL;
    return var;
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

int isEmpty(list l) {
    return l.first == NULL;
}

int add_variable(list l, char* name, unbounded_int val) {
    variable* var = create_variable(name, val);
    if(var == NULL)
        return 0;
    if(isEmpty(l)) {
        l.first = var;
        l.last = var;
        return 0;
    }
    l.last->next = var;
    l.last = l.last->next;
    return 0;
}

variable* get_variable(list l, char* name) {
    if(name == NULL)
        return NULL;
    variable* tmp = l.first;
    for(;tmp != NULL && strcmp(tmp->name, name) != 0;tmp = tmp->next)
    ;
    return tmp;
}

char* getWord(int begin, char* line, char delimiter) {
    if(line == NULL || begin < 0 || begin >= strlen(line))
        return NULL;
    char* pos = line + begin;
    char* end = strchr(pos, delimiter);
    if(end == NULL)
        end = strchr(pos, '\0');
    ptrdiff_t size = end - pos;
    char* str = malloc((size+1) * sizeof(char));
    memmove(str, pos, size);
    str[size] = '\0';

    return str;
}

char* readLine(FILE* source) {
    if(source == NULL)
        return NULL;
    int i=0, size = 30;
    char* line = malloc(size * sizeof(char));
    char c;
    for(;(c = fgetc(source)) != '\n' && !feof(source);i++) {
        if(i == size-1) { // On double la taille du buffer
            char* doubleLine = realloc(line, size * 2 * sizeof(char)); 
            if(doubleLine == NULL)
                return NULL;
            size *= 2;
            line = doubleLine;
        }
        line[i] = c;
    }

    line[i] = '\0';

    return line;
}