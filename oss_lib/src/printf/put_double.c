/*
** EPITECH PROJECT, 2019
** put_double
** File description:
** Main file
*/

#include "my.h"

double get_double_mag(double a)
{
    double mag = 1;

    while (a > 10 * mag)
        mag *= 10;
    return (mag);
}

int float_equal(double a, double b)
{
    int ab = abs(a - b);

    if (b == 1.f)
        return (a > b);
    return (ab < 0.01);
}

double float_mod(double a, double n)
{
    double mag = get_double_mag(a);

    while (a > n || float_equal(a, n)){
        if (a > mag * n || float_equal(a, mag * n))
            a -= mag * n;
        else
            mag /= 10;
    }
    return (abs(a));
}

void put_double(double a, double precision, pf_conf_t *conf)
{
    double float_p = float_mod(a, 1) * 10;
    double int_p = (a - float_mod(a, 1));
    double mag = get_double_mag(a);
    int round;

    while (mag > 0.1){
        pf_putchar(float_mod(int_p / mag, 10) + 0x30, conf);
        mag /= 10;
    }
    if (precision <= 0)
        return;
    pf_putchar('.', conf);
    while (precision-- > 1){
        pf_putchar(float_mod(float_p, 10) + 0x30, conf);
        float_p *= 10;
    }
    round = (int) float_mod(float_p, 10);
    if (float_mod(float_p * 10, 10) > 4)
        round += (round < 9 ? 1 : -10);
    pf_putchar(round + 0x30, conf);
}

void print_double(va_list *ap, pf_conf_t *conf)
{
    double res = va_arg(*ap, double);
    int precision = 6;

    if (conf->precision != -1)
        precision = conf->precision;
    put_double(res, precision, conf);
}
