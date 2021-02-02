
#include "screen.h"
#include "color.h"

void change_mode(char mode)
{
    asm volatile("movb %0,%%al\n\t"
                 "movb %1,%%ah\n\t"
                 "int $0x10"
                 :
                 :"r" (mode), "N" (0)
                 :);
}

//25 80 -> window size
int mvprint(int x, int y, char *str, char color)
{
    char *screen = OS_S_SCREEN;
    int str_p = 0;
    int screen_p = (x + y * CONSOLE_WIDTH);

    for (; str[str_p]; screen_p++, str_p++){
        if (str[str_p] == '\n'){
            screen_p += CONSOLE_WIDTH;
            screen_p -= screen_p % CONSOLE_WIDTH;
            str_p += 1;
        }
        screen[screen_p * 2] = str[str_p];
        screen[screen_p * 2 + 1] = color;
    }
    return (screen_p);
}

void write_screen(char *str, int size)
{
    char *screen = OS_S_SCREEN;
    int str_p = 0;
    static int screen_p = 0;

    for (; str_p < size; screen_p++, str_p++){
        if (str[str_p] == '\n'){
            screen_p += CONSOLE_WIDTH;
            screen_p -= screen_p % CONSOLE_WIDTH+1;
            continue;
        }
        screen_p = screen_p % (CONSOLE_WIDTH * CONSOLE_HEIGHT);
        screen[screen_p * 2] = str[str_p];
        screen[screen_p * 2 + 1] = OS_SCREEN_COLOR_WHITE;
    }
}
/*
void write_screen(char *str)
{
    static int cursor = 0;
    int x = cursor % CONSOLE_WIDTH;
    int y = cursor / CONSOLE_WIDTH;

    cursor = mvprint(x, y, str, OS_SCREEN_COLOR_WHITE);
}
*/
void clear(void)
{
    char *screen = OS_S_SCREEN;
    int x = 0;
    int y = 0;
    int screen_p = (x + y * CONSOLE_WIDTH);

    for (int i = 0; i < CONSOLE_HEIGHT; i++)
        for (int ii = 0; ii < CONSOLE_WIDTH; ii++, screen_p++)
            screen[screen_p * 2] = 0, screen[screen_p * 2 + 1] = OS_SCREEN_COLOR_BLACK;
}
