
#include <screen.h>
#include <irq.h>

#include <sys/io.h>

#include <dev/serial.h>

uint8_t m_buf[4] = {0};
uint8_t m_off    =  0 ;

uint64_t m_x = 0;
uint64_t m_y = 0;

void draw_at(int x, int y, char val)
{
    char *loc = (char *) screen->p_loc;
    uint32_t ppl = screen->pix_per_line;
    uint32_t x_len = screen->x_len;

    int cursor = (y * ppl + x) * 4;
    uint32_t size = 30;

    
    for (uint32_t i = 0; i < size; i++) {
        for (uint32_t j = 0; j < size; j++) {
            loc[cursor + 0] = 0;
            loc[cursor + 1] = 0;
            loc[cursor + 2] = val;
            loc[cursor + 3] = 0;

            cursor += 4;
        }

        cursor += ((ppl - x_len) + (x_len - size)) * 4;
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

    if (m_buf[0] & (1 << 8) || m_buf[0] & (1 << 7))
        goto out; // overflow, skip

    int32_t d_x = m_buf[1];
    int32_t d_y = m_buf[2];

    if (m_buf[0] & (1 << 4))
        d_x |= 0xFFFFFF00;
    
    if (m_buf[0] & (1 << 5))
        d_y |= 0xFFFFFF00;

    my_put_nbr(d_x);
    write_serial(' ');
    my_put_nbr(d_y);
    write_serial('\n');

    //draw_at(m_x, m_y, 0);

    m_x += d_x;
    m_y -= d_y;
    
    draw_at(m_x, m_y, 255);

    out:
    end_of_interrupt(12);
}

void init_mouse()
{
    m_x = screen->x_len / 2;
    m_y = screen->y_len / 2;

    my_put_nbr(m_x);
    write_serial('\n');
    my_put_nbr(m_y);
    write_serial('\n');
    
    draw_at(m_x, m_y, 255);
}
