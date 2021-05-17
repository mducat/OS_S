
#ifndef _SCREEN_H
#define _SCREEN_H

#include <kboot.h>

#define CONSOLE_WIDTH   (80)
#define CONSOLE_HEIGHT  (25)

#define IS_PRINT(x)     (x >= ' ' && x <= '~')

typedef struct display_s {
    screen_t *screen;
    void     *back;
} disp_t;

typedef struct vec_s {
    uint64_t x;
    uint64_t y;
} vec_t;

extern volatile disp_t *disp;

size_t write_screen(const char *buf, size_t count);
void refresh(void);

#endif
