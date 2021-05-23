/*
** EPITECH PROJECT, 2019
** my_put_nbr
** File description:
** Display the passed number to stdout
*/

#include "my.h"

void my_put_unsigned_nbr(unsigned long long int nb, pf_conf_t *conf)
{
    long long int magnitude = 1;
    unsigned long long int nb_copy = nb;

    while (nb_copy > 9) {
        magnitude *= 10;
        nb_copy /= 10;
    }
    while (magnitude > 0) {
        pf_putchar((nb / magnitude) % 10 + 48, conf);
        magnitude /= 10;
    }
}
