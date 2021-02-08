
#include <gdt.h>
#include <irq.h>

#include <screen.h>
#include <kboot.h>

void freeze(void)
{
    __asm__ __volatile__ ("cli\n\r"
                          "hlt");
}

void write_debug(char *str)
{
    char *screen = (void *) 0xb80000;//0x8000000;// VGA = 0xb8000;

    for (int i = 0; str[i] != '\0'; i++){
        screen[i] = str[i];
    }
}

void draw_debug(void)
{
    uint32_t x_len = screen->x_len;
    uint32_t y_len = screen->y_len;
    uint32_t ppl = screen->pix_per_line;

    uint8_t *loc = screen->p_loc;

    uint32_t x_s = x_len / 3;
    uint32_t y_s = y_len / 3;
    int cursor = (x_s + y_s * ppl) * 4;
    
    for (uint32_t i = 0; i < y_s; i++) {
        for (uint32_t j = 0; j < x_s; j++) {
            loc[cursor + 0] = ( (j * 255) / x_s );
            loc[cursor + 1] = ( ( (x_s - j) * 255) /  x_s);
            loc[cursor + 2] = 120;
            loc[cursor + 3] = 0;

            cursor += 4;
        }

        cursor += ((ppl - x_len) + (2 * x_s)) * 4;
    }
}

void k_start(boot_t *data)
{
    if (data->sig != 0x42)
        freeze(); // kernel panic: Where was this booted from ??

    screen = data->screen;

    init_interrupts();

    while (1);
    //freeze();     // prevent $pc from exiting kernel

    //init_malloc((void *)0x100000, (void *)0x8000000);
}
