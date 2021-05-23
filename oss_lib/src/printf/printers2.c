/*
** EPITECH PROJECT, 2019
** my_printf
** File description:
** flags displayers
*/

#include "my.h"

int handle_null(int len, pf_conf_t *conf)
{
    if (len != 0)
        return (0);
    handle_conf('p', conf, 5, 0);
    pf_putstr("(nil)", conf);
    post_conf('p', conf, 5, 0);
    return (1);
}

void print_pointer(va_list *ap, pf_conf_t *conf)
{
    long addr = (long) va_arg(*ap, void *);
    int len = 0;
    char to_print;

    for (; addr >> (len * 4) != 0; len++);
    if (handle_null(len, conf) != 0)
        return;
    handle_conf('p', conf, len, 0);
    pf_putchar('0', conf);
    pf_putchar('x', conf);
    for (int i = len - 1; i > -1; i--){
        to_print = (addr >> (i * 4)) & 0xF;
        to_print = to_print < 10 ? to_print + '0' : to_print - 10 + 'a';
        pf_putchar(to_print, conf);
    }
    post_conf('p', conf, len, 0);
}

void print_binary(va_list *ap, pf_conf_t *conf)
{
    unsigned long long int n = get_number(ap, conf, 0);
    int len = get_nbr_length(n, "01", 1);

    handle_conf('b', conf, len, n);
    my_putnbr_base(n, "01", len, conf);
    post_conf('b', conf, len, n);
}

void print_hexa(va_list *ap, pf_conf_t *conf)
{
    unsigned long long int n = get_number(ap, conf, 0);
    int len = get_nbr_length(n, "0123456789abcdef", 1);

    handle_conf('x', conf, len, n);
    my_putnbr_base(n, "0123456789abcdef", len, conf);
    post_conf('x', conf, len, n);
}

void print_unsigned(va_list *ap, pf_conf_t *conf)
{
    unsigned long long int n = get_number(ap, conf, 0);
    int len = get_nbr_length(n, "0123456789", 1);

    handle_conf('u', conf, len, n);
    my_put_unsigned_nbr(n, conf);
    post_conf('u', conf, len, n);
}
