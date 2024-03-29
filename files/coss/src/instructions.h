#include "coss.h"

#ifdef __OSS__
#include <lld.h>
#include <oss.h>

#else

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#endif

typedef enum ligne_type {
    FUNCTION,
    SCOPE,
    OPERATOR_E,
    OPERATOR_E_OPERATOR,
    END_OF_SCOPE,
    RETURN,
    CALL,
} ligne_type_e;

brick_t *concatBrick(brick_t *a, brick_t *b);
void brickAdd(brick_t *a, brick_t *b);
void brickFree(brick_t *a);
brick_t *brickInit(char *str);


char *generate_nothing(lld_t *mv);
int match_nothing(lld_t *mv);


int matchFunction(lld_t *mv);
brick_t *generateFunction(lld_t *mv);

int matchScope(lld_t *mv);
brick_t *generateScope(lld_t *mv);

int matchOperator_e(lld_t *mv);
brick_t *generateOperator_e(lld_t *mv);

int matchOperator_e_operator(lld_t *mv);
brick_t *generateOperator_e_operator(lld_t *mv);

int matchEndOfScope(lld_t *mv);
brick_t *generateEndOfScope(lld_t *mv);

int matchReturn(lld_t *mv);
brick_t *generateReturn(lld_t *mv);

int matchCall(lld_t *mv);
brick_t *generateCall(lld_t *mv);

