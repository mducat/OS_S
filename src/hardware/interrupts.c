
#include <interrupts.h>
#include <segments.h>

#include <screen.h>
#include <malloc.h>
#include <kernel.h>

// TODO refactor this ugly file

void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %0,%1"
                 :
                 : "a" (value), "Nd" (port)
                 :);
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;

    asm volatile("inb %1,%0"
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

extern void write_port(unsigned short port, unsigned char data);


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
    outb(MASTER_DATA, 0xFD); // FD
    outb(SLAVE_DATA, 0xEF); // EF
}

void register_int_handler(uint8_t *idt, uint8_t irq, void (*handler)(void), uint8_t type)
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
    //draw_debug();
    //write_screen("I0", 2);
    end_of_interrupt(0);
}

void irq1_handler(void)
{
    draw_debug();

    char *test = (char *) 0x100042;
    *test = (*test) + 1;

    uint8_t a = inb(KBD_STATUS);
    uint8_t input = inb(KBD_DATA);


    char *test2 = (char *) 0x100043;
    *test2 = a;

    char *test3 = (char *) 0x100043;
    *test3 = input;


    char *loc = (char *) screen->p_loc;
    for (int i = 0; i < 20; i++)
        loc[(50 * screen->pix_per_line + input * 20 + i) * 4 + 2] = 255;
    
    end_of_interrupt(1);
}

uint8_t m_buf[3] = {0};
uint8_t m_off    =  0 ;

uint64_t m_x = 0;
uint64_t m_y = 0;

void irq12_handler(void)
{
    uint8_t a = inb(0x60);

    if (!(a & 0x8))
        goto out;

    m_buf[m_off++] = a;
    m_off %= 3;

    if (m_off) 
        goto out;

    int8_t s_x = ((m_buf[0] >> 4) & 0x1) == 0 ? -1 : 1;
    int8_t s_y = ((m_buf[0] >> 5) & 0x1) == 0 ? -1 : 1;

    char *loc = (char *) screen->p_loc;
    loc[(m_y * screen->pix_per_line + s_x) * 4 + 2] = 0;

    
    for (int i = 0; i < 20; i++)
        loc[(100 * screen->pix_per_line + 200 + i) * 4 + 2] = 255;

    m_x += m_buf[1] * s_x;
    m_y += m_buf[2] * s_y;
    
    loc[(m_y * screen->pix_per_line + s_x) * 4 + 2] = 255;

    out:
    end_of_interrupt(12);
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

void load_idt_as(uint8_t* idt);

void load_idt(uint8_t* idt)
{
    /*uint32_t ptr[2];
    ptr[0] = (IDT_LEN) << 16;
    ptr[1] = ((uintptr_t) idt) & 0xFFFFFFFF;
    asm volatile("lidt (%0)"
                 : 
                 : "r" (((char *) ptr) + 2)
                 :);*/

    uint32_t ptr[4] = {0};
    uintptr_t idt_loc = (uintptr_t) idt;

    ptr[0] = (IDT_LEN) | (idt_loc & 0xFFFF) << 16;
    ptr[1] = (idt_loc & 0xFFFF0000) >> 16;
    asm volatile("lidt %0"
                 : 
                 : "m" (ptr)
                 :);
    //load_idt_as(idt);
    
    activate_interrupts();
}

void int_ignore(void)
{
    return;
}

extern void irq1_caller(void);
extern void irq12_caller(void);


void init_mouse(void)
{
    
    //outb(0x64, 0xAD);
    //outb(0x64, 0xA7); // disable mouse + kbd

    while (inb(0x60) & 0x1); // flush buffer

    outb(0x64, 0xA8); // enable mouse

    outb(0x64, 0x20); // read cur config
    uint8_t cfg = inb(0x60);

    cfg |= 1 << 1; // enable mouse port interrupt

    outb(0x64, 0x60);
    outb(0x60,  cfg); // set new config

    outb(0x64, 0xD4);

    outb(0x60, 0xF4); // enable packet send
    while (inb(0x60) != 0xFA); /* Wait for ACK from mouse... */

    // TODO Determine if mouse is plugged + perform hardware tests
    
    // outb(0x64, 0xAE); // enable kbd

    m_x = screen->x_len / 2;
    m_y = screen->y_len / 2;
}

void init_interrupts(void)
{
    uint8_t *idt = (uint8_t *) 0x1000000;//[IDT_LEN];

    memset(idt, 0, IDT_LEN * 16);
    //end_of_interrupt(1);
    //for (uint8_t i = 0; i < 255; i++)
    //    register_int_handler(idt, i, int_ignore, INT_GATE);

    register_int_handler(idt, 0x21, irq1_caller, INT_GATE);
    register_int_handler(idt, 0x2c, irq12_caller, INT_GATE);

    remap_pic();

    load_idt(idt);
    init_mouse();
}
