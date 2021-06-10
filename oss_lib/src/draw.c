
#include <oss.h>

void draw_rect(rect_t *rect, uint32_t color)
{
    int num = 10;

    asm volatile("int $0x30" :
                 : "a" (num),
                   "b" (rect),
                   "c" (color));
}

void draw_circle(circle_t *circle, uint32_t color)
{
    int num = 11;

    asm volatile("int $0x30" :
                 : "a" (num),
                   "b" (circle),
                   "c" (color));
}
