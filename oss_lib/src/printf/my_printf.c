/*
** EPITECH PROJECT, 2019
** my_printf
** File description:
** primary file
*/

#include <oss.h>

#include "my.h"

void pf_putchar(char c, pf_conf_t *conf)
{
    write_raw(&c, 1);
    conf->char_printed += 1;
}

void pf_putstr(char *s, pf_conf_t *conf)
{
    int len = my_strlen(s);

    write_raw(s, len);
    conf->char_printed += len;
}

void print_arg(char const *format, int *i, va_list *ap, pf_conf_t *conf)
{
    /*void (*pr[])(va_list *, pf_conf_t *) = {print_str, print_nbr, print_nbr,
        print_char, print_str_format, print_binary, print_pointer, print_hexa,
        print_unsigned, print_double, print_percent, print_errno, print_memory,
        print_octal, print_hexa_maj};*/

    // function pointers does not work in OSS

    int ty = get_type(format, *i);

    get_configuration(format, *i, conf);
    *i = get_length_index(format, *i) + 1;
    if (ty < 0){
        free(conf->flags);
        return;
    }
    switch (ty) {
    case 0:
        print_str(ap, conf);
        break;
    case 1:
    case 2:
        print_nbr(ap, conf);
        break;
    case 3:
        print_char(ap, conf);
        break;
    case 4:
        print_str_format(ap, conf);
        break;
    case 5:
        print_binary(ap, conf);
        break;
    case 6:
        print_pointer(ap, conf);
        break;
    case 7:
        print_hexa(ap, conf);
        break;
    case 8:
        print_unsigned(ap, conf);
        break;
    case 9:
        print_double(ap, conf);
        break;
    case 10:
        print_percent(ap, conf);
        break;
    case 11:
        print_memory(ap, conf);
        break;
    case 12:
        print_octal(ap, conf);
        break;
    case 13:
        print_hexa_maj(ap, conf);
        break;
    }
    free(conf->flags);
}

int check_format(char const *format, int *i, va_list *ap, pf_conf_t *conf)
{
    if (format[*i] == '%' && format[*i] != '\0'){
        *i += 1;
        print_arg(format, i, ap, conf);
        check_format(format, i, ap, conf);
        return (1);
    }
    return (0);
}

int printf(const char *format, ...)
{
    va_list ap;
    pf_conf_t conf;

    conf.char_printed = 0;
    va_start(ap, format);
    for (int i = 0; format[i] != '\0'; i++){
        if (check_format(format, &i, &ap, &conf)){
            i--;
            continue;
        }
        pf_putchar(format[i], &conf);
    }
    va_end(ap);
    return (conf.char_printed);
}
