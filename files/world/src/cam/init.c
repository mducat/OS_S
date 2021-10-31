/*
** EPITECH PROJECT, 2019
** my_str_isprintable
** File description:
** hello
*/

#include "../world.h"

void init_map(world_t *world)
{
    int size_x = world->x;
    int size_y = world->y;
    printf("|   init mesh...\n")
    float **mesh = create_mesh(world->x, world->y, 7);
    printf("|   init water...\n")
    for (int i = 0; i < size_x*size_y; i++){
        int xy[] = {rand()%(size_x-2)+1, rand()%(size_y-2)+1, size_x, size_y};
        drop_water(mesh, xy, 0.5, 0);
    }
    world->mesh = mesh;
    world->sun = 0;
    world->cs = SCREEN_X/(world->x)*1.5;
}

void init_cam(world_t *world)
{
    printf("|   init matrixs...\n");
    float *mat_start = mat3_init();
    mat3_ttx(mat_start, world->x/2);
    mat3_tty(mat_start, world->y/2);
    mat3_ry(mat_start, -0.01);
    mat3_rx(mat_start, PI*0.75);
    mat3_tz(mat_start, -100);
    world->mat_start = mat_start;
    //sfVideoMode mode = {SCREEN_X, SCREEN_Y, 32};
    //world->cam = sfRenderWindow_create(mode, "camera", sfClose, 0);
    printf("|   init framebuffer...\n");
    world->cam_buf = framebuffer_create(SCREEN_X, SCREEN_Y);
    world->cam_buf->x = SCREEN_X;
    world->cam_buf->y = 0;
    world->mv = 1;
}
