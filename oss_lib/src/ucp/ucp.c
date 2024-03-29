/*
** EPITECH PROJECT, 2019
** ucp (universal_c_parseur)
** File description:
** main file for ucp
*/

#include "my.h"
#include "lld.h"
#include "oss.h"

int ucp_recursiv(char *str, char *buf, char *regex, int (**fonc)(char *))
{
    int total = 0;
    if (*regex == '*'){
        int i = 0;
        do {
            buf[i] = 0;
            if (fonc[0](buf)){
                buf[i] = str[i];
                total += ucp_recursiv(str+i, buf+i, regex+1, fonc+1);
            } else
                buf[i] = str[i];
        } while (str[i++]);
    } else {
        while (*regex == *str && *regex != '*'){
            if (!*regex && !*str)
                return (1);
            regex++, str++, buf++;
        }
        *regex == '*' ? total += ucp_recursiv(str, buf, regex, fonc) : 0;
    }
    return (total);
}

int ucp(char *str, char *regex, ...)
{
    va_list va;
    lld_t *lld_fonc = lld_init();
    char *buf = strdup(str);

    va_start(va, regex);
    for (int i = 0; regex[i]; i++)
        if (regex[i] == '*')
            lld_insert(lld_fonc, (u64)lld_fonc->data, va_arg(va, void *));
    int (**fonc)(char *) = (void *)lld_lld_to_tab(lld_fonc);
    lld_free(lld_fonc);
    int tot = ucp_recursiv(str, buf, regex, fonc);
    free(buf);
    free(fonc);
    return (tot);
}

int ucp_ptrs(char *str, char *regex, int *(**foncs)(char *)) {
    lld_t *lld_fonc = lld_init();
    char *buf = strdup(str);
    int fonc_p = 0;
    for (int i = 0; regex[i]; i++)
        if (regex[i] == '*')
            lld_insert(lld_fonc, (u64)lld_fonc->data, foncs[fonc_p++]);
    int (**fonc)(char *) = (void *)lld_lld_to_tab(lld_fonc);
    lld_free(lld_fonc);
            
    int tot = ucp_recursiv(str, buf, regex, fonc);
    free(buf);
    free(fonc);
    return (tot);
}