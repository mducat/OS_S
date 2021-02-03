
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
    outb(MASTER_DATA, 0xFD);
    outb(SLAVE_DATA, 0xFD);
}

void register_int_handler(uint8_t *idt, uint8_t irq, void (*handler)(), uint8_t type)
{
    uintptr_t ptr = (uintptr_t) handler;
    uint16_t loc = irq * 8;
    uint16_t kernel = get_kernel_code_location();

    idt[loc + 0] = (ptr >> 0) & 0xFF;
    idt[loc + 1] = (ptr >> 8) & 0xFF;
    idt[loc + 2] = (kernel >> 0) & 0xFF;
    idt[loc + 3] = (kernel >> 8) & 0xFF;
    idt[loc + 4] = 0;
    idt[loc + 5] = type;
    idt[loc + 6] = (ptr >> 16) & 0xFF;
    idt[loc + 7] = (ptr >> 24) & 0xFF;
}

void end_of_interrupt(uint8_t irq)
{
    if (irq > 8)
        outb(SLAVE_CMD, EOI);
    outb(MASTER_CMD, EOI);
    asm ("iret");
}

void irq0_handler(void)
{
    write_screen("I0", 2);
    end_of_interrupt(1);
}

void irq1_handler(void)
{
    write_screen("I1", 2);
    end_of_interrupt(1);
}

void irq2_handler(void)
{
    write_screen("I2", 2);
    end_of_interrupt(2);
}

void irq3_handler(void)
{
    //write_screen("I3", 2);
    end_of_interrupt(0x8);
}

void irq4_handler(void)
{
    //write_screen("I4", 2);
    end_of_interrupt(0xd);
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
    ptr[0] = (IDT_LEN) << 16;
    ptr[1] = ((uintptr_t) idt) & 0xFFFFFFFF;
    asm volatile("lidt (%0)"
                 : /* no output */
                 : "p" ((char *) ptr + 2)
                 :);
    activate_interrupts();
}

void int_ignore(void)
{
    return;
}

void init_interrupts(void)
{
    uint8_t *idt = (uint8_t *) 0x1000000;//[IDT_LEN];

    for (uint8_t i = 0; i < 255; i++)
        register_int_handler(idt, i, int_ignore, INT_GATE);
    //register_int_handler(idt, 0x09, irq2_handler);
    //register_int_handler(idt, 0x06, irq2_handler);
    register_int_handler(idt, 0x08, irq3_handler, INT_GATE);
    register_int_handler(idt, 0x0d, irq4_handler, INT_GATE);
    //register_int_handler(idt, 0x21, irq0_handler, INT_GATE);

    //char *str = my_putnbr_base(idt[256], "0123456789");
    //mvprint(0, 2, str, 0x3);
    mem_print(1, 1, &(idt[256]), 0x30);
    //mem_print(1, 1, 0, 0x50);
    remap_pic();
    //load_idt(idt);

    uint16_t ret[4];
    ret[3] = 0x72;
    ret[1] = 0x42;
    asm("sidt %0"
        : 
        : "m" (ret)
        :);

    char *str = my_putnbr_base(ret[0], "0123456789ABCDEF");
    mvprint(0, 0, str, 0x2);
    str = my_putnbr_base(ret[1], "0123456789ABCDEF");
    mvprint(0, 1, str, 0x2);
    str = my_putnbr_base(ret[2], "0123456789ABCDEF");
    mvprint(0, 2, str, 0x2);
    str = my_putnbr_base(ret[3], "0123456789ABCDEF");
    mvprint(0, 3, str, 0x2);

/*    while (1) {
        short reg = get_requested_interrupts();
        char *str = my_putnbr_base(reg, "01");
        mvprint(10, 0, str, 0x3);
        if (reg & 0x2) {
            irq1_handler();
            break;
            }
        uint8_t a = inb(KBD_STATUS);
        str = my_putnbr_base(a, "0123456789");
        write_screen(str, strlen(str));
        write_screen(" ", 1);
        if (!(a & 1))
            continue;
        uint8_t input = inb(KBD_DATA);
        char c = keyboard_map[input];
        if (!c)
            continue;
        write_screen(&c, 1);
        //write_screen(" ", 1);
    }*/
    // while (1)
    // asm("hlt");
}
