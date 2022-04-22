#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <assert.h>

#include "unbounded_int.h"

typedef struct variable {
    char* name;
    unbounded_int value;
    struct variable* next;
}variable;

typedef struct {
    variable* first;
}list;

static void help();
static int isInputOption(const char* str);
static int isOutputOption(const char* str);
void destroy_list(list* l);
void destroy_variable(variable* var);
variable* create_variable(char* name, unbounded_int val);
variable* get_variable(list* l, char* name);
variable* add_variable(list* l, char* name, char* val);
int affect_var(variable* v, char* str, list* l);
char* nextWord(int begin, char* line, char delimiter);
char* readLine(FILE* source);
static int indexOf(const char* str, char elt);
static unbounded_int compute_with_variables(char* str, list* l);
static unbounded_int get_unbounded(char* x, list *l);
int affect_var2(variable* v, char* str, list* l);
static int word_counter(char* str);
// static char* get_words(char* s);

/*
////////////////////////////////////////////////////////////////////
ftell(SEEK_END)
et fseek pour recuperer directement le nombre d'octet d'un fichier
et donc recuperer sa taille.
On peut alors allouer directement un buffer à la bonne taille.
On pourrait donc rempalcer la methode ou je lis au fur et a mesure
le fichier et ou je fais des realloc de temps en temps
////////////////////////////////////////////////////////////////////

*/

