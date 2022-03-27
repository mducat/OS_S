/*
** EPITECH PROJECT, 2019
** my_compute_power_it
** File description:
** hello
*/

#include "frameBuffer.h"

void my_clear_buffer(framebuffer_t *buf, Color_t color)
{
    for (unsigned int i = 0; i < buf->width*buf->height; i++)
        buf->pixels4[i] = color.c;
}
