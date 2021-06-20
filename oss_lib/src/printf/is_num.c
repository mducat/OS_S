/*
** EPITECH PROJECT, 2019
** libmy
** File description:
** is_numerical char
*/

int is_num(char c)
{
    if (c > '9' || c < '0')
        return (0);
    return (1);
}

int is_numtype(char type)
{
    if (type == '%' || type == 's' ||
        type == 'S' || type == 'c' || type == 'p')
        return (0);
    return (1);
}
