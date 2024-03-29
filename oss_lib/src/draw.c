
#include <oss.h>

void draw_rect(rect_t *rect, uint32_t color)
{
    int num = 10;

    LOAD3(num, rect, color);
    asm volatile("int $0x30");
}

void draw_circle(circle_t *circle, uint32_t color)
{
    int num = 11;

    LOAD3(num, circle, color)
    asm volatile("int $0x30");
}

void draw_fb(void *pixels, rect_t *rect)
{
    int num = 15;

    LOAD3(num, pixels, rect)
    asm volatile("int $0x30");
}

void draw_fb_win(int win_id, void *pixels, rect_t *rect)
{
    int num = 16;

    LOAD4(num, win_id, pixels, rect)
    asm volatile("int $0x30");
}
