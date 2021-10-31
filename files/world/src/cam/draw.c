/*
** EPITECH PROJECT, 2019
** my_str_isprintable
** File description:
** hello
*/

#include "../world.h"
char **sun_grid;

char did_we_draw(Vector2f_t **vecs, float **points, int i, int y)
{
    if (i % y == 0 || points[i][2] < 0  || points[i][0] > SCREEN_X ||
        points[i][0] < 0 || points[i][1] > SCREEN_Y || points[i][1] < 0)
        return (1);
    vecs[0]->x = points[i][0], vecs[0]->y = points[i][1];
    if (vecs[0]->x > SCREEN_X || vecs[0]->x < 0 || vecs[0]->y > SCREEN_Y
        || vecs[0]->y < 0 || points[i][2] > 10000) return (1);
    vecs[1]->x = points[i-y][0], vecs[1]->y = points[i-y][1];
    if (vecs[1]->x > SCREEN_X || vecs[1]->x < 0 || vecs[1]->y > SCREEN_Y
        || vecs[1]->y < 0) return (1);
    vecs[2]->x = points[i-y-1][0], vecs[2]->y = points[i-y-1][1];
    if (vecs[2]->x > SCREEN_X || vecs[2]->x < 0 || vecs[2]->y > SCREEN_Y
        || vecs[2]->y < 0) return (1);
    vecs[3]->x = points[i-1][0], vecs[3]->y = points[i-1][1];
    if (vecs[3]->x > SCREEN_X || vecs[3]->x < 0 || vecs[3]->y > SCREEN_Y
        || vecs[3]->y < 0) return (1);
    return (0);
}

//void draw_shape(sfConvexShape *shape, Vector2f_t **vecs, Color_t color,
void draw_shape(Vector2f_t **vecs, Color_t color, world_t *world)
{
    //if (!world->mv){
    //    sfConvexShape_setPoint(shape, 0, *vecs[0]);
    //    sfConvexShape_setPoint(shape, 1, *vecs[1]);
    //    sfConvexShape_setPoint(shape, 2, *vecs[2]);
    //    sfConvexShape_setPoint(shape, 3, *vecs[3]);
    //    sfConvexShape_setFillColor(shape, color);
    //    sfRenderWindow_drawConvexShape(world->cam, shape, 0);
    //} else {
        my_draw_line(world->cam_buf, *vecs[0], *vecs[1], color);
        my_draw_line(world->cam_buf, *vecs[0], *vecs[3], color);
    //}
}

void aply_flags(char flag, float **points, int *i_v, int xy)
{
    int tmp;
    flag & 0x1 ? tmp = i_v[3], i_v[3] = i_v[1], i_v[1] = tmp, i_v[5] = -1 : 0;
    flag & 0x2 ? tmp = i_v[2], i_v[2] = i_v[0], i_v[0] = tmp, i_v[4] = -1 : 0;
    for (int i = -2; i < xy; i++){
        points[i][2] = -points[i][2];
        points[i][0] = points[i][0] * 2 / points[i][2] * 32 * 8 + SCREEN_X/2;
        points[i][1] = points[i][1] * 2 / points[i][2] * 32 * 8 + SCREEN_Y/2;
    }
}

void draw_mesh(world_t *world, float **points)
{
    char **sun_grid = calc_sun_grid(world->mesh, world->x, world->y, world);
    Color_t color;
    color.r = 0, color.g = 0, color.b = 0, color.a = 0;
    my_clear_buffer(world->cam_buf, color);
    
    //static sfConvexShape *shape = 0; !shape ? shape = sfConvexShape_create(),
    //sfConvexShape_setPointCount(shape, 4) : 0;
    int i_v[] = {1, 1, world->x-1, world->y-1, 1, 1};
    char flag = calc_flag(points);
    aply_flags(flag, points, i_v, world->x*world->y);
    Vector2f_t **vecs = malloc(sizeof(Vector2f_t *) * 4);
    for (int i = 0; i < 4; i++) 
        vecs[i] = malloc(sizeof(Vector2f_t));
    for (int i_x = i_v[0]; i_x != i_v[2]; i_x += i_v[4]){
        for (int i_y = i_v[1]; i_y != i_v[3]; i_y += i_v[5]){
            int i = (flag & 0x4) ? i_y+i_x*world->x : i_y*world->y+i_x;
            if (did_we_draw(vecs, points, i, world->x)) continue;
            Color_t color = calc_color(world->mesh, i, world->x, sun_grid);
            //draw_shape(shape, vecs, color, world);
            draw_shape(vecs, color, world);
        }
    }
    free(vecs[0]), free(vecs[1]), free(vecs[2]), free(vecs[3]), free(vecs);
}

float **mesh_to_points(float **mesh, int x, int y)
{
    float **points = malloc(sizeof(float *) * (x*y+1+2));
    points[x*y+2] = 0;
    for (int i = 0; i < x*y+2; i++)
        points[i] = malloc(sizeof(float) * 4);
    points[0][0] = 1, points[0][1] = 0, points[0][2] = 0, points[0][3] = 0;
    points[1][0] = 0, points[1][1] = 1, points[1][2] = 0, points[1][3] = 0;
    points += 2;
    for (int i = 0; i < x; i++){
        for (int ii = 0; ii < y; ii++){
            points[i+ii*x][0] = -i;
            points[i+ii*x][1] = -ii;
            points[i+ii*x][2] = -mesh[i][ii];
            points[i+ii*x][3] = 1;
        }
    }
    points -= 2;
    return (points);
}
