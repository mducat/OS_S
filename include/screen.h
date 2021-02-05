
#ifndef _SCREEN_H
#define _SCREEN_H

#include <kboot.h>

#define CONSOLE_WIDTH   (80)
#define CONSOLE_HEIGHT  (25)
#define OS_S_SCREEN     ((void *)0xb8000)

extern screen_t *screen;

int mvprint(int x, int y, char *str, char color);
void write_screen(char *str, int size);
void clear(void);
void change_mode(char mode);

#endif
