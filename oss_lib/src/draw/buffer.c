/*
** EPITECH PROJECT, 2019
** my_compute_power_it
** File description:
** hello
*/

#include "my.h"
#include "frameBuffer.h"
#include "oss.h"

framebuffer_t *framebuffer_create(unsigned int width, unsigned int height)
{
    framebuffer_t *buf = malloc(sizeof(struct framebuffer));

    buf->width = width;
    buf->height = height;
    buf->pixels = malloc(width*height*sizeof(int32_t)*4);
    Color_t c;
    c.r = 0, c.g = 0, c.b = 0, c.a = 0;
    my_clear_buffer(buf, c);
    return (buf);
}

void framebuffer_destroy(framebuffer_t *framebuffer)
{
    free(framebuffer->pixels);
    free(framebuffer);
}

void my_put_pixel(framebuffer_t *framebuffer, unsigned int x, unsigned int y, Color_t color)
{
    if (x < framebuffer->width && y < framebuffer->height){
        framebuffer->pixels[(y*framebuffer->width + x)*4] = color.r;
        framebuffer->pixels[(y*framebuffer->width + x)*4 + 1] = color.g;
        framebuffer->pixels[(y*framebuffer->width + x)*4 + 2] = color.b;
        framebuffer->pixels[(y*framebuffer->width + x)*4 + 3] = color.a;
    }
}

void my_put_pixel_fast(framebuffer_t *framebuffer, unsigned int x, unsigned int y, Color_t color) {
    if (x < framebuffer->width && y < framebuffer->height)
        framebuffer->pixels4[(y*framebuffer->width + x)] = color.c;
}

void my_fill_buffer(framebuffer_t *buf, Color_t color)
{
    int size = buf->width * buf->height;

    for (int i = 0; i < size; i++){
        buf->pixels[i*4] = color.r;
        buf->pixels[i*4 + 1] = color.g;
        buf->pixels[i*4 + 2] = color.b;
        buf->pixels[i*4 + 3] = color.a;
    }
}

void my_blur_buffer(framebuffer_t *buf, float ratio)
{
    int len = buf->width*buf->height*sizeof(int32_t)*4;

    for (int i = 0; i < len; i++)
        buf->pixels[i] *= ratio;
}