/*
** EPITECH PROJECT, 2020
** OS_S
** File description:
** interrupts handling
*/

#include "types.h"
#include "interrupts.h"
#include "segments.h"

#include "screen.h"
#include "malloc.h"

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

static uint16_t read_register(uint8_t cmd)
{
    outb(MASTER_CMD, cmd);
    outb(SLAVE_CMD, cmd);

    return (inb(MASTER_CMD) | inb(SLAVE_CMD) << 8);
}

uint16_t get_serviced_interrupts(void)
{
    //ISR = In-service register
    return (read_register(OCW3_ISR));
}

uint16_t get_requested_interrupts(void)
{
    //ISR = Interrupts request register
    return (read_register(OCW3_IRR));
}

void remap_pic(void)
{
    // restart both PIC
    outb(MASTER_CMD, ICW1_INIT | ICW1_ICW4);
    outb(SLAVE_CMD, ICW1_INIT | ICW1_ICW4);

    // set Interrupt Vector Table (IVT) location
    outb(MASTER_DATA, 0x20);
    outb(SLAVE_DATA, 0x28);

    // Connect master and slave
    outb(MASTER_DATA, 0x4);
    outb(SLAVE_DATA, 0x2);

    // set the PIC mode
    outb(MASTER_DATA, ICW4_8086);
    outb(SLAVE_DATA, ICW4_8086);

    // set new PIC mask
    outb(MASTER_DATA, 0x0);
    outb(SLAVE_DATA, 0x0);
}

void register_int_handler(uint8_t irq, void (*handler)())
{
    uint32_t ptr = (uint32_t) handler;
    uint16_t loc = irq * 8;
    uint16_t kernel = get_kernel_code_location();

    idt[loc + 0] = (ptr >> 0) & 0xFF;
    idt[loc + 1] = (ptr >> 8) & 0xFF;
    idt[loc + 2] = (kernel >> 0) & 0xFF;
    idt[loc + 3] = (kernel >> 8) & 0xFF;
    idt[loc + 4] = 0;
    idt[loc + 5] = INTERRUPT_GATE;
    idt[loc + 6] = (ptr >> 16) & 0xFF;
    idt[loc + 7] = (ptr >> 24) & 0xFF;
}

void end_of_interrupt(uint8_t irq)
{
    if (irq > 8)
        outb(SLAVE_CMD, EOI);
    outb(MASTER_CMD, EOI);
}

void irq0_handler(void)
{
    write_screen("INTERRUPT", 9);
    end_of_interrupt(0);
}

void irq1_handler(void)
{
    write_screen("INTERRUPT 2", 11);
    end_of_interrupt(1);
}

void activate_interrupts(void)
{
    asm volatile("sti");
}

void deactivate_interrupts(void)
{
    asm volatile("cli");
}

void load_idt(uint8_t* idt)
{
    uint32_t ptr[2];
    ptr[0] = IDT_LEN << 16;
    ptr[1] = (uint32_t) idt;
    asm volatile("lidt (%0)"
                 : /* no output */
                 : "p" (ptr + 2)
                 :);
    activate_interrupts();
}

void init_interrupts(void)
{
    remap_pic();

    //register_int_handler(0x20, &irq0_handler);
    //register_int_handler(0x21, &irq1_handler);

    //char *str = my_putnbr_base(idt[256], "0123456789");
    //mvprint(0, 2, str, 0x3);
    mem_print(1, 1, &(idt[256]), 0x30);
    while (1){
        char *str = my_putnbr_base(get_requested_interrupts(), "01");
        mvprint(0, 0, str, 0x3);
    }
    load_idt(idt);
}
