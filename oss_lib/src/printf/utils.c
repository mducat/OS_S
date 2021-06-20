/*
** EPITECH PROJECT, 2019
** my_printf
** File description:
** Text utilities
*/

#include <oss.h>

#include "my.h"

int get_unsigned_length(unsigned long long int nbr, char *base)
{
    int len = 0;
    int b = my_strlen(base);

    while (nbr != 0){
        nbr /= b;
        len++;
    }
    return (len);
}

int get_nbr_length(long long int nbr, char *base, int is_signed)
{
    int len = 0;
    int b = my_strlen(base);

    if (nbr == 0)
        len++;
    if (is_signed)
        return (get_unsigned_length(nbr, base));
    while (nbr != 0){
        nbr /= b;
        len++;
    }
    return (len);
}

long long int get_unsigned(va_list *ap, pf_conf_t *conf)
{
    switch(conf->size){
    case -1:
        return ((unsigned int) va_arg(*ap, int));
    case 0:
        return ((unsigned char) va_arg(*ap, int));
    case 1:
        return ((unsigned short) va_arg(*ap, int));
    case 2:
    case 4:
        return ((unsigned long long int) va_arg(*ap, long long int));
    case 3:
        return ((unsigned long int) va_arg(*ap, long int));
    case 5:
        return (va_arg(*ap, size_t));
    }
    return (0);
}

long long int get_signed(va_list *ap, pf_conf_t *conf)
{
    switch(conf->size){
    case -1:
        return (va_arg(*ap, int));
    case 0:
        return ((char) va_arg(*ap, int));
    case 1:
        return ((short) va_arg(*ap, int));
    case 2:
    case 4:
        return (va_arg(*ap, long long int));
    case 3:
        return (va_arg(*ap, long int));
    case 5:
        return (va_arg(*ap, size_t));
    }
    return (0);
}

long long int get_number(va_list *ap, pf_conf_t *conf, int is_signed)
{
    if (is_signed)
        return (get_signed(ap, conf));
    else
        return (get_unsigned(ap, conf));
}
