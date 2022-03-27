
#include <screen.h>
#include <irq.h>

#include <sys/io.h>

#include <dev/serial.h>

uint8_t m_buf[4] = {0};
uint8_t m_off    =  0 ;

uint64_t m_x = 0;
uint64_t m_y = 0;

#include "frameBuffer.h"

void mouseClickCallBack(int x, int y, int32_t type) {
    (void)x, (void)y, (void)type;
    
}

void (*mouse_click_call_back)(int x, int y, int32_t type) = mouseClickCallBack;

void draw_mouse(int x, int y) {
    static framebuffer_t *buf = 0;
    //static framebuffer_t *bufBack = 0;
    if (!buf) {
        buf = framebuffer_create_from_uint32_array(disp->screen->x_len, disp->screen->y_len, (uint32_t *)disp->screen->p_loc);
        //bufBack = framebuffer_create_from_uint32_array(disp->screen->x_len, disp->screen->y_len, (uint32_t *)disp->back);
    }
    
    static uint32_t lastColor = 0;

    static int lastPos[2] = {-1, -1};
    if (lastPos[0] != -1 && lastPos[1] != -1) {
        if (buf->pixels4[(lastPos[1]*buf->width + lastPos[0])] == lastColor) {
            Vector2f_t cursorLines[] = {
                {13.3333, 17},
                {0, 0},
                {0, 20},
                {8.88889, 14.6667},
                {2.22222, 6.16667},
                {2.22222, 16.1667},
                {13.3333, 17},
            };

            for (int i = 0; i < 7; i++) {
                cursorLines[i].x += lastPos[0];
                cursorLines[i].y += lastPos[1];
            }
            my_xor_lines(buf, cursorLines, 7);
            //my_xor_lines(bufBack, cursorLines, 7);
        }
    }
    lastPos[0] = x;
    lastPos[1] = y;

    {
        Vector2f_t cursorLines[] = {
            {13.3333, 17},
            {0, 0},
            {0, 20},
            {8.88889, 14.6667},
            {2.22222, 6.16667},
            {2.22222, 16.1667},
            {13.3333, 17},
        };

        for (int i = 0; i < 7; i++) {
            cursorLines[i].x += x;
            cursorLines[i].y += y;
        }
        my_xor_lines(buf, cursorLines, 7);
        lastColor = buf->pixels4[(y*buf->width + x)];

        //my_xor_lines(bufBack, cursorLines, 7);
    }
}

void irq12_handler(void)
{
    uint8_t a = inb(0x60);

    if (m_off == 0 && !(a & (1 << 3)))
        goto out;

    m_buf[m_off++] = a;
    m_off %= 3;

    if (m_off) 
        goto out;

    if (m_buf[0] & (1 << 6) || m_buf[0] & (1 << 7))
        goto out; // overflow, skip

    int32_t d_x = m_buf[1];
    int32_t d_y = m_buf[2];

    if (m_buf[0] & (1 << 4))
        d_x |= 0xFFFFFF00;
    
    if (m_buf[0] & (1 << 5))
        d_y |= 0xFFFFFF00;





    m_x += d_x;
    m_y -= d_y;


    draw_mouse(m_x, m_y);

    if (mouse_click_call_back) 
        mouse_click_call_back(m_x, m_y, m_buf[0]);


    out:
    end_of_interrupt(12);
}

void init_mouse()
{
    m_x = disp->screen->x_len / 2;
    m_y = disp->screen->y_len / 2;

    //my_put_nbr(m_x);
    //write_serial('\n');
    //my_put_nbr(m_y);
    //write_serial('\n');
    
    //draw_at(m_x, m_y, 255, 0, 0);
}
