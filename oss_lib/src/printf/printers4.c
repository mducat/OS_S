/*
** EPITECH PROJECT, 2019
** my_printf
** File description:
** printers
*/

#include "my.h"

void print_hexa_maj(va_list *ap, pf_conf_t *conf)
{
    unsigned long long int n = get_number(ap, conf, 0);
    int len = get_nbr_length(n, "0123456789ABCDEF", 1);

    handle_conf('X', conf, len, n);
    my_putnbr_base(n, "0123456789ABCDEF", len, conf);
    post_conf('X', conf, len, n);
}
