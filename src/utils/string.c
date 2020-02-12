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

int strstr (const char *head, const char *search)
{
    int size_search = strlen(search);
    int ptr = 0;

    for (int i = 0; head[i]; i++) {

    }
}
/*
strstr
strchr
strdup
strcmp
strcat
strncmp
*/
