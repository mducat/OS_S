/*
** EPITECH PROJECT, 2019
** my_compute_power_it
** File description:
** hello
*/

#include "my.h"
#include "frameBuffer.h"

void my_draw_circle(framebuffer_t *buf, Vector2u_t position, unsigned int size, Color_t color)
{
    double max = size*size;
    for (unsigned int x = 0; x <= size; x++){
        double size_x = (x-size)*(x-size);
        unsigned int y = 0;
        for (; (size_x + (y-size)*(y-size)) > max; y++);
        for (; y <= size; y++){
            my_put_pixel_fast(buf, position.x-size + x,
            position.y-size + y, color);
            my_put_pixel_fast(buf, position.x+size - x,
            position.y-size + y, color);
            my_put_pixel_fast(buf, position.x-size + x,
            position.y+size - y, color);
            my_put_pixel_fast(buf, position.x+size - x,
            position.y+size - y, color);
        }
    }
}