int main(int argc, char* argv[]) {

    // char* string = "5 - 5";
    // unbounded_int x = compute_with_variables(string, NULL);
    // print_unbounded_int(x);

    // printf("\n");

    // char* string2 = "5 * 5";
    // unbounded_int y = compute_with_variables(string2, NULL);
    // print_unbounded_int(y);

    // printf("\n");

    // char* string3 = "5 + 5";
    // unbounded_int z = compute_with_variables(string3, NULL);
    // print_unbounded_int(z);

    // char* str = "This is a word counter";
    // int a = word_counter(str);
    // printf("a =  %d", a);

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

    if(source == NULL) {
        source = stdin;
        puts("(Press Ctrl+D to stop the program)");
    }
    if(output == NULL)
        output = stdout;

    list l = {.first = NULL};
    char* line, *word = NULL, *lastWord = NULL, *uStr;
    variable* var;
    while(!feof(source)) {
        line = readLine(source);
        if(line == NULL)
            continue;
        for(int index=0;(word = nextWord(index, line, ' ')) != NULL;) {
            // PRINT
            if(lastWord != NULL && strcmp(lastWord, "print") == 0) {
                var = get_variable(&l, word);
                if(var != NULL) {
                    uStr = unbounded_int2string(var->value);
                    fprintf(output, "%s -> %s\n", var->name, uStr);
                    free(uStr);
                }
                else {
                    printf("ERROR: cannot print variable\n");
                    return EXIT_FAILURE;
                }
            }
            else {
                // On verifie si word contient un egal '='
                // Si c'est le cas, on recupere l'index du egal
                int equalIndex = indexOf(word, '=');
                if(equalIndex != -1) {
                    // Dans ce cas, on un '=' au debut de word.
                    // suivi d'une affection, colle ou non au symbole egal
                    // ex: "=-7823", "=  9876", "=   a * 88"
                    if(equalIndex == 0 && lastWord != NULL) { // La variable a affecter est normalement deja stocke dans lastWord
                        var = get_variable(&l, lastWord);
                        if(var == NULL)
                            var = add_variable(&l, lastWord, "0");
                    }
                    // Dans ce cas, on a :
                    // - une variable
                    // - Puis un '=' colle a la variable
                    // Par exemple : "a=", "abab=  8" ou encore "var=   b   * c"
                    else {
                        // On place temporairement un '\0' a la place du egal
                        word[equalIndex] = '\0';
                        var = get_variable(&l, word);
                        if(var == NULL)
                            var = add_variable(&l, word, "0");
                        // On replace le '='
                        word[equalIndex] = '=';
                    }
                    affect_var2(var, (line+index)+equalIndex+1, &l); // line+index+equalIndex+1 car on passe ce qu'il y a apres le egal

                    free(word); // lastWord sera free (si besoin) a la sortie du for.
                    break;
                }
            }

            // On se decalle pour recuperer le prochain mot.
            index += strlen(word);
            while(line[index] == ' ' && line[index] != '\0')
                index++;

            if(lastWord != NULL)
                free(lastWord);

            lastWord = word;
        }
        if(lastWord != NULL) {
            free(lastWord);
            lastWord = NULL; // Important. Peut faire bugger le prgm au prochain tour de boucle
        }
        //puts("");
        free(line);
    }
    
    destroy_list(&l);

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

// str est la chaine apres le '='
int affect_var(variable* v, char* str, list* l) {
    if(str == NULL || *str == '\0' || v == NULL)
        return 1;
    int index = 0;
    while(str[index] == ' ' && str[index] != '\0')
        index++;
    char* val = nextWord(index, str, ' '); // ' ' ou '\0'
    if(val == NULL)
        return 1;
    unbounded_int u = string2unbounded_int(val);
    free(val);
    if(u.signe == '*') // Si il y a un probleme a la creation de l'unbounded_int
        return 1;
    // On free l'ancien unbounded_int relie a la variable v
    destroy_unbounded_int(v->value);
    // On set la nouvelle valeur de v->value
    v->value = u;
    return 0;
}


int affect_var2(variable* v, char* str, list* l) {
    if(str == NULL || *str == '\0' || v == NULL)
        return 1;
    if(word_counter(str) == 1) {
        // printf("*");
        int index = 0;
        while(str[index] == ' ' && str[index] != '\0')
            index++;
        char* val = nextWord(index, str, ' '); // ' ' ou '\0'
        if(val == NULL)
            return 1;
        if(strcmp(val, v->name) == 0) {
            free(val);
            return 0;
        }
        unbounded_int u = get_unbounded(val, l);
        free(val);
        if(u.signe == '*') // Si il y a un probleme a la creation de l'unbounded_int
            return 1;
        // On free l'ancien unbounded_int relie a la variable v
        destroy_unbounded_int(v->value);
        // On set la nouvelle valeur de v->value
        v->value = u;
    }else {
        // printf("**");
        unbounded_int res = compute_with_variables(str, l);
        destroy_unbounded_int(v->value);
        v->value = res;
    }
    return 0;
}

// static char* get_words(char* s) {
//     int len = word_counter(s);

//     char* only_words = malloc(sizeof(char) * (len + 1));
//     int len_only_words = strlen(only_words);

//     assert(only_words != NULL);
//     char delim[] = " ";

//     char* a = strtok(only_words, delim);
//     int len_counter = 0;

//     while (a != NULL && len_counter < len_only_words) {
//         only_words[len_counter] = a;
//         a = strtok(NULL, delim);
//     }
//     free(only_words);
//     return only_words;
// }

static int word_counter(char* str) {
    int size = strlen(str);
    char* copy_str = malloc(sizeof(char) * (size + 1));
    int counter = 0;

    assert(copy_str != NULL);
    strcpy(copy_str, str);

    char delim[] = " ";

    char* a = strtok(copy_str, delim);

    while (a != NULL) {
        counter++;
        a = strtok(NULL, delim);
    }
    free(copy_str);
    return counter;
}


static unbounded_int compute_with_variables(char* str, list* l) {
/*  

    @param str: a char pointer. 
    @return unbounded_int: result from an arithmetic operation between two numbers.

    This function compute the result of an arithmetic operation between two variables.
    It reads all the words separeted by the delimeter. We expect to find something of the type: 7
    5+5, 5-5,  5*5 or the same
    but using variables

*/

    int size = strlen(str);
    char* copy_str = malloc(sizeof(char) * (size + 1));

    assert(copy_str != NULL);
    strcpy(copy_str, str);

    char delim[] = " ";

    char* a = strtok(copy_str, delim);
    char signe = strtok(NULL, delim)[0];
    char* b = strtok(NULL, delim);
    // printf("%s %c %s = ", a, signe, b);

    unbounded_int n1 = get_unbounded(a, l);
    unbounded_int n2 = get_unbounded(b, l);
    unbounded_int res;

    // printf("-----------------\n");
    // print_unbounded_int(n1);
    // print_unbounded_int(n2);
    // printf("-----------------\n");

    res = calculate(n1, signe, n2);
    // print_unbounded_int(res);
    free(copy_str);

    return res;
}

static unbounded_int get_unbounded(char* x, list* l) {
    unbounded_int n1;
    char c = x[0];
    
    if(isalpha(c) && l!= NULL) {
        variable *z = get_variable(l, x);
        n1 = unbounded_int_copy(z->value);
    }else{
        n1 = string2unbounded_int(x);
    }

    return n1;
}

variable* create_variable(char* name, unbounded_int val) {
    if(name == NULL || val.signe == '*')
        return NULL;
    variable* var = malloc(sizeof(variable));
    if(var == NULL)
        return NULL;
    char* n = malloc(sizeof(char)*(strlen(name)+1));
    if(n == NULL) {
        free(var);
        return NULL;
    }
    strcpy(n, name); // strcpy copie egalement le caractere null '\0'
    var->name = n;
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

void destroy_list(list* l) {
    variable* tmp = l->first;
    for(variable* var=l->first;var != NULL;var=tmp) {
        tmp = tmp->next;
        destroy_variable(var);
    }
}

int isEmpty(list* l) {
    return l->first == NULL;
}

variable* add_variable(list* l, char* name, char* val) {
    if(l == NULL || name == NULL || val == NULL)
        return NULL;
    unbounded_int u = string2unbounded_int(val);
    variable* var = create_variable(name, u);
    if(var == NULL) {
        destroy_unbounded_int(u);
        return NULL;
    }
    if(isEmpty(l)) {
        l->first = var;
        return var;
    }
    var->next = l->first;
    l->first = var;
    return var;
}

variable* get_variable(list* l, char* name) {
    if(name == NULL)
        return NULL;
    variable* tmp = l->first;
    for(;tmp != NULL && strcmp(tmp->name, name) != 0;tmp = tmp->next)
    ;
    return tmp;
}

char* nextWord(int begin, char* line, char delimiter) {
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

static int indexOf(const char* str, char elt) {
    if(str == NULL)
        return -1;
    int index = 0;
    char* ptr = (char*)str;
    for(;*ptr != elt && *ptr != '\0';ptr++, index++)
        ;
    
    return *ptr == '\0' ? -1 : index;
}