/*
** EPITECH PROJECT, 2019
** my_compute_power_it
** File description:
** hello
*/

#include "../my.h"
#include "bufferScreen.h"

void my_draw_square(framebuffer_t *buf, Vector2u_t position, unsigned int size, Color_t color)
{
    for (unsigned int x = 0; x < size; x++)
        for (unsigned int y = 0; y < size; y++)
            my_put_pixel_fast(buf, position.x+x, position.y+y, color);
}
