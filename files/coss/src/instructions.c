#include <my.h>

#include "instructions.h"
#include "coss.h"

#ifdef __OSS__
#include <lld.h>
#include <oss.h>

#define printf(str, ...) {printf(str __VA_OPT__(,) __VA_ARGS__); refresh();}

#else

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#endif



brick_t *concatBrick(brick_t *a, brick_t *b) {
    brick_t *ret = malloc(sizeof(brick_t));
    ret->code = strconcat(a->code, b->code);
    return ret;
}

void brickAdd(brick_t *a, brick_t *b) {
    char *str = a->code;
    a->code = strconcat(str, b->code);
    free(str);
}

void brickFree(brick_t *a) {
    free(a->code);
    free(a);
}

brick_t *brickInit(char *str) {
    brick_t *ret = malloc(sizeof(brick_t));
    ret->code = str;
    return ret;
}

int match_nothing(lld_t *mv) {
    (void)mv;
    return 0;
}

char *generate_nothing(lld_t *mv) {
    (void)mv;
    return 0;
}


// is alpha (A..Z || a..z)
int ucp_alpha(char *str) {
    for (int i = 0; str[i]; i++) {
        if (!(('a' <= str[i] && str[i] <= 'z') || ('A' <= str[i] && str[i] <= 'Z') || ('0' <= str[i] && str[i] <= '9')))
            return 0;
    }
    return 1;
}

// is _nb
int ucp_const(char *str) {
    if (str[0] != '_')
        return 0;
    for (int i = 1; str[i]; i++) {
        if (!(('a' <= str[i] && str[i] <= 'z') || ('A' <= str[i] && str[i] <= 'Z') || ('0' <= str[i] && str[i] <= '9') || str[i] == '-' || str[i] == '+'))
            return 0;
    }
    return 1;
}

// is $var
int ucp_var(char *str) {
    if (str[0] != '$')
        return 0;
    for (int i = 1; str[i]; i++) {
        if (!(('a' <= str[i] && str[i] <= 'z') || ('A' <= str[i] && str[i] <= 'Z') || ('0' <= str[i] && str[i] <= '9')))
            return 0;
    }
    return 1;
}

// is multiple ($var) separate ' '
int ucp_vars(char *str) {
    char **words = strToWords(str, ' ');
    int ret = 1;
    for (int i = 0; words[i]; i++) {
        if (!ucp_var(words[i])) {
            ret = 0;
            break;
        }
    }
    for (int i = 0; words[i]; i++)
        free(words[i]);
    free(words);
    return ret;
}

// is _nb or $var
int ucp_arg(char *str) {
    return ucp_var(str) || ucp_const(str);
}

// is an handled operator

int ucp_operator(char *str) {
    if (strlen(str) != 1)
        return 0;
    char operators[] = "*+-";
    for (int i = 0; operators[i]; i++) {
        if (operators[i] == str[0])
            return 1;
    }
    return 0;
}

// is multiple (_nb or $var) separate ' '
int ucp_args(char *str) {   
    char **words = strToWords(str, ' ');

    int ret = 1;
    for (int i = 0; words[i]; i++) {
        if (!ucp_arg(words[i])) {
            ret = 0;
            break;
        }
    }

    for (int i = 0; words[i]; i++)
        free(words[i]);
    free(words);
    return ret;
}

int ucp_spaces(char *str) {
    for (int i = 0; str[i]; i++)    
        if (str[i] != ' ')
            return 0;
    return 1;
}

int matchOperator_e_operator(lld_t *mv) {
    line_t *line = mv->data;
    return ucp(line->line, "* = * * *", ucp_var, ucp_arg, ucp_operator, ucp_arg) == 1;
}

brick_t *generateOperator_e_operator(lld_t *mv) {
    line_t *line = mv->data;
    char ***tab = ucp_tab(line->line, "* = * * *", ucp_var, ucp_arg, ucp_operator, ucp_arg);
    printf("%s = operator%s(%s, %s)", tab[0][0], tab[0][2], tab[0][1], tab[0][3]);

    ucp_free(tab);
    return brickInit(strdup(""));
}

int matchOperator_e(lld_t *mv) {
    line_t *line = mv->data;
    return ucp(line->line, "* = *", ucp_var, ucp_arg) == 1;
}

brick_t *generateOperator_e(lld_t *mv) {
    line_t *line = mv->data;
    char ***tab = ucp_tab(line->line, "* = *", ucp_var, ucp_arg);
    printf("%s = %s", tab[0][0], tab[0][1]);
    
    int var_id = findInStack(tab[0][0]);
    
    brick_t *load_in_rax = loadInRax(tab[0][1]);
    char *tmpstr1 = strconcat("mov rcx rsp\nmov rcx _-0x", my_putnbr_base_str(var_id*8, "0123456789ABCDEF"));
    char *tmpstr2 = strconcat(tmpstr1, " rax\n");
    free(tmpstr1);
    brick_t *br = brickInit(tmpstr2);
    brickAdd(load_in_rax, br);
    brickFree(br);
    ucp_free(tab);
    return load_in_rax;
}



int matchFunction(lld_t *mv) {
    line_t *line = mv->data;
    return ucp(line->line, "def *(*)", &ucp_alpha, &ucp_args) == 1;
}

