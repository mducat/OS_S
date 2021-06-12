/*
** EPITECH PROJECT, 2019
** my_compute_power_it
** File description:
** hello
*/

#include "lld.h"
#include <oss.h>

lld_t *lld_init(void)
{
    lld_t *lld = malloc(sizeof(lld_t));
    lld->next = 0;
    lld->prev = 0;
    lld->data = 0;
    return (lld);
}
