/*
** EPITECH PROJECT, 2019
** my_compute_power_it
** File description:
** hello
*/

#include "my.h"
#include "frameBuffer.h"


void my_draw_rect(framebuffer_t *buf, int *pos_size, Color_t color)
{
    for (int x = 0; x < pos_size[2]; x++)
        for (int y = 0; y < pos_size[3]; y++)
            my_put_pixel_fast(buf, pos_size[0]+x, pos_size[1]+y, color);
}
