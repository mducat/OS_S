/*
** EPITECH PROJECT, 2019
** my_compute_power_it
** File description:
** hello
*/

#include "lld.h"
#include <oss.h>
#include "screen.h"

void *lld_write_start(lld_t *lld, void *pos, void *data)
{
    lld = lld->next;
    for (void *i = 0; pos > i; lld = lld->next, i++);
    void *data2 = lld->data;
    lld->data = data;
    return (data2);
}

void *lld_write_end(lld_t *lld, void *pos, void *data)
{
    lld_t *lld_o = lld;
    lld = lld->prev;
    for (void *i = lld_o->data-1; pos < i; lld = lld->prev, i--);
    void *data2 = lld->data;
    lld->data = data;
    return (data2);
}

void *lld_write(lld_t *lld, int poss, void *data)
{
    void *pos = (void *)(long int)poss;
    if ((long int)lld->data/2 >= (long int)pos){
        data = lld_write_start(lld, pos, data);
    } else {
        data = lld_write_end(lld, pos, data);
    }
    return (data);
}
