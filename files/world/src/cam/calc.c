/*
** EPITECH PROJECT, 2019
** my_str_isprintable
** File description:
** hello
*/

#include "my.h"
#include "../world.h"

char calc_flag(float **points)
{
    float mid = cos(PI/4);
    char flag =
    (points[-1][2] > -mid && points[-1][2] < 0 && points[-2][2] < -mid) ? 0x0 :
    (points[-1][2] < mid && points[-1][2] > 0 && points[-2][2] < -mid) ? 0x1 :
    (points[-1][2] > mid && points[-2][2] > -mid && points[-2][2] < 0) ? 0x6 :
    (points[-1][2] > mid && points[-2][2] < mid && points[-2][2] > 0) ? 0x7 :
    (points[-1][2] < mid && points[-1][2] > 0 && points[-2][2] > mid) ? 0x3 :
    (points[-1][2] < 0 && points[-1][2] > -mid && points[-2][2] > mid) ? 0x2 :
    (points[-1][2] < -mid && points[-2][2] > 0 && points[-2][2] < mid) ? 0x5 :
    points[-1][2] < -mid && points[-2][2] < 0 && points[-2][2] > -mid ? 0x4 : 0;
    return (flag);
}



char **calc_sun_grid(float **mesh, int x, int y, world_t *world)
{
    //world->sun += PI/6000;
    //float sun_angle = fabsf(tan(world->sun));
    float sun_angle = 0.25;
    world->sun > PI*1.5 ? world->sun = -PI/1.5 : 0;
    sin(world->sun) < 0 ? sun_angle *= -1 : 0;
    if (sun_grid == 0){
        sun_grid = malloc(sizeof(char *) * y);
        for (int i = 0; i < y; i++) 
            sun_grid[i] = malloc(sizeof(char) * x);
        world->sun_grid = sun_grid;
    }
    for (int yy = 0; yy < y; yy++){
        float start = mesh[yy][world->sun < PI/2 ? 0 : x-1];
        for (int xx = (world->sun < PI/2.0) ? 0 : x-1; xx < x && 0 <= xx; xx += (world->sun < PI/2.0) ? 1 : -1){
            // todo repair sun (pb = next malloc will crash)
            start -= sun_angle;
            sun_grid[yy][xx] = ((mesh[yy][xx]) >= start) ? start = (mesh[yy][xx]), 1 : 0;
            //sun_grid[yy][xx] = 1;
        }
    }
    return (sun_grid);
}

float gauss(float x, float dec1, float dec2, float off)
{
    x -= off;
    return (1/(pow(E, x/dec2)+pow(E, -x/dec1)));
}

Color_t calc_color(float **mesh, int i, int x, char **sun_grid)
{
    float height = (mesh[i%x][i/x] + mesh[i%x-1][i/x-1] +
    mesh[i%x][i/x-1] + mesh[i%x-1][i/x])/4;
    int water = gauss(height, 100, 1, -20) * 500;
    int grass = gauss(height, 7, 7, -10) * 500;
    int stone = gauss(height, 1, 10, 10) * 500;
    int snow = gauss(height, 10, 100, 17) * 500;

    Color_t color;
    color.r = min_i(snow+stone, 255);
    color.g = min_i(grass+snow+stone, 255);
    color.b = min_i(water+snow+stone, 255);
    color.a = 255;
    if (sun_grid[i%x][i/x] == 0){
        float div = 0;
        div += -sun_grid[(i+1)%x][i/x]+1;
        div += -sun_grid[(i-1)%x][i/x]+1;
        div += -sun_grid[i%x][(i/x+1)%x]+1;
        div += -sun_grid[i%x][(i/x-1)%x]+1;
        div = 1.0 / (div/2+1);
        color.r *= div, color.g *= div, color.b *= div;
    }
    return (color);
}
