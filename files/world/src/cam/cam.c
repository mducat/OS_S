/*
** EPITECH PROJECT, 2019
** my_str_isprintable
** File description:
** hello
*/

#include "my.h"
#include "../world.h"

int main_cam(world_t *world)
{
    int size_x = world->x;
    int size_y = world->y;
    for (int i = 0; i < size_x*size_y/100; i++){
        int xy[] = {rand()%(size_x-2)+1, rand()%(size_y-2)+1, size_x, size_y};
        if (world->mesh[xy[0]][xy[1]] < -20)
            drop_water(world->mesh, xy, 0.5, 0);
    }
    world->mv |= take_movement_input(world->mat_start, world);
    float **points = mesh_to_points(world->mesh, size_x, size_y);
    float **points2 = rotate_points(points, world->mat_start);
    draw_mesh(world, points2+2);
    free_points(points2);
    free_points(points);
    //draw_window(world->cam, world->cam_buf);
    draw_window(world->cam_buf);
    world->mv &= 2;
    return (0);
}

int take_movement_input(float *mat_start, world_t *world)
{
    (void)mat_start;
    (void)world;
    int mv = 0;
    //static Vector2i_t mouse_o = {0, 0};
    //Vector2i_t mouse = sfMouse_getPositionRenderWindow(world->cam);
    //if (mouse.x < 0 || mouse.y < 0 || mouse.x > SCREEN_X || mouse.y > SCREEN_Y)
    //    return (0);
    //sfKeyboard_isKeyPressed(sfKeyZ) ? mat3_tz(mat_start, 1), mv = 1 : 0;
    //sfKeyboard_isKeyPressed(sfKeyS) ? mat3_tz(mat_start, -1), mv = 1 : 0;
    //sfKeyboard_isKeyPressed(sfKeyQ) ? mat3_tx(mat_start, 1), mv = 1 : 0;
    //sfKeyboard_isKeyPressed(sfKeyD) ? mat3_tx(mat_start, -1), mv = 1 : 0;
    //sfKeyboard_isKeyPressed(sfKeyE) ? mat3_ty(mat_start, 1), mv = 1 : 0;
    //sfKeyboard_isKeyPressed(sfKeyA) ? mat3_ty(mat_start, -1), mv = 1 : 0;
    //sfKeyboard_isKeyPressed(sfKeyW) ? mat3_rz(mat_start, PI/180), mv = 1 : 0;
    //sfKeyboard_isKeyPressed(sfKeyX) ? mat3_rz(mat_start, -PI/180), mv = 1 : 0;
    mat3_rrz(mat_start, PI/180);
    //if (sfMouse_isButtonPressed(sfMouseLeft)){
    //    float vx = mouse.x - mouse_o.x;
    //    float vy = mouse.y - mouse_o.y;
    //    mat3_rx(mat_start, vy/180), mat3_ry(mat_start, -vx/180), mv = 1;
    //}
    //mouse_o = mouse;
    return (mv);
}
