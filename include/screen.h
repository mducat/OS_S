/*
** EPITECH PROJECT, 2020
** OS_S
** File description:
** Screen func prototypes
*/

#ifndef _SCREEN_H
#define _SCREEN_H

#define CONSOLE_WIDTH   (80)
#define CONSOLE_HEIGHT  (25)
#define OS_S_SCREEN     ((void *)0xb8000)

int mvprint(int x, int y, char *str, char color);
void write_screen(char *str);
void clear(void);

#endif
