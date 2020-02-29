/*
** EPITECH PROJECT, 2020
** OS_S
** File description:
** interrupts
*/

#ifndef _INT_H
#define _INT_H

#include "types.h"

#define IDT_LEN (2048)

uint8_t idt[IDT_LEN];

void init_interrupts(void);

void activate_interrupts(void);
void deactivate_interrupts(void);

#define MASTER_CMD  (0x20)
#define MASTER_DATA (0x21)
#define SLAVE_CMD   (0xA0)
#define SLAVE_DATA  (0xA1)

// EOI = End Of Interrupt
#define EOI (0x20)

// ICW = Init command word
#define ICW1_ICW4 (0x01)
#define ICW1_INIT (0x10)

#define ICW4_8086 (0x01)

// OCW = Operatonal command word
#define OCW3_IRR  (0x0A)
#define OCW3_ISR  (0x0B)

#define INTERRUPT_GATE (0x8e)

#endif
