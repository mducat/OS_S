
#include <string.h>
#include <screen.h>
#include <font.h>

void print_char_at(vec_t pos, char c, disp_state_t *state)
{
    const unsigned char *pix = font.Bitmap + (c - 31) * 16;
    uint32_t front = state->front_color;
    uint32_t back  = state->back_color;
    uint32_t line  = state->line_color;

    uint32_t *screen = (uint32_t *) disp->back;
    uint32_t ppl = disp->screen->pix_per_line;

    vec_t scr_p = {
        .x = pos.x * ( 8 + 1) + 10,
        .y = pos.y * (16 + 2) + 5,
    };

    screen += scr_p.y * ppl + scr_p.x;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8 + 1; j++)
            *screen++ = (pix[i] >> (7 - j)) & 1 ? front : back;

        screen += ppl - 8 - 1;
    }

    for (int j = 0; j < 8 + 1; j++)
        *screen++ = line;
}

vec_t move_cursor(vec_t pos)
{
    pos.x += 1;

    if (pos.x >= disp->screen->x_len / (8 + 1)) {
        pos.x = 0;
        pos.y += 1;
    }

    // loop to top 
    if (pos.y >= 50) {
        pos.y = 0;
    }

    return pos;
}

size_t write_screen(const char *buf, size_t count)
{
    size_t displayed = 0;

    static vec_t pos = {.x = 0, .y = 0};
    static disp_state_t state = {
        .front_color = 0x00FFFFFF,
        .back_color  = 0x00000000,
        .line_color  = 0x00000000
    };


    static int is_back = 0;

    static uint32_t read_color = 0;

    static uint8_t idx = 0;
    static uint8_t placeholder = 0;


    while (count--) {
        char current = *buf++;

        if (IS_PRINT(current) && !is_back) {
            print_char_at(pos, current, &state);

            displayed++;
            pos = move_cursor(pos);

            continue;
        } else if (IS_PRINT(current)) {

            if (current >= '0' && current <= '9') {
                placeholder *= 10;
                placeholder += current - '0';
                continue;
            }

            if (current == ',') {
                
                if (idx <= 2)
                    read_color |= placeholder << ((2 - idx++) * 8);

                placeholder = 0;
                continue;
            }
            
            if (current == ';') {

                switch (placeholder) {
                case 0:
                    state.back_color = read_color;
                    break;
                case 1:
                    state.front_color = read_color;
                    break;
                case 2:
                    state.line_color = read_color;
                    break;
                }

                placeholder = 0;
                read_color = 0;
                is_back = 0;
                idx = 0;
            }

        }
        
        switch (current) {
        case 27:
            is_back = 1;
            break;
        case 3:
            //clear();

            pos.y = 0;
            pos.x = 0;
            break;
        case '\n':
            pos.y += 1;
            pos.x  = 0;

            displayed++;

            
            {// clear next line on \n
                disp_state_t clear = {
                    .front_color = 0x00FFFFFF,
                    .back_color  = 0x00000000,
                    .line_color  = 0x00000000
                };
                pos.y++;
                for (uint32_t i = 0; i < disp->screen->x_len / (8 + 1); i++, pos.x++) {
                    print_char_at(pos, ' ', &clear);
                }
                pos.y--;
                pos.x = 0;
            }

            break;
        case '\b':
            pos.x = (pos.x == 0 ? 0 : pos.x - 1);
            print_char_at(pos, ' ', &state);

            displayed++;
            break;
        }
    }
    
    return displayed;
}

void draw_rect(rect_t *rect, uint32_t color)
{
    uint32_t *screen = (uint32_t *) disp->back;
    uint32_t ppl = disp->screen->pix_per_line;

    screen += rect->y * ppl + rect->x;

    for (uint64_t i = 0; i < rect->dy; i++) {
        for (uint64_t j = 0; j < rect->dx; j++)
            *screen++ = color;

        screen += ppl - rect->dx;
    }
}

void draw_fb(void *pixels, rect_t *rect)
{
    uint32_t *screen = (uint32_t *) disp->back;
    uint32_t *cursor = (uint32_t *) pixels;
    uint32_t ppl = disp->screen->pix_per_line;

    screen += rect->y * ppl + rect->x;

    for (uint64_t i = 0; i < rect->dy; i++) {
        for (uint64_t j = 0; j < rect->dx; j++)
            *screen++ = *cursor++;

        screen += ppl - rect->dx;
    }
}

void draw_circle(circle_t *circle, uint32_t color)
{
    uint32_t *screen = (uint32_t *) disp->back;
    uint32_t ppl = disp->screen->pix_per_line;

    vec_t scr_p = {
        .x = circle->x - circle->d / 2,
        .y = circle->y - circle->d / 2,
    };

    vec_t scr_m = {
        .x = circle->x + circle->d / 2,
        .y = circle->y + circle->d / 2,
    };

    uint64_t d = 2 * circle->d / 2;
    int p = -circle->d / 2;
    int k = -circle->d / 2;
    int dsq = (int) (circle->d / 2) * (circle->d / 2);

    screen += scr_p.y * ppl + scr_p.x;

    for (uint64_t i = scr_p.y; i < scr_m.y; i++, p++) {
        for (uint64_t j = scr_p.x; j < scr_m.x; j++, k++)
            *screen++ = (p * p + k * k) < dsq ? color : 0;

        screen += ppl - d;
        k = -circle->d / 2;
    }
}

typedef struct _u128 {
    uint64_t a;
    uint64_t b;
} u128;

typedef struct _u256 {
    u128 a;
    u128 b;
} u256;

typedef struct _u512 {
    u256 a;
    u256 b;
} u512;

typedef struct _u1024 {
    u512 a;
    u512 b;
} u1024;

typedef struct _u2048 {
    u1024 a;
    u1024 b;
} u2048;

typedef struct _u4096 {
    u2048 a;
    u2048 b;
} u4096;

void *memcpy8(void *dest, const void *src, size_t n)
{
    u2048 *p_dest = (u2048 *) dest;
    u2048 *p_src  = (u2048 *) src;

    while ((n -= 256) > 0)
        *p_dest++ = *p_src++;
    return dest;
}

u2048 v_n = {0};

void *memset8(void *dest, int val, size_t n)
{
    (void)val;
    u2048 *p_dest = (u2048 *) dest;

    while ((n -= 256) > 0)
        *p_dest++ = v_n;
    return dest;
}

void refresh(void)
{
    memcpy8(disp->screen->p_loc, disp->back, disp->screen->buf_size);
}

void clear(void)
{
    memset8(disp->back, 0, disp->screen->buf_size);
}

uint32_t get_screenX(void)
{
    screen_t *screen = (screen_t *) disp->screen;
    return screen->x_len; 
}

uint32_t get_screenY(void)
{
    screen_t *screen = (screen_t *) disp->screen;
    return screen->y_len;
}
