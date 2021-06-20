/*
** EPITECH PROJECT, 2019
** my_printf
** File description:
** printers function
*/

#include "my.h"

void print_errno(va_list *ap, pf_conf_t *conf)
{
    (void)ap;
    (void)conf;
    // char const *error = sys_errlist[errno];

    // pf_putstr(error, conf);
}

void print_char(va_list *ap, pf_conf_t *conf)
{
    int c = va_arg(*ap, int);

    handle_conf('c', conf, 1, 0);
    pf_putchar(c, conf);
    post_conf('c', conf, 1, 0);
}

void print_percent(va_list *ap, pf_conf_t *conf)
{
    (void) ap;
    pf_putchar('%', conf);
}

void print_memory(va_list *ap, pf_conf_t *conf)
{
    int *address = va_arg(*ap, int *);

    *address = conf->char_printed;
}

void print_octal(va_list *ap, pf_conf_t *conf)
{
    unsigned long long int n = get_number(ap, conf, 0);
    int len = get_nbr_length(n, "01234567", 1);

    handle_conf('o', conf, len, n);
    my_putnbr_base(n, "01234567", len, conf);
    post_conf('o', conf, len, n);
}
