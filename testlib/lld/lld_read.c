/*
** EPITECH PROJECT, 2019
** my_compute_power_it
** File description:
** hello
*/

#include "../my.h"

void *lld_read_start(lld_t *lld, void *pos)
{
    lld = lld->next;
    for (void *i = 0; pos > i; lld = lld->next, i++);
    void *data = lld->data;
    return (data);
}

void *lld_read_end(lld_t *lld, void *pos)
{
    lld_t *lld_o = lld;
    lld = lld->prev;
    for (void *i = lld_o->data-1; pos < i; lld = lld->prev, i--);
    void *data = lld->data;
    return (data);
}

void *lld_read(lld_t *lld, int poss)
{
    void *pos = (void *)(long int)poss;
    if ((long int)lld->data/2 >= (long int)pos){
        lld = lld_read_start(lld, pos);
    } else {
        lld = lld_read_end(lld, pos);
    }
    return (lld);
}
