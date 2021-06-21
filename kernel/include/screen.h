
#ifndef _SCREEN_H
#define _SCREEN_H

#include <types.h>

#define IS_PRINT(x)                (x >= ' ' && x <= '~')
#define RGBA_TO_COLOR(r, g, b, a)  (b | g << 8 | r << 16 | a << 24)

typedef struct vec_s {
    uint64_t x;
    uint64_t y;
} vec_t;

typedef struct rect_s {
    uint64_t x;
    uint64_t y;
    uint64_t dx;
    uint64_t dy;
} rect_t;

typedef struct circle_s {
    uint64_t x;
    uint64_t y;
    uint64_t d;
} circle_t;

#ifndef _OSS_LIB

    #include <kboot.h>

    typedef struct display_s {
        screen_t *screen;
        void     *back;
    } disp_t;

    typedef struct display_state_s {
        uint32_t front_color;
        uint32_t back_color;
        uint32_t line_color;
    } disp_state_t;

    extern volatile disp_t *disp;

    size_t write_screen(const char *buf, size_t count);
    void refresh(void);
    void clear(void);

    void draw_rect(rect_t *rec, uint32_t color);
    void draw_circle(circle_t *rec, uint32_t color);
    uint32_t get_screenX(void);
    uint32_t get_screenY(void);

#endif

#endif
