/*
** EPITECH PROJECT, 2019
** my_compute_power_it
** File description:
** hello
*/

#include "lld.h"
#include "malloc.h"
#include "screen.h"

void *lld_pop_node_start(lld_t *lld, void *pos)
{
    if (pos >= lld->data) write_screen("db lld pop < 0\n", 21);
    lld_t *lld_o = lld;
    lld->data -= 1;
    lld = lld->next;
    for (void *i = 0; pos > i; lld = lld->next, i++);
    if (lld->prev)
        lld->prev->next = lld->next;
    else
        lld_o->next = lld->next;
    if (lld->next)
        lld->next->prev = lld->prev;
    else
        lld_o->prev = lld->prev;
    return (lld);
}

void *lld_pop_node_end(lld_t *lld, void *pos)
{
    if (pos >= lld->data) write_screen("db lld pop >= lld->data\n", 21);
    lld_t *lld_o = lld;
    lld->data -= 1;
    lld = lld->prev;
    for (void *i = lld_o->data; pos < i; lld = lld->prev, i--);
    if (lld->prev)
        lld->prev->next = lld->next;
    else
        lld_o->next = lld->next;
    if (lld->next)
        lld->next->prev = lld->prev;
    else
        lld_o->prev = lld->prev;
    return (lld);
}

void *lld_pop_node(lld_t *lld, int poss)
{
    void *pos = (void *)(long int)poss;
    if ((long int)lld->data/2 >= (long int)pos){
        lld = lld_pop_node_start(lld, pos);
    } else {
        lld = lld_pop_node_end(lld, pos);
    }
    return (lld);
}
