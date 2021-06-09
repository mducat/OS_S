
#include <irq.h>
#include <gdt.h>
#include <string.h>
#include <screen.h>

#include <proc.h>

#include <sys/io.h>

#include <dev/ps2.h>
#include <dev/serial.h>

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

static void remap_pic(void)
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
    outb(MASTER_DATA, 0xF9); // FD
    outb(SLAVE_DATA, 0xEF); // EF
}

static void register_int_handler(uint8_t *idt, uint8_t irq, void (*handler)(void), uint8_t type)
{
    uintptr_t ptr = (uintptr_t) handler;
    uint16_t loc = irq * 16;
    uint16_t kernel = get_kernel_code_location(); // CS reg val
    uint32_t *l_pt = (uint32_t *) idt;

    idt[loc + 0] = (ptr >> 0) & 0xFF;
    idt[loc + 1] = (ptr >> 8) & 0xFF;
    idt[loc + 2] = (kernel >> 0) & 0xFF;
    idt[loc + 3] = (kernel >> 8) & 0xFF;
    idt[loc + 4] = 0;
    idt[loc + 5] = type;
    idt[loc + 6] = (ptr >> 16) & 0xFF;
    idt[loc + 7] = (ptr >> 24) & 0xFF;

    l_pt += 2;
    l_pt[0] = (ptr >> 32) & 0xFFFFFFFF;
    l_pt[1] = 0;
}

void end_of_interrupt(uint8_t irq)
{
    if (irq > 8)
        outb(SLAVE_CMD, EOI);
    outb(MASTER_CMD, EOI);
}

void irq0_handler(void)
{
    end_of_interrupt(0);
}

void irq2_handler(void) // cascade interrupt
{
    end_of_interrupt(2);
}

void irq80_handler(void)
{
    uint64_t num;
    uint64_t par1;
    uint64_t par2;
    uint64_t par3;

    asm("" : "=a" (num),
        "=b" (par1),
        "=c" (par2),
        "=d" (par3));
    end_of_interrupt(30);
    syscall_handler(num, par1, par2, par3);
}

void activate_interrupts(void)
{
    asm volatile("sti");
}

void deactivate_interrupts(void)
{
    asm volatile("cli");
}

static void load_idt(uint8_t* idt)
{
    uint32_t ptr[4] = {0};
    uintptr_t idt_loc = (uintptr_t) idt;

    ptr[0] = (IDT_LEN) | (idt_loc & 0xFFFF) << 16;
    ptr[1] = (idt_loc & 0xFFFF0000) >> 16;
    asm volatile("lidt %0"
                 : 
                 : "m" (ptr)
                 :);
    
    activate_interrupts();
}

void init_interrupts(void)
{
    uint8_t *idt = (uint8_t *) 0x1000000;//[IDT_LEN];

    memset(idt, 0, IDT_LEN * 16);
    //end_of_interrupt(1);
    //for (uint8_t i = 0; i < 255; i++)
    //    register_int_handler(idt, i, int_ignore, INT_GATE);

    register_int_handler(idt, 0x20, irq0_caller, INT_GATE);
    register_int_handler(idt, 0x21, irq1_caller, INT_GATE);
    register_int_handler(idt, 0x22, irq2_caller, INT_GATE);
    register_int_handler(idt, 0x2c, irq12_caller, INT_GATE);
    register_int_handler(idt, 0x2c, irq12_caller, INT_GATE);
    register_int_handler(idt, 0x30, irq80_caller, INT_GATE);

    remap_pic();

    load_idt(idt);
    init_ps2();

    init_serial();
}
