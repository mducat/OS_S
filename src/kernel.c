/*
** EPITECH PROJECT, 2019
** OSS
** File description:
** Kernel main file
*/

#include "kernel.h"

//25 80 -> window size
int mvprint(int x, int y, char *str, char color)
{
    char *screen = (void *) 0xb8000;
    int str_p = 0;
    int screen_p = (x+y*80);

    for (; str[str_p]; screen_p++, str_p++){
        if (str[str_p] == '\n'){
            screen_p += 80;
            screen_p -= screen_p % 80;
            str_p += 1;
        }
        screen[screen_p * 2] = str[str_p];
        screen[screen_p * 2 + 1] = color; // Color 0x9 = light blue
    }
    return (screen_p);
}

void write_screen(char *str)
{
    static int cursor;
    int x = cursor % CONSOLE_WIDTH;
    int y = cursor / CONSOLE_WIDTH;

    cursor = mvprint(x, y, str, 0x07);
}

void kernel_main(void)
{
    mvprint(0, 0, GOODENOUGHT, 0x09);
    /*
    write_screen(GOODENOUGHT);
    write_screen("florent\nest\nobese\nquand\nil\nparle de david");*/
}
