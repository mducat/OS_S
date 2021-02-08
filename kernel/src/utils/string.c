
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

int strncmp(char const *str1, char const *str2, int nb)
{
    int i = 0;

    while (str1[i] == str2[i] && str1[i] && str2[i]) {
        if (i == nb)
            break;
        i++;
    }
    return (str1[i] - str2[i]);
}

void strcat(char *dest, char const *src)
{
    int x = 0;

    for (; dest[x]; x++);
    for (; src[x]; x++)
        dest[x] = src[x];
    dest[x] = 0;
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
