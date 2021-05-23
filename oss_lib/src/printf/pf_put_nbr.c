/*
** EPITECH PROJECT, 2019
** my_put_nbr
** File description:
** Display the passed number to stdout
*/

#include "my.h"

int my_strncmp(const char *s1, const char *s2, int n)
{
    for (int i = 0; i < n; i++){
        if (s1[i] != s2[i])
            return (s1[i] - s2[i]);
    }
    return (0);
}

int is_recognized(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    if (c == '-' || c == '+')
        return (1);
    return (0);
}

int my_getnbr(char const *str)
{
    int res = 0;
    int end = 1;

    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] == '8' && res == 214748364 && end != -1)
            return (0);
        if (str[i] >= '0' && str[i] <= '9')
            res = 10 * res + str[i] - '0';
        if (str[i] == '-' && res == 0)
            end *= -1;
        if (str[i] == '+' && res == 0)
            end = 1;
        if (is_recognized(str[i]) == 0)
            break;
        if (res < 0 && res != -2147483648)
            return (0);
    }
    if (res != -2147483648)
        res *= end;
    return (res);
}

int my_strlen(char const *str)
{
    int counter = 0;
    for (; *str != '\0'; str++)
        counter++;
    return (counter);
}

void pf_put_nbr(long long int nb, pf_conf_t *conf)
{
    int magnitude = 1;
    long long int nb_copy = nb;
    if (nb < 0) {
        pf_putchar('-', conf);
        if (nb == -2147483648) {
            pf_putchar('2', conf);
            nb = -147483648;
        }
        nb *= -1;
        nb_copy = nb;
    }
    while (nb_copy > 9) {
        magnitude *= 10;
        nb_copy /= 10;
    }
    while (magnitude > 0) {
        pf_putchar((nb / magnitude) % 10 + 48, conf);
        magnitude /= 10;
    }
}
