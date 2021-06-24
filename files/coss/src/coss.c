
#include <my.h>

#include "coss.h"
#include "instructions.h"

#ifdef __OSS__
#include <lld.h>
#include <oss.h>

#else

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#endif


typedef struct variable {
    char *name;
    int stack_pos;
} variable_t;



typedef struct scope {
    int stack_size;
    variable_t *variables;
} scope_t;

typedef struct function {
    int argc; // nb of args to push/pull on stack
    scope_t *scope;
} function_t;

typedef struct instruction {
    char *(*generate)(char **);
    int (*match)(lld_t *mv);
} instruction_t;

typedef struct ligne {
    char *line;
    char **words;
} ligne_t;

typedef unsigned int uint;

void rmDoubledCHar(char *str, char c) {
    int p = 0;
    int i = 0;
    while (str[p] == c)
        p++;
    for (; str[p]; p++) {
        if (str[p] == c && str[p+1] == c)
            continue;
        str[i++] = str[p];
    }
    i--;
    if (i < 0) {
        str[0] = 0;
        return;
    }
    while (str[i] == c) 
        str[i--] = 0;
    str[i+1] = 0;
}



instruction_t *generateInstruction(int (*match)(lld_t *mv), char *(*generate)(char **)) {
    instruction_t *inst = malloc(sizeof(instruction_t));
    inst->match = match;
    inst->generate = generate;
    return inst;
}

instruction_t **instructionsSet = 0;
#define PUSHBACK(lld, data) lld_insert(lld, lld_len(lld), data)
void generateInstructionsSet() {
    lld_t *lld = lld_init();

    PUSHBACK(lld, generateInstruction(&match_nothing, &generate_nothing));
    PUSHBACK(lld, generateInstruction(&match_nothing, &generate_nothing));
    PUSHBACK(lld, generateInstruction(&match_nothing, &generate_nothing));

    PUSHBACK(lld, generateInstruction(&match_nothing, &generate_nothing));
    PUSHBACK(lld, generateInstruction(&match_nothing, &generate_nothing));
    PUSHBACK(lld, generateInstruction(&match_nothing, &generate_nothing));
    
    PUSHBACK(lld, generateInstruction(&match_nothing, &generate_nothing));
    PUSHBACK(lld, generateInstruction(&match_nothing, &generate_nothing));
    PUSHBACK(lld, generateInstruction(&match_nothing, &generate_nothing));

    PUSHBACK(lld, generateInstruction(&match_nothing, &generate_nothing));
    PUSHBACK(lld, generateInstruction(&match_nothing, &generate_nothing));
    PUSHBACK(lld, generateInstruction(&match_nothing, &generate_nothing));

    instructionsSet = (instruction_t **)lld_lld_to_tab(lld);
    lld_free(lld);
}

void freeInstructionsSet() {
    for (int i = 0; instructionsSet[i]; i++) {
        free(instructionsSet[i]);
    }
    free(instructionsSet);
}

int main(int ac, char **av) {

    if (ac != 2){
        printf("need a file at first arg\n");
        return 1;
    }
    int error_count = 0;
    (void)error_count;

    #ifdef __OSS__
    file_t *src = open(av[1]);
    
    #else
    FILE *src = fopen(av[1], "r+");
    
    #endif
    if (!src){
        printf("cannot open: %s\n", av[1]);
        return 1;
    }
    lld_t *file = lld_init();
    
    #ifdef __OSS__
    char *fileBuf = malloc(src->size+1);
    memcpy(fileBuf, src->content, src->size);
    fileBuf[src->size] = 0;
    char **file_split = strToWords(fileBuf, '\n');
    free(fileBuf);
    for (int i = 0; file_split[i]; i++) {
        lld_insert(file, lld_len(file), file_split[i]);
    }
    free(file_split);
    #else
    size_t line_len = 0;
    int line_len2 = 0;
    char *line = 0;
    while ((line_len2 = getline(&line, &line_len, src)) > 0){
        if (!line)
            continue;
        if (line[line_len2-1] == '\n') 
            line[line_len2-1] = 0;
        lld_insert(file, lld_len(file), line);
        line = 0;
    }
    free(line);
    fclose(src);
    #endif

    generateInstructionsSet();

    {
        long int line_count = 0;
        lld_t *to_pop = lld_init();
        for (lld_t *mv = file->next; mv; mv = mv->next, line_count++) {
            rmDoubledCHar(mv->data, ' ');
            printf("%4li| %s\n", line_count, (char *)mv->data);
            char **words = strToWords(mv->data, ' ');
            free(mv->data);
            mv->data = words;
            // spot useless lines
            if (!words[0] || !words[0][0] || !strcmp(words[0], "//")) {
                lld_insert(to_pop, 0, (void *)line_count);
            } else  {
            }
        }

        // remove useless lines
        for (lld_t *mv = to_pop->next; mv; mv = mv->next) {
            char **words = lld_pop(file, (long int)mv->data);
            for (int i = 0; words[i]; i++)
                free(words[i]);
            free(words);
        }
        lld_free(to_pop);
    }
    printf("1 ############################ clear the text\n");

    {
        long int line_count = 0;
        for (lld_t *mv = file->next; mv; mv = mv->next, line_count++) {
            char **words = mv->data;
                printf("%4li|", line_count);
            for (int i = 0; words[i]; i++) 
                printf(" %s", words[i]);




            printf("\n");
        }

    }
    printf("2 ############################ find the instructions\n");


    // write code to file


    // free everytyhing
    for (lld_t *mv = file->next; mv; mv = mv->next) {
        char **data = mv->data;
        for (int i = 0; data[i]; i++)
            free(data[i]);
        free(data);
    }
    lld_free(file);
    freeInstructionsSet();
    return 0;
}