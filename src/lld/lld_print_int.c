/*
** EPITECH PROJECT, 2019
** my_compute_power_it
** File description:
** hello
*/

#include "lld.h"
#include "screen.h"
#include "malloc.h"

void lld_print_int(lld_t *lld)
{
    for (lld = lld->next; lld; lld = lld->next){
        my_put_nbr((long int)lld->data);
        write_screen(" ", 1);
    }
    write_screen("\n", 1);
}
