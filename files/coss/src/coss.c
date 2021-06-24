
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

coss_t *global_coss = 0;

void init_coss_global(void) {
    global_coss = malloc(sizeof(coss_t));
    global_coss->stack_vars = lld_init();
}

void free_coss_global(void) {
    lld_free(global_coss->stack_vars);
    free(global_coss);   
}

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

instruction_t *generateInstruction(int (*match)(lld_t *mv), brick_t *(*generate)(lld_t *mv)) {
    instruction_t *inst = malloc(sizeof(instruction_t));
    inst->match = match;
    inst->generate = generate;
    return inst;
}

#define PUSHBACK(lld, data) lld_insert(lld, lld_len(lld), data)

void generateInstructionsSet() {
    lld_t *lld = lld_init();
    PUSHBACK(lld, generateInstruction(&matchFunction, &generateFunction));
    PUSHBACK(lld, generateInstruction(&matchScope, &generateScope));
    PUSHBACK(lld, generateInstruction(&matchOperator_e, &generateOperator_e));
    PUSHBACK(lld, generateInstruction(&matchOperator_e_operator, &generateOperator_e_operator));
    PUSHBACK(lld, generateInstruction(&matchEndOfScope, &generateEndOfScope));
    PUSHBACK(lld, generateInstruction(&matchReturn, &generateReturn));
    PUSHBACK(lld, generateInstruction(&matchCall, &generateCall));


    global_coss->instructionsSet = (instruction_t **)lld_lld_to_tab(lld);
    lld_free(lld);
}

void freeInstructionsSet() {
    for (int i = 0; global_coss->instructionsSet[i]; i++) {
        free(global_coss->instructionsSet[i]);
    }
    free(global_coss->instructionsSet);
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

    init_coss_global();
    generateInstructionsSet();


    {
        long int line_count = 0;
        lld_t *to_pop = lld_init();
        for (lld_t *mv = file->next; mv; mv = mv->next, line_count++) {
            rmDoubledCHar(mv->data, ' ');
            printf("%4li| %s\n", line_count, (char *)mv->data);
            line_t *line = malloc(sizeof(line_t));
            line->words = strToWords(mv->data, ' ');
            line->line = mv->data;
            mv->data = line;
            // spot useless lines
            if (!line->words[0] || !line->words[0][0] || !strcmp(line->words[0], "//")) {
                lld_insert(to_pop, 0, (void *)line_count);
            } else  {
            }
        }

        // remove useless lines
        for (lld_t *mv = to_pop->next; mv; mv = mv->next) {
            line_t *line = lld_pop(file, (long int)mv->data);
            for (int i = 0; line->words[i]; i++)
                free(line->words[i]);
            free(line->words);
            free(line->line);
            free(line);
        }
        lld_free(to_pop);
    }
    printf("1 ############################ clear the text\n");

    {
        long int line_count = 0;
        for (lld_t *mv = file->next; mv; mv = mv->next, line_count++) {
            line_t *line = mv->data;
            printf("%4li| %s", line_count, line->line);

            int spacetoprint = 40-strlen(line->line);
            for (; spacetoprint; spacetoprint--){
                if (line_count%2 || spacetoprint % 3)
                    printf(" ");
                else
                    printf("-");
            }

            int match = 0;
            for (int i = 0; global_coss->instructionsSet[i]; i++) {
                instruction_t *inst = global_coss->instructionsSet[i];
                if (inst->match(mv)) {
                    inst->generate(mv);
                    match++;
                }
            }
            if (match == 0) {
                printf("does not match");
            } else if (match > 1) {
                printf("match %i time!!!", match);
            }


            printf("\n");
        }

    }
    printf("2 ############################ find the instructions\n");


    // write code to file


    // free everytyhing
    for (lld_t *mv = file->next; mv; mv = mv->next) {
        line_t *data = mv->data;
        for (int i = 0; data->words[i]; i++)
            free(data->words[i]);
        free(data->words);
        free(data->line);
        free(data);
    }
    lld_free(file);
    freeInstructionsSet();
    free_coss_global();
    return 0;
}