/*
** EPITECH PROJECT, 2019
** OSS
** File description:
** Kernel main file
*/

#include "screen.h"
#include "kernel.h"

void kernel_main(void)
{
    //mvprint(0, 0, GOODENOUGHT, OS_SCREEN_COLOR_DARK_BLUE);
    //clear();

    //mvprint(0, 0, GOODENOUGHT, 0x09);
    write_screen(GOODENOUGHT);
    write_screen("florent\nest\nobese\nquand\nil\nparle de david");
}
