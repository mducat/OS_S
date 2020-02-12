/*
** EPITECH PROJECT, 2020
** OS_S
** File description:
** All memory functions
*/

int memset(char *str, char c, int size)
{
    int i = 0;

    for (; str[i] && i < size; i++) {
        str[i] = c;
    }
    return (0);
}

int memcpy(char *dest, char *src)
{
    __try{
        for (int x = 0; src[x]; x++)
            dest[x] = src[x];
        dest[x] = 0;
        return (0);
    }__except{
        return (84);
    }
}
