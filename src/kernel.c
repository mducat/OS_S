/*
** EPITECH PROJECT, 2019
** OSS
** File description:
** Kernel main file
*/

#include "screen.h"
#include "kernel.h"
#include "malloc.h"

void kernel_main(void *data)
{
    //mvprint(0, 0, GOODENOUGHT, OS_SCREEN_COLOR_DARK_BLUE);
    //mvprint(0, 0, GOODENOUGHT, 0x09);
    /*write_screen(GOODENOUGHT, my_strlen(GOODENOUGHT));
    //write_screen("florent\nest\nobese\nquand\nil\nparle de david");
    clear();
    */
    init_malloc((void *)0x100000, (void *)0x8000000);
    char *str1 = malloc(7);
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
    write_screen("|\n", 2);

    mem_print(40, 0, str1, 256);

}
