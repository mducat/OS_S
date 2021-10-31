/*
** EPITECH PROJECT, 2019
** my_compute_power_it
** File description:
** hello
*/

#include "../my.h"
#include "bufferScreen.h"
#include <string.h>

void my_clear_buffer(framebuffer_t *buf, Color_t color)
{
    memset(buf->pixels4, color.c, buf->width * buf->height*4);
}
