#include "my.h"
#include <oss.h>
#include <lld.h>
#include <my.h>

int strlen(const char *str) {
    int len = 0;
    for (; str[len]; len++);
    return len;
}

void memcpy(char *dest, const char *src, int size){
    for (int i = 0; i < size; i++)
        dest[i] = src[i];
}

char **strToWords(const char *str, char split) {
    lld_t *lld = lld_init();
    int p = -1;
    int i = 0;
    for (; str[i]; i++) {
        if (str[i] == split) {
            i--;
            int len = i-p;
            char *str2 = malloc(len+1);
            str2[len] = 0;
            memcpy(str2, str+p+1, len);
            lld_insert(lld, lld_len(lld), str2);
            i++;
            p = i;
        }
    }
    int len = i-p-1;
    if (len > 0) {
        char *str2 = malloc(len+1);
        str2[len] = 0;
        memcpy(str2, str+p+1, len);
        lld_insert(lld, lld_len(lld), str2);
    }
    p = i;
    char **tab = (char **)lld_lld_to_tab(lld);
    lld_free(lld);
    return tab;
}

char *my_putnbr_base_str(unsigned long int nbr, char *base){
    int len = strlen(base);
    int i;
    static char to_print[65];
    static char to_print2[65];
    //nbr < 0 ? nbr *= -1 : 0;
    for (int y = 0; y < 65; to_print[y] = 0, to_print2[y] = 0, y++);
    for (len = 0; base[len]; len++);
    for (i = 0; nbr != 0; i++){
        to_print[i] = base[nbr%len];
        nbr /= len;
    }
    int a = 0;
    for (i = i-1; i >= 0; i--, a++)
        to_print2[a] = to_print[i];
    to_print2[0] == 0 ? to_print2[0] = base[0] : 0;
    return (to_print2);
}

int strcmp(const char *str1, const char *str2) {
    for (int i = 0; str1[i] && str2[i]; i++){
        if (str1[i] != str2[i])
            return str1[i] - str2[i];
    }
    return 0;
}

char *strdup(const char *str){
    char *dest = malloc(strlen(str)+1);
    memcpy(dest, str, strlen(str)+1);
    return dest;
}

#ifndef ULONG_MAX
#define        ULONG_MAX        ((unsigned long)(~0L))                /* 0xFFFFFFFF */
#endif
#ifndef LONG_MAX
#define        LONG_MAX        ((long)(ULONG_MAX >> 1))        /* 0x7FFFFFFF */
#endif
#ifndef LONG_MIN
#define        LONG_MIN        ((long)(~LONG_MAX))                /* 0x80000000 */
#endif

#define ISSPACE(c) (c == ' ')
#define ISDIGIT(c) (c >= '0' && c <= '9')
#define ISALPHA(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define ISUPPER(c) (c >= 'A' && c <= 'Z')


long strtol(const char *nptr, char **endptr, register int base)
{
        register const char *s = nptr;
        register unsigned long acc;
        register int c;
        register unsigned long cutoff;
        register int neg = 0, any, cutlim;
        /*
         * Skip white space and pick up leading +/- sign if any.
         * If base is 0, allow 0x for hex and 0 for octal, else
         * assume decimal; if base is already 16, allow 0x.
         */
        do {
                c = *s++;
        } while (ISSPACE(c));
        if (c == '-') {
                neg = 1;
                c = *s++;
        } else if (c == '+')
                c = *s++;
        if ((base == 0 || base == 16) &&
            c == '0' && (*s == 'x' || *s == 'X')) {
                c = s[1];
                s += 2;
                base = 16;
        }
        if (base == 0)
                base = c == '0' ? 8 : 10;
        /*
         * Compute the cutoff value between legal numbers and illegal
         * numbers.  That is the largest legal value, divided by the
         * base.  An input number that is greater than this value, if
         * followed by a legal input character, is too big.  One that
         * is equal to this value may be valid or not; the limit
         * between valid and invalid numbers is then based on the last
         * digit.  For instance, if the range for longs is
         * [-2147483648..2147483647] and the input base is 10,
         * cutoff will be set to 214748364 and cutlim to either
         * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
         * a value > 214748364, or equal but the next digit is > 7 (or 8),
         * the number is too big, and we will return a range error.
         *
         * Set any if any `digits' consumed; make it negative to indicate
         * overflow.
         */
        cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
        cutlim = cutoff % (unsigned long)base;
        cutoff /= (unsigned long)base;
        for (acc = 0, any = 0;; c = *s++) {
                if (ISDIGIT(c))
                        c -= '0';
                else if (ISALPHA(c))
                        c -= ISUPPER(c) ? 'A' - 10 : 'a' - 10;
                else
                        break;
                if (c >= base)
                        break;
                if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
                        any = -1;
                else {
                        any = 1;
                        acc *= base;
                        acc += c;
                }
        }
        if (any < 0) {
                acc = neg ? LONG_MIN : LONG_MAX;
        } else if (neg)
                acc = -acc;
        if (endptr != 0)
                *endptr = (char *) (any ? s - 1 : nptr);
        return (acc);
}

char *strconcat(char const *str1, char const *str2) {
    char *str3 = malloc(sizeof(char) * (strlen(str1) + strlen(str2)+1));
    int x = 0;

    for (int i = 0; str1[i]; i++, x++) str3[x] = str1[i];
    for (int i = 0; str2[i]; i++, x++) str3[x] = str2[i];
    str3[x] = 0;
    return (str3);
}