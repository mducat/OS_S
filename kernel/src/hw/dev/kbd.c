
#include <irq.h>
#include <kernel.h>
#include <screen.h>

#include <dev/kbd.h>

#include <sys/io.h>

void irq1_handler(void)
{
    end_of_interrupt(1);

    uint8_t status = inb(KBD_STATUS);
    static int next = 0;

    if ((status & 1) == 0)
        return;
    
    uint8_t input = inb(KBD_DATA);

    if (input == 0xF0) {
        next = 1;
        return;
    }

    if (input > 0x80) // -1
        return;

    if (!next) {
        char print = scan_code_set_2_fr[(uint8_t) input];

        send_tty(print);
    } else {
        next = 0;
    } 
}
