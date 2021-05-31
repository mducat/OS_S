/*
** EPITECH PROJECT, 2019
** my_compute_power_it
** File description:
** hello
*/

#include "../my.h"
#include <unistd.h>
#include <stdio.h>

void lld_print_int(lld_t *lld)
{
    for (lld = lld->next; lld; lld = lld->next){
        printf("%li ", (long int)lld->data);
    }
    write(1, "\n", 1);
}
