/*
** EPITECH PROJECT, 2019
** my_str_isprintable
** File description:
** hello
*/

#include <my.h>
#include <oss.h>
#include "world.h"

int min_i(int a, int b)
{
    return (a > b ? b : a);
}

void draw_window(framebuffer_t *fb)
{
    draw_fb(fb->pixels4, &fb->rect);
}

world_t *create_world(int x, int y)
{
    world_t *world = malloc(sizeof(world_t));
    world->x = x;
    world->y = y;
    printf("init map...\n");
    init_map(world);
    printf("init cam...\n");
    init_cam(world);
    printf("init edit...\n");
    init_edit(world);
    printf("init finished\n");
    return (world);
}

int main(int ac, char **av)
{
    int size = 64;
    if (ac == 2 && !my_strcmp(av[1], "-h")){
        help();
        return (0);
    } else if (ac == 2 && my_getnbr(av[1]) > 0) {
        size = my_getnbr(av[1]);
    }
    //srand(&size);
    grad = 0;
    sun_grid = 0;
    world_t *world = create_world(size, size);
    //sfEvent event;
    for (int frame_nb = 0; 1; frame_nb++){
        main_cam(world);
        main_edit(world);
        refresh();
    //    while (sfRenderWindow_pollEvent(world->edi, &event))
    //        if (event.type == sfEvtClosed)
    //            return (0);
    //    while (sfRenderWindow_pollEvent(world->cam, &event))
    //        if (event.type == sfEvtClosed)
    //            return (0);
    }
    return (0);
}
