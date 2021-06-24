#include <my.h>

#include "instructions.h"

#ifdef __OSS__
#include <lld.h>
#include <oss.h>

#else

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#endif


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
        if (!(('a' <= str[i] && str[i] <= 'z') || ('A' <= str[i] && str[i] <= 'Z') || ('0' <= str[i] && str[i] <= '9')))
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
    return 0;
}

int matchOperator_e(lld_t *mv) {
    line_t *line = mv->data;
    return ucp(line->line, "* = *", ucp_var, ucp_arg) == 1;
}

brick_t *generateOperator_e(lld_t *mv) {
    line_t *line = mv->data;
    char ***tab = ucp_tab(line->line, "* = *", ucp_var, ucp_arg);
    printf("%s = %s", tab[0][0], tab[0][1]);
    ucp_free(tab);
    return 0;
}



int matchFunction(lld_t *mv) {
    line_t *line = mv->data;
    return ucp(line->line, "def *(*)", ucp_alpha, ucp_args) == 1;
}


brick_t *generateFunction(lld_t *mv) {
    line_t *line = mv->data;
    char ***tab = ucp_tab(line->line, "def *(*)", ucp_alpha, ucp_args);
    char **args = strToWords(tab[0][1], ' ');
    printf("%s(%s)", tab[0][0], tab[0][1]);

    
    //free
    //end:;
    for (int i = 0; args[i]; i++)
        free(args[i]);
    free(args);
    ucp_free(tab);
    return 0;
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

    
    //free
    //end:;
    for (int i = 0; vars[i]; i++)
        free(vars[i]);
    free(vars);
    ucp_free(tab);
    return 0;
}

int matchEndOfScope(lld_t *mv) {
    line_t *line = mv->data;
    return ucp(line->line, "}") == 1;   
}

brick_t *generateEndOfScope(lld_t *mv) {
    return 0;
}