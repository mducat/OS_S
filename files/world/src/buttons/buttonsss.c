/*
** EPITECH PROJECT, 2019
** my_str_isprintable
** File description:
** hello
*/

#include "../world.h"
#include <my.h>

int interact_butons(world_t *world)
{
    (void)world;
    int tot = 0;
    //sfKeyboard_isKeyPressed(sfKeyZ) ? world->edi_y += 10, world->mv |= 1 : 0;
    //sfKeyboard_isKeyPressed(sfKeyS) ? world->edi_y -= 10, world->mv |= 1 : 0;
    //sfKeyboard_isKeyPressed(sfKeyQ) ? world->edi_x += 10, world->mv |= 1 : 0;
    //sfKeyboard_isKeyPressed(sfKeyD) ? world->edi_x -= 10, world->mv |= 1 : 0;
    button(world, 0) == 0x3 ? world->brush += 1 : 0;
    button(world, 1) == 0x3 ? world->brush > 0 ? world->brush -= 1 : 0 : 0;
    button(world, 2) == 0x3 ? world->brush_type = 1 : 0;
    button(world, 3) == 0x3 ? world->brush_type = 2 : 0;
    button(world, 4) == 0x3 ? world->brush_type = 3 : 0;
    button(world, 5) == 0x3 ? world->mv ^= 2 : 0;
    button(world, 6) == 0x3 ? world->sun += PI/12 : 0;
    button(world, 7) == 0x3 ? world->sun -= PI/12 : 0;
    button(world, 8) == 0x3 ? world->cs *= 1.5 : 0;
    button(world, 9) == 0x3 ? world->cs /= 1.5 : 0;
    for (int i = 0; i < world->buton_nb; i++)
        tot += button(world, i);
    return (tot);
}

char button(world_t *world, int b_nb)
{
    (void)world;
    (void)b_nb;
    //button_t *b = &world->edi_buttons[b_nb];
    //int vect[] = {b->pos_x, b->pos_y, b->size_x, b->size_y};
    //Vector2i_t m = sfMouse_getPositionRenderWindow(world->edi);
    char flag = 0x1;
    //char flag = 0;
    //(m.x > b->pos_x && m.y > b->pos_y &&
    //m.x < b->pos_x + b->size_x && m.y < b->pos_y + b->size_y) ? flag |= 0x1 : 0;
    //(flag & 0x1 && sfMouse_isButtonPressed(sfMouseLeft)) ? flag |= 0x2 : 0;
    //flag == 0x0 ? my_draw_rect(world->edi_buf, vect, b->color1) : 0;
    //flag == 0x1 ? my_draw_rect(world->edi_buf, vect, b->color2) : 0;
    //flag == 0x3 ? my_draw_rect(world->edi_buf, vect, b->color3) : 0;
    //sfText_setString(world->txt, b->txt);
    //sfText_setCharacterSize(world->txt, 20);
    //sfText_setFont(world->txt, world->font);
    //sfText_setFillColor(world->txt, sfWhite);
    //sfVector2f vect2[] = {b->pos_x+10, b->pos_y+10};
    //sfText_setPosition(world->txt, *vect2);
    //sfRenderWindow_drawText(world->edi, world->txt, 0);
    //flag == 0x3 ? b->on == 1 ? flag = 0x2 : (b->on = 1) : (b->on = 0);
    return (flag);
}
