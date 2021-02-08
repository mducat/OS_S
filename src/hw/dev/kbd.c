
#include <irq.h>
#include <kernel.h>
#include <screen.h>

#include <dev/kbd.h>

#include <sys/io.h>

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
