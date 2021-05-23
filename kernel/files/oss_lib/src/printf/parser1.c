/*
** EPITECH PROJECT, 2019
** my_printf
** File description:
** parser
*/

#include "my.h"

void get_flags(char const *format, int i, int fl[])
{
    char *flags = "-+ 0#";

    for (int j = 0; flags[j] != 0; j++){
        if (format[i] == flags[j]){
            fl[j] = 1;
            i++;
            j = -1;
        }
    }
}

int get_size(char const *format, int i)
{
    char *sizes[] = {"hh", "h", "ll", "l", "L", "z", "j", "t", 0};
    int len = 0;

    i = get_precision_index(format, i);
    for (int j = 0; sizes[j] != 0; j++){
        len = my_strlen(sizes[j]);
        if (my_strncmp(format + i, sizes[j], len) == 0)
            return (j);
    }
    return (-1);
}

int get_type(char const *format, int i)
{
    char *types = "sdicSbpxuf%mnoX";

    i = get_length_index(format, i);
    for (int j = 0; types[j] != 0; j++){
        if (format[i] == types[j])
            return (j);
    }
    return (-1);
}

int get_width(char const *format, int i)
{
    int width = -1;
    int w_index = get_width_index(format, i);
    int f_index = get_flag_index(format, i);

    if (f_index != w_index && is_num(format[f_index]))
        width = my_getnbr(format + f_index);
    return (width);
}

int get_precision(char const *format, int i)
{
    int precision = -1;
    int w_index = get_width_index(format, i);
    int p_index = get_precision_index(format, i);

    if (w_index != p_index && is_num(format[w_index + 1]))
        precision = my_getnbr(format + w_index + 1);
    return (precision);
}
