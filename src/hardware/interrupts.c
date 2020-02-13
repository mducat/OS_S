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

uint8_t inb(uint16_t port)
{
    uint8_t ret;

    asm volatile("inb %w1,%0"
                 : "=a" (ret)
                 : "Nd" (port)
                 :);
    return (ret);
}

void activate_interrupts(void)
{
    asm volatile("sti");
}

void deactivate_interrupts(void)
{
    asm volatile("cli");
}
