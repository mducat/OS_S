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

char *generate_nothing(char **words) {
    (void)words;
    return 0;
}

int match_operator_e(lld_t *mv) 
{
    return 0;
}



