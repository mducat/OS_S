#ifndef __COSS_H__
#define __COSS_H__

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



typedef struct line {
    char *line;
    char **words;
} line_t;

typedef struct var {
    char *name;
} var_t;

typedef unsigned int uint;

typedef struct brick {
    char *code;
    struct brick **bricks;
} brick_t;

typedef struct instruction {
    brick_t *(*generate)(lld_t *mv);
    int (*match)(lld_t *mv);
} instruction_t;



typedef struct coss {
    lld_t *stack_vars; // stack of all variables var_t
    instruction_t **instructionsSet;
} coss_t;

extern coss_t *global_coss;

#endif