brick_t *generateFunction(lld_t *mv) {
    line_t *line = mv->data;
    char ***tab = ucp_tab(line->line, "def *(*)", ucp_alpha, ucp_args);
    char **args = strToWords(tab[0][1], ' ');
    printf("%s(%s)", tab[0][0], tab[0][1]);
    int len = strlen(tab[0][0])+3;
    char *balise = malloc(len);
    balise[0] = '!';
    memcpy(balise+1, tab[0][0], strlen(tab[0][0]));
    balise[len-2] = '\n';
    balise[len-1] = 0;
    
    if (global_coss->last_func_name)
        free(global_coss->last_func_name);
    global_coss->last_func_name = strdup(tab[0][0]);

    brick_t *brick = brickInit(balise);
    // push vars on the stack and lld
    int depth = 0;
    for (int i = 0; args[i]; i++, depth++) {
        addVarToStack(args[i]);
        brick_t *var = brickInit(strdup("push _0\n"));
        brickAdd(brick, var);
        brickFree(var);
    }


    brick_t *bri = trigerGenerator(nextLine());
    brickAdd(brick, bri);
    brickFree(bri);

    for (int i = 0; i < depth; i++)
        rmVarFromStack();
    char ins[] = "add rsp _-0x";
    char *nb_str = my_putnbr_base_str(depth*8, "0123456789ABCDEF");
    char *tmpstr = strconcat(ins, nb_str);
    char *tmpstr2 = strconcat(tmpstr, "\nret\n");
    free(tmpstr);
    brick_t *var = brickInit(tmpstr2);
    brickAdd(brick, var);
    brickFree(var);

    //end:;
    for (int i = 0; args[i]; i++)
        free(args[i]);
    free(args);
    ucp_free(tab);
    return brick;
}

int matchScope(lld_t *mv) {
    line_t *line = mv->data;
    return ucp(line->line, "{**", ucp_spaces, ucp_vars) == 1;
}

brick_t *generateScope(lld_t *mv) {
    line_t *line = mv->data;
    char ***tab = ucp_tab(line->line, "{**", ucp_spaces, ucp_vars);
    char **vars = strToWords(tab[0][1], ' ');
    printf("%s", tab[0][1]);
    printf("->");
    int depth = incScopeDepth();
    
    brick_t *brick = brickInit(strdup("// creating a scope\n"));
    // push vars on the stack and lld
    int var_count = 0;
    for (int i = 0; vars[i]; i++, var_count++) {
        addVarToStack(vars[i]);
        brick_t *var = brickInit(strdup("push _0\n"));
        brickAdd(brick, var);
        brickFree(var);
    }

    while (depth == global_coss->scopeDepth) {
        brick_t *bri = trigerGenerator(nextLine());
        if (bri) {
            brickAdd(brick, bri);
            brickFree(bri);
        }
    }

    // pop vars on the stack and lld
    for (int i = 0; i < var_count; i++)
        rmVarFromStack();
    
    brick_t *var;
    if (var_count) {
        char mvo[] = "// deleting scope\nmov rcx _-0x";
        char *nb_str = my_putnbr_base_str(var_count*8, "0123456789ABCDEF");
        char *tmpstr = strconcat(mvo, nb_str);
        char *tmpstr2 = strconcat(tmpstr, "\nadd rsp rcx\nret\n");
        free(tmpstr);
        var = brickInit(tmpstr2);
    } else {
        var = brickInit(strdup("// deleting scope\n"));
    }

    brickAdd(brick, var);
    brickFree(var);
    printf("<-");
    //free
    //end:;
    for (int i = 0; vars[i]; i++)
        free(vars[i]);
    free(vars);
    ucp_free(tab);
    return brick;
}

int matchEndOfScope(lld_t *mv) {
    line_t *line = mv->data;
    return ucp(line->line, "}") == 1;   
}

brick_t *generateEndOfScope(lld_t *mv) {
    (void)mv;
    decScopeDepth();
    return brickInit(strdup(""));
}

int matchReturn(lld_t *mv) {
    line_t *line = mv->data;
    return ucp(line->line, "return *", ucp_arg) == 1;
}

brick_t *generateReturn(lld_t *mv) {
    line_t *line = mv->data;
    char ***tab = ucp_tab(line->line, "return *", ucp_arg);
    printf("%s", tab[0][0]);
    int depth = lld_len(global_coss->stack_vars);

    brick_t *brick1 = loadInRax(tab[0][0]);    

    char mvo[] = "// return\nmov rcx _-0x";
    char *nb_str = my_putnbr_base_str(depth*8, "0123456789ABCDEF");
    char *tmpstr = strconcat(mvo, nb_str);
    char *tmpstr2 = strconcat(tmpstr, "\nadd rsp rcx\nret\n");
    free(tmpstr);
    brick_t *brick = brickInit(tmpstr2);
    brickAdd(brick1, brick);
    brickFree(brick);

    ucp_free(tab);
    return brick1;
}

int matchCall(lld_t *mv) {
    line_t *line = mv->data;
    return ucp(line->line, "* = *(*)", ucp_var, ucp_alpha, ucp_args) == 1;
}

brick_t *generateCall(lld_t *mv) {
    line_t *line = mv->data;
    char ***tab = ucp_tab(line->line, "* = *(*)", ucp_var, ucp_alpha, ucp_args);
    printf("%s = %s(%s)", tab[0][0], tab[0][1], tab[0][2]);

    ucp_free(tab);
    return brickInit(strdup(""));
}