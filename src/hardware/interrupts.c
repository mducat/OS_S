/*
** EPITECH PROJECT, 2020
** OS_S
** File description:
** interrupts handling
*/

#include "types.h"

void outb(uint8_t value, uint16_t port)
{
    asm volatile("outb %b0,%w1"
                 :
                 : "a" (value), "Nd" (port)
                 :);
}

void activate_interrupts(void)
{
    asm volatile("sti");
}

void deactivate_interrupts(void)
{
    asm volatile("cli");
}
