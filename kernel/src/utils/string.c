
#include <string.h>
#include <malloc.h>
#include <lld.h>

int strlen(const char *str)
{
    int length = 0;

    if (!str)
        return (-1);
    for (; str[length]; length++);
    return (length);
}

char *strstr (const char *head, const char *search)
{
    int size_search = strlen(search);
    int current = 0;

    for (int i = 0; head[i]; i++) {
        if (search[current] && head[i] == search[current]) {
            current++;
        }
        if (size_search == current) {
            return ((char *) &head[i-current]);
        }
    }
    return (0);
}

char *strchr(const char *s, int c)
{
    for (int i = 0; s[i]; i++) {
        if (s[i] == c)
            return ((char *) &s[i]);
    }
    return (0);
}

int strcmp(char const *str1, char const *str2)
{
    int i = 0;

    while (str1[i] == str2[i] && str1[i] && str2[i]) {
        i++;
    }
    return (str1[i] - str2[i]);
}

int strncmp(char const *s, char const *t, int n)
{
    while (n-- && *s == *t) {
          ++s;
          ++t;
    }
    if (n >= 0) return *s - *t;
    return 0;
}

void strcat(char *dest, char const *src)
{
    int x = 0;

    for (; dest[x]; x++);
    for (; src[x]; x++)
        dest[x] = src[x];
    dest[x] = 0;
}

char **strToWords(char *str, char split) {
    lld_t *lld = lld_init();
    int p = -1;
    int i = 0;
    for (; str[i]; i++) {
        if (str[i] == split) {
            i--;
            int len = i-p;
            if (len) {
                char *str2 = malloc(len+1);
                str2[len] = 0;
                memcpy(str2, str+p+1, len);
                lld_insert(lld, lld_len(lld), str2);
            }
            i++;
            p = i;
        }
    }
    int len = i-p-1;
    char *str2 = malloc(len+1);
    str2[len] = 0;
    memcpy(str2, str+p+1, len);
    lld_insert(lld, lld_len(lld), str2);
    p = i;
    char **tab = (char **)lld_lld_to_tab(lld);
    lld_free(lld);
    return tab;
}


/*
strlen done Mathieu
strstr done Mathieu
strchr done Mathieu
strdup
strcmp done Mathieu
strcat done Maxime
strncmp done Mathieu
*/
