
#include <irq.h>

#include <sys/io.h>

#include <dev/ps2.h>
#include <dev/mouse.h>

void write_ps2(uint8_t port, uint8_t data)
{
    uint16_t tm_out = 10000;

    while ((inb(PS2_CMD) & 2) && tm_out-- != 0) {
        __asm__ __volatile__ ("nop");
    }

    outb(port, data);
}

uint8_t read_ps2(uint8_t port)
{
    uint16_t tm_out = 10000;

    while (!(inb(PS2_CMD) & 1) && tm_out-- != 0) {
        __asm__ __volatile__ ("nop");
    }

    return inb(port);
}

void write_mouse(uint8_t data)
{
    write_ps2(PS2_CMD, 0xD4);
    write_ps2(PS2_DATA, data);
}

uint8_t ps2_ack(void)
{
    return (read_ps2(PS2_DATA) != PS2_ACK);
}

void ps2_wconf(uint8_t conf)
{
    write_ps2(PS2_CMD, PS2_CWD_W_CONF);
    write_ps2(PS2_DATA, conf);
}

uint8_t ps2_rconf(void)
{
    write_ps2(PS2_CMD, PS2_CWD_R_CONF);
    return read_ps2(PS2_DATA);
}

void init_ps2()
{
    deactivate_interrupts();

    // disable mouse + kbd
    write_ps2(PS2_CMD, PS2_CWD_DOWN_FIRST);
    write_ps2(PS2_CMD, PS2_CWD_DOWN_SECOND);

    // ignore buffer
    inb(PS2_DATA);

    // Disable irq + translation in config
    uint8_t cfg = ps2_rconf();

    cfg |= 1 << 2;
    cfg &= ~(1 | (1 << 1) | (1 << 6));

    ps2_wconf(cfg);

    // perform controller self test
    write_ps2(PS2_CMD, PS2_CWD_SELF_TEST);

    if (read_ps2(PS2_DATA) != PS2_STEST_REP)
        return;

    // Rewrite conf byte in case of reset
    ps2_wconf(cfg);

    // Determine the number of channels
    write_ps2(PS2_CMD, PS2_CWD_UP_SECOND);

    uint8_t dev_1 = 1;
    uint8_t dev_2 = 1;

    cfg = ps2_rconf();

    if (cfg & (1 << 5)) {
        dev_2 = 0; // no double channel
    } else {
        write_ps2(PS2_CMD, PS2_CWD_DOWN_SECOND); // disable 2
    }

    // perform interface tests
    write_ps2(PS2_CMD, PS2_CWD_TEST_FIRST);

    if (read_ps2(PS2_DATA))
        dev_1 = 0;

    if (dev_2) {
        write_ps2(PS2_CMD, PS2_CWD_TEST_SECOND);

        if (read_ps2(PS2_DATA))
            dev_2 = 0;
    }

    // enable devices
    if (dev_1) {
        write_ps2(PS2_CMD, PS2_CWD_UP_FIRST);
        cfg |= 1;
        cfg &= ~ (1 << 4);
    }

    if (dev_2) {
        write_ps2(PS2_CMD, PS2_CWD_UP_SECOND);
        cfg |= (1 << 1);
        cfg &= ~ (1 << 5);
    }

    ps2_wconf(cfg);

    // reset devices
    write_ps2(PS2_CMD, PS2_RESET);
    ps2_ack();
    
    write_mouse(PS2_RESET);

    ps2_ack();
    read_ps2(PS2_DATA); // expect 0xAA
    read_ps2(PS2_DATA); // expect 0x00 ?

    // enable packet streaming
    write_mouse(0xF4);
    ps2_ack();


    // CONF PARAMETERS

    write_mouse(0xF3); // set sample rate
    ps2_ack();
    write_mouse(80);
    ps2_ack();

    write_mouse(0xE8); // set resolution
    ps2_ack();
    write_mouse(0);
    ps2_ack();

    write_mouse(0xE7); // disable scaling
    ps2_ack();

    
    // configure software-side
    init_mouse();

    activate_interrupts();
}
