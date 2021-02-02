
/*
#include "screen.h"
#include "kernel.h"
#include "malloc.h"
#include "segments.h"
#include "interrupts.h"
*/

void freeze(void)
{
    __asm__ __volatile__ ("hlt"); // "cli\n\r"
}

void k_start(void *data)
{
    
    freeze();

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
