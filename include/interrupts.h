
#ifndef _INT_H
#define _INT_H

#include <types.h>

#define IDT_LEN (2048)

void init_interrupts(void);

void activate_interrupts(void);
void deactivate_interrupts(void);

#define MASTER_CMD  (0x20)
#define SLAVE_CMD   (0xA0)
#define MASTER_DATA (0x21)
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

// FORMAT:
// [P,PL,0,D,1,1,0]
// P = segment present flag
// PL = Privilege level (ring) [0-3]
// D = Gate size (0 = 16 bits, 1 = 32 bits) 
#define INT_GATE (0b10001110)
#define TRAP_GATE (0b10001111)
#define TASK_GATE (0b10000101)

#define KBD_DATA (0x60)
#define KBD_STATUS (0x64)

// from http://www.osdever.net/bkerndev/Docs/keyboard.htm
static const unsigned char keyboard_map[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',/* 9 */
    '9', '0', '-', '=', '\b',/* Backspace */
    '\t',/* Tab */
    'a', 'z', 'e', 'r',/* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n',/* Enter key */
    0,/* 29   - Control */
    'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm',/* 39 */
    '%', '*',   0,/* Left shift */
    '<', 'w', 'x', 'c', 'v', 'b', 'n',/* 49 */
    ',', '.', ':', '!',   0,/* Right shift */
    '*',
    0,/* Alt */
    ' ',/* Space bar */
    0,/* Caps lock */
    0,/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,/* < ... F10 */
    0,/* 69 - Num lock*/
    0,/* Scroll Lock */
    0,/* Home key */
    0,/* Up Arrow */
    0,/* Page Up */
    '-',
    0,/* Left Arrow */
    0,
    0,/* Right Arrow */
    '+',
    0,/* 79 - End key*/
    0,/* Down Arrow */
    0,/* Page Down */
    0,/* Insert Key */
    0,/* Delete Key */
    0,   0,   0,
    0,/* F11 Key */
    0,/* F12 Key */
    0,/* All other keys are undefined */
};

#endif
