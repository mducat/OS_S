
#include <screen.h>
#include <irq.h>

#include <sys/io.h>

uint8_t m_buf[3] = {0};
uint8_t m_off    =  0 ;

uint64_t m_x = 0;
uint64_t m_y = 0;

void irq12_handler(void)
{
    uint8_t a = inb(0x60);

    if (m_off == 0 && !(a & (1 << 3)))
        goto out;

    m_buf[m_off++] = a;
    m_off %= 3;

    if (m_off) 
        goto out;

    int8_t s_x = ((m_buf[0] >> 4) & 0x1) == 0 ? 1 : -1;
    int8_t s_y = ((m_buf[0] >> 5) & 0x1) == 0 ? 1 : -1;

    char *loc = (char *) screen->p_loc;
    //loc[(m_y * screen->pix_per_line + s_x) * 4 + 2] = 0;

    
    for (int i = 0; i < 20; i++)
        loc[(100 * screen->pix_per_line + 200 + i) * 4 + 2] = 255;

    m_x += m_buf[1] * s_x;
    m_y += m_buf[2] * s_y;
    
    loc[(m_y * screen->pix_per_line + s_x) * 4 + 2] = 255;

    out:
    end_of_interrupt(12);
}

void init_mouse()
{
    m_x = screen->x_len / 2;
    m_y = screen->y_len / 2;
}
