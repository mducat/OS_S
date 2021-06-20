/*
** EPITECH PROJECT, 2019
** my_printf
** File description:
** Configuration handling
*/

#include "my.h"
//    char *flags = "-+ 0#";

int is_signed(char type)
{
    if (type == 'd' || type == 'i' || type == 'f' ||
        type == 'f' || type == 'F' || type == 'e' ||
        type == 'E' || type == 'g' || type == 'G')
        return (1);
    return (0);
}

void handle_number(char type, pf_conf_t *conf, int nb)
{
    if (!is_signed(type))
        return;
    if (nb < 0)
        return (pf_putchar('-', conf));
    if (conf->flags[1])
        pf_putchar('+', conf);
    else if (conf->flags[2])
        pf_putchar(' ', conf);
}

int handle_alt(char type, pf_conf_t *conf, int nb, int printing)
{
    (void) nb;
    if (!conf->flags[4])
        return (0);
    switch (type){
    case 'o':
        if (printing)
            pf_putchar('0', conf);
        return (1);
    case 'x':
        if (printing)
            pf_putstr("0x", conf);
        return (2);
    case 'X':
        if (printing)
            pf_putstr("0X", conf);
        return (2);
    }
    return (0);
}

void handle_conf(char type, pf_conf_t *conf, int size, int nb)
{
    int offset = ((conf->flags[1] | conf->flags[2]) & (nb > 0)) | (nb < 0);
    int padding = conf->width - size - offset;
    int zeros = 0;

    if (type == '%')
        return;
    padding -= handle_alt(type, conf, nb, 1);
    if (conf->precision > 0 && (conf->flags[0] | conf->flags[3])
        && is_numtype(type))
        padding -= (zeros = conf->precision - size);
    else if (conf->width > 0 && conf->flags[3] && is_numtype(type))
        padding -= (zeros = conf->width - size);
    if (!conf->flags[0])
        for (int i = 0; i < padding; i++)
            pf_putchar(' ', conf);
    handle_number(type, conf, nb);
    for (int i = 0; i < zeros; i++)
        pf_putchar('0', conf);
}

void post_conf(char type, pf_conf_t *conf, int size, int nb)
{
    char print = ' ';
    int offset = ((conf->flags[1] | conf->flags[2]) & (nb > 0)) | (nb < 0);
    int padding = conf->width - size - offset;

    if (!conf->flags[0])
        return;
    padding -= handle_alt(type, conf, nb, 0);
    if (conf->precision > 0 && (conf->flags[0] | conf->flags[3])
        && is_numtype(type))
        padding -= conf->precision - size;
    else if (conf->width > 0 && conf->flags[3] && is_numtype(type))
        padding -= conf->width - size;
    if (padding > 0){
        for (int i = 0; i < padding; i++)
            pf_putchar(print, conf);
    }
}
