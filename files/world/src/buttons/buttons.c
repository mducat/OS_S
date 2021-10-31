/*
** EPITECH PROJECT, 2019
** my_str_isprintable
** File description:
** hello
*/

#include "../world.h"
#include <my.h>

void button0(world_t *world, int nb)
{
    button_t *buts = world->edi_buttons;
    buts[nb].pos_x = SCREEN_X - 150, buts[nb].pos_y = 100 + nb*60;
    buts[nb].size_x = 150, buts[nb].size_y = 50;
    Color_t c1;
    c1.r = 255;
    c1.g =  255;
    c1.b = 255;
    c1.a = 50;
    Color_t c2;
    c2.r = 50;
    c2.g =  50;
    c2.b = 50;
    c2.a = 0;
    Color_t c3;
    c3.r = 200;
    c3.g =  200;
    c3.b = 200;
    c3.a = 255;
    buts[nb].color1 = c1, buts[nb].color2 = c2, buts[nb].color3 = c3;
    buts[nb].txt = "Brush size +";
}

void button1(world_t *world, int nb)
{
    button_t *buts = world->edi_buttons;
    buts[nb].pos_x = SCREEN_X - 150, buts[nb].pos_y = 100 + nb*60;
    buts[nb].size_x = 150, buts[nb].size_y = 50;
    Color_t c1;
    c1.r = 255;
    c1.g = 255;
    c1.b = 255;
    c1.a = 50;
    Color_t c2;
    c2.r = 50;
    c2.g = 50;
    c2.b = 50;
    c2.a = 0;
    Color_t c3;
    c3.r = 200;
    c3.g = 200;
    c3.b = 200;
    c3.a = 255;
    buts[nb].color1 = c1, buts[nb].color2 = c2, buts[nb].color3 = c3;
    buts[nb].txt = "Brush size -";
    buts[nb].on = 0;
}

void button2(world_t *world, int nb)
{
    button_t *buts = world->edi_buttons;
    buts[nb].pos_x = SCREEN_X - 150, buts[nb].pos_y = 100 + nb*60;
    buts[nb].size_x = 150, buts[nb].size_y = 50;
    Color_t c1;
    c1.r = 255;
    c1.g = 255;
    c1.b = 255;
    c1.a = 50;
    Color_t c2;
    c2.r = 50;
    c2.g = 50;
    c2.b = 50;
    c2.a = 0;
    Color_t c3;
    c3.r = 200;
    c3.g = 200;
    c3.b = 200;
    c3.a = 255;
    buts[nb].color1 = c1, buts[nb].color2 = c2, buts[nb].color3 = c3;
    buts[nb].txt = "Water Drop";
    buts[nb].on = 0;
}

void button3(world_t *world, int nb)
{
    button_t *buts = world->edi_buttons;
    buts[nb].pos_x = SCREEN_X - 150, buts[nb].pos_y = 100 + nb*60;
    buts[nb].size_x = 150, buts[nb].size_y = 50;
    Color_t c1;
    c1.r = 255;
    c1.g = 255;
    c1.b = 255;
    c1.a = 50;
    Color_t c2;
    c2.r = 50;
    c2.g = 50;
    c2.b = 50;
    c2.a = 0;
    Color_t c3;
    c3.r = 200;
    c3.g = 200;
    c3.b = 200;
    c3.a = 255;
    buts[nb].color1 = c1, buts[nb].color2 = c2, buts[nb].color3 = c3;
    buts[nb].txt = "Height +";
    buts[nb].on = 0;
}

void button4(world_t *world, int nb)
{
    button_t *buts = world->edi_buttons;
    buts[nb].pos_x = SCREEN_X - 150, buts[nb].pos_y = 100 + nb*60;
    buts[nb].size_x = 150, buts[nb].size_y = 50;
    Color_t c1;
    c1.r = 255;
    c1.g = 255;
    c1.b = 255;
    c1.a = 50;
    Color_t c2;
    c2.r = 50;
    c2.g = 50;
    c2.b = 50;
    c2.a = 0;
    Color_t c3;
    c3.r = 200;
    c3.g = 200;
    c3.b = 200;
    c3.a = 255;
    buts[nb].color1 = c1, buts[nb].color2 = c2, buts[nb].color3 = c3;
    buts[nb].txt = "Height -";
    buts[nb].on = 0;
}
