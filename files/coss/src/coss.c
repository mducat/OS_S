
#include <my.h>

#include "coss.h"
#include "instructions.h"

#ifdef __OSS__
#include <lld.h>
#include <oss.h>

#define printf(str, ...) {printf(str __VA_OPT__(,) __VA_ARGS__); refresh();}

#else

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#endif

coss_t *global_coss = 0;

void init_coss_global(void) {
    global_coss = malloc(sizeof(coss_t));
    global_coss->stack_vars = lld_init();
    global_coss->last_func_name = 0;
}

void free_coss_global(void) {
    lld_free(global_coss->stack_vars);
    if (global_coss->last_func_name)
        free(global_coss->last_func_name);
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


lld_t *nextLine() {
    global_coss->current_line = global_coss->current_line->next;
    global_coss->line_id++;
    return global_coss->current_line;
}

int addVarToStack(char *varname) {
    for (lld_t *mv = global_coss->stack_vars->next; mv; mv = mv->next)
        if (!strcmp(((var_t *)mv->data)->name, varname))
            printf("l%i : a variable with this name already exist", global_coss->line_id);

    var_t *var = malloc(sizeof(var_t));
    var->name = strdup(varname);
    lld_insert(global_coss->stack_vars, 0, var);
    return 0;
}

int rmVarFromStack() {
    var_t *var = lld_pop(global_coss->stack_vars, 0);
    free(var->name);
    free(var);
    return 0;
}

int findInStack(char *str) {
    int id = 0;
    for (lld_t *mv = global_coss->stack_vars->next; mv; mv = mv->next, id++) {
        if (!strcmp(((var_t *)mv->data)->name, str))
            return id;
    }
    printf("var %s not found\n", str);
    return -1;
}

brick_t *loadInRax(char *str) {
    char *make2 = 0;
    if (str[0] == '$') {
        int id = findInStack(str);
        char line[] = "mov rax _-0x";
        char line2[] = "\nadd rax rsp\n";
        char *id_str = my_putnbr_base_str(id*8, "0123456789ABCDEF");
        char *make1 = strconcat(line, id_str);
        make2 = strconcat(make1, line2);
        free(make1);
    } else if (str[0] == '_') {
        char line[] = "mov rax ";
        char *make1 = strconcat(line, str);
        make2 = strconcat(make1, "\n");
        free(make1);
    }
    return brickInit(make2);
}

int incScopeDepth() {
    return ++global_coss->scopeDepth;

}

int decScopeDepth() {
    return --global_coss->scopeDepth;
}

brick_t *trigerGenerator(lld_t *mv) {
    int match = 0;
    brick_t *ret = 0;
    line_t *line = mv->data;
    printf("\n");
    printf("%4i| %s",  global_coss->line_id, line->line);
    int spacetoprint = 40-strlen(line->line);
    for (; spacetoprint; spacetoprint--){
        if (global_coss->line_id % 2 || spacetoprint % 3) {
            printf(" ");
        } else {
            printf("-");
        }
    }

    for (int i = 0; global_coss->instructionsSet[i]; i++) {
        instruction_t *inst = global_coss->instructionsSet[i];
        if (inst->match(mv)) {
            ret = inst->generate(mv);
            match++;
        }
    }
    if (match == 0) {
        printf("does not match");
    } else if (match > 1) {
        printf("match %i time!!!", match);
    }
    return ret;
}

void compileScope(lld_t *mv) {
    (void)mv;
    //compileScope
}

void compileFunction(lld_t *mv) {
    (void)mv;
    // compileScope
}

brick_t *compileFile(lld_t *file) {
    global_coss->file = file;
    global_coss->current_line = file->next;
    global_coss->line_id = 0;
    global_coss->scopeDepth = 0;
    brick_t *brick = brickInit(strdup(""));
    for (; global_coss->current_line; nextLine()) {
        lld_t *mv = global_coss->current_line;
        brick_t *func = trigerGenerator(mv);
        brickAdd(brick, func);
        printf("\n");
        printf("code:\n%s\n", func->code);
        brickFree(func);
    }
    return brick;
    // compileFunction
}

int main(int ac, char **av) {

    ucpInit();
    if (ac != 2){
        printf("need a file at first arg\n");
        return 1;
    }
    int error_count = 0;
    (void)error_count;

    #ifdef __OSS__
    file_t *src = open(av[1]);
    
    #else
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
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
    #ifdef __OSS__
    refresh();
    #endif

    brick_t *main_brique = compileFile(file);
    printf("2 ############################ find the instructions\n");
    #ifdef __OSS__
    refresh();
    #endif

    // write code to file
    int file_name_len = 0;
    for (; av[1][file_name_len] != '.' && av[1][file_name_len]; file_name_len++);
    char *destName = malloc(file_name_len+6);
    memcpy(destName, av[1], file_name_len);
    memcpy(destName+file_name_len, ".aoss", 6);

    #ifdef __OSS__
    write_file(destName, main_brique->code, strlen(main_brique->code));
    #else
    FILE *dest = fopen(destName, "wb");
    fwrite(main_brique->code, strlen(main_brique->code), 1, dest);
    fclose(dest);
    #endif
    brickFree(main_brique);
    free(destName);

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
    ucpDestroy();
    return 0;
}