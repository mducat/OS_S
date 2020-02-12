/*
** EPITECH PROJECT, 2020
** OS_S
** File description:
** All functions related with string
*/

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
            return (head[i-current]);
        }
    }
    return (0);
}

char *strchr(const char *s, int c)
{
    for (int i = 0; s[i]; i++) {
        if (s[i] == c)
            return (&s[i]);
    }
    return (0);
}

/*
strlen done Mathieu
strstr done Mathieu
strchr done Mathieu
strdup
strcmp
strcat
strncmp
*/
