
#include <string.h>
#include <screen.h>
#include <font.h>

void print_char_at(vec_t pos, char c)
{
    const unsigned char *pix = font.Bitmap + (c - 31) * 16;
    uint32_t color = 0x00FFFFFF;

    uint32_t *screen = (uint32_t *) disp->back;
    uint32_t ppl = disp->screen->pix_per_line;

    vec_t scr_p = {
        .x = pos.x * ( 8 + 1) + 10,
        .y = pos.y * (16 + 2) + 5,
    };

    screen += scr_p.y * ppl + scr_p.x;

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8; j++)
            *screen++ = (pix[i] >> (7 - j)) & 1 ? color : 0;

        screen += ppl - 8;
    }
}

vec_t move_cursor(vec_t pos)
{
    pos.x += 1;

    if (pos.x >= disp->screen->x_len / (8 + 2)) {
        pos.x = 0;
        pos.y += 1;
    }
    
    return pos;
}

size_t write_screen(const char *buf, size_t count)
{
    static vec_t pos = {.x = 0, .y = 0};
    size_t displayed = 0;

    while (count--) {
        char current = *buf++;

        if (IS_PRINT(current)) {
            print_char_at(pos, current);

            displayed++;
            pos = move_cursor(pos);

            continue;
        }
        
        switch (current) {
        case 3:
            //clear();

            pos.y = 0;
            pos.x = 0;
            break;
        case '\n':
            pos.y += 1;
            pos.x  = 0;

            displayed++;
            break;
        case '\b':
            pos.x = (pos.x == 0 ? 0 : pos.x - 1);
            print_char_at(pos, ' ');

            displayed++;
            break;
        }
    }
    
    return displayed;
}

void *memcpy8(void *dest, const void *src, size_t n)
{
    uint64_t *p_dest = (uint64_t *) dest;
    uint64_t *p_src  = (uint64_t *) src;

    while ((n -= 8) >0)
        *p_dest++ = *p_src++;
    return dest;
}

void refresh(void)
{
    memcpy8(disp->screen->p_loc, disp->back, disp->screen->buf_size);
}

void clear(void)
{
    memset(disp->back, 0, disp->screen->buf_size);
}
