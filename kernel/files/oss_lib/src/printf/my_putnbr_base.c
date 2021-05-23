/*
** EPITECH PROJECT, 2019
** my_putnbr_base
** File description:
** Displays a number nbr into a base (given as an str)
*/

#include "my.h"

int get_display_length(unsigned long long int nbr, char const *base)
{
    int i = 0;
    int len = 0;

    for (; base[len] != '\0'; len++);
    while (nbr >= len){
        nbr /= len;
        i++;
    }
    return (i);
}

void my_putnbr_base(unsigned long long int nbr, char const *base,
    int disp_len, pf_conf_t *conf)
{
    int len = 0;
    int i = 0;
    int checker;
    char mem_display[disp_len];

    for (; base[len] != '\0'; len++);
    while (nbr >= len){
        checker = nbr % len;
        nbr /= len;
        mem_display[i++] = base[checker];
    }
    mem_display[i] = base[nbr % len];
    for (int j = disp_len - 1; j != -1; j--)
        pf_putchar(mem_display[j], conf);
}
