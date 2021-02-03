
/*
#include "screen.h"
#include "kernel.h"
#include "malloc.h"
#include "segments.h"
#include "interrupts.h"
*/

#include <kboot.h>

void freeze(void)
{
    __asm__ __volatile__ ("hlt"); // "cli\n\r"
}

void write_screen(char *str)
{
    char *screen = (void *) 0xb80000;//0x8000000;// VGA = 0xb8000;

    for (int i = 0; str[i] != '\0'; i++){
        screen[i] = str[i];
    }
}

void k_start(boot_t *data)
{
    if (data->sig != 0x42)
        freeze(); // kernel panic: Where was this booted from ??

    uint32_t x_len = data->screen->x_len;
    uint32_t y_len = data->screen->y_len;
    uint32_t ppl = data->screen->pix_per_line;

    uint8_t *loc = data->screen->p_loc;
    int cursor = x_len / 3 + (y_len / 3) * ppl;
    
    for (uint32_t i = y_len / 3; i < 2 * y_len / 3; i++) {
        for (uint32_t j = x_len / 3; j < 2 * x_len / 3; j++) {
            loc[cursor + 0] = 255;
            loc[cursor + 1] = 255;
            loc[cursor + 2] = 255;
            loc[cursor + 3] =   0;

            cursor += 4;
        }

        cursor += (ppl - x_len) + (2 * x_len / 3);
    }
    
    freeze();     // prevent $pc from exiting kernel

    //setup_gdt();
    //init_interrupts();
    //init_malloc((void *)0x100000, (void *)0x8000000);
    
    //change_mode(0x13);

/*    char *str1 = malloc(7);
    strdup("abcdef", str1);
    char *str2 = malloc(11);
    strdup("ghijqlmnop", str2);
    char *str3 = malloc(11);
    strdup("qrstuvwxyz", str3);
    char *str4 = malloc(11);
    strdup("0123456789", str4);
    //write_screen("florent\nest\nobese\nquand\nil\nparle de david");

    char *print = my_putnbr_base(str1, "0123456789ABCDEF");
    write_screen(print, my_strlen(print));
    write_screen("|", 1);
    write_screen(str1, 7);
    write_screen("|\n", 2);

    print = my_putnbr_base(str2, "0123456789ABCDEF");
    write_screen(print, my_strlen(print));
    write_screen("|", 1);
    write_screen(str2, 11);
    write_screen("|\n", 2);

    print = my_putnbr_base(str3, "0123456789ABCDEF");
    write_screen(print, my_strlen(print));
    write_screen("|", 1);
    write_screen(str3, 11);
    write_screen("|\n", 2);

    print = my_putnbr_base(str4, "0123456789ABCDEF");
    write_screen(print, my_strlen(print));
    write_screen("|", 1);
    write_screen(str4, 11);
    write_screen("|\n", 2);*/

    //mem_print(40, 0, str1, 256);
    //char *ptr = 0x1D * 8;
    //mem_print(40, 0, ptr, 256);
    //mem_print(40, 0, 1, 256);
    //mvprint(0,0,GOODENOUGHT, 0x2);
}
