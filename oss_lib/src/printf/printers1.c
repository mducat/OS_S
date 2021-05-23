/*
** EPITECH PROJECT, 2019
** my_printf
** File description:
** Printers functions
*/

#include "my.h"

void print_np(char c, pf_conf_t *conf)
{
    int len = get_nbr_length(c, "0123456789", 0);

    c = c > 0 ? c : -c;
    pf_putchar('\\', conf);
    if (c < 64)
        pf_putchar('0', conf);
    if (c < 8)
        pf_putchar('0', conf);
    my_putnbr_base(c, "01234567", len, conf);
}

void put_str_internal(char *s, int mode, int len, pf_conf_t *conf)
{
    if (!mode){
        for (int i = 0; i < len; i++)
            pf_putchar(s[i], conf);
        return;
    }
    for (int i = 0; i < len; i++){
        if (s[i] >= 32 && s[i] < 127)
            pf_putchar(s[i], conf);
        else
            print_np(s[i], conf);
    }
}

void print_str(va_list *ap, pf_conf_t *conf)
{
    char *s = va_arg(*ap, char *);
    int len = (s == 0) ? 6 : my_strlen(s);
    int pr = conf->precision;

    len = (pr > 0 && len > pr) ? pr : len;
    handle_conf('s', conf, len, 0);
    if (s == 0){
        if (len > 5)
            pf_putstr("(null)", conf);
        else
            len = 0;
        post_conf('s', conf, len, 0);
        return;
    }
    put_str_internal(s, 0, len, conf);
    post_conf('s', conf, len, 0);
    return;
}

void print_str_format(va_list *ap, pf_conf_t *conf)
{
    char *s = va_arg(*ap, char *);
    int len = (s == 0) ? 6 : my_strlen(s);
    int pr = conf->precision;

    len = (pr > 0 && len < pr) ? len : pr;
    handle_conf('s', conf, len, 0);
    if (s == 0){
        if (len > 5)
            pf_putstr("(null)", conf);
        else
            len = 0;
        post_conf('s', conf, len, 0);
        return;
    }
    put_str_internal(s, 1, len, conf);
    post_conf('s', conf, len, 0);
}

void print_nbr(va_list *ap, pf_conf_t *conf)
{
    long long int n = get_number(ap, conf, 1);
    int len = get_nbr_length(n, "0123456789", 0);

    handle_conf('d', conf, len, n);
    pf_put_nbr(abs(n), conf);
    post_conf('d', conf, len, n);
}
