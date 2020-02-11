/*
** EPITECH PROJECT, 2019
** OSS
** File description:
** Kernel main file
*/

#include "kernel.h"
//25 80 -> window size
void mvprint(int x, int y, char *str, char color)
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
}


void write_screen(char *str)
{
    char a[2];
    static int actual_x = 0;
    static int actual_y = 0;

    a[1] = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '\n') {
            actual_y = (actual_y > 24) ? 0 : actual_y + 1;
            actual_x = 0;
            i++;
        }
        a[0] = str[i];
        mvprint(actual_x, actual_y, &a, 0x07);
        (actual_x > 79) ? actual_x = 0, actual_y++ : actual_x + 1;
    }
}

void kernel_main(void)
{
    mvprint(0, 0, GOODENOUGHT, 0x09);
    /*
    write_screen(GOODENOUGHT);
    write_screen("florent\nest\nobese\nquand\nil\nparle de david");*/
}
