/*
** EPITECH PROJECT, 2019
** my_str_isprintable
** File description:
** hello
*/

#include "my.h"
#include "../world.h"

float ***grad = 0;

float ***grid_grad(int x, int y)
{
    float ***grad = malloc(sizeof(float *) * (x+1));
    for (int i = 0; i < x; i++){
        grad[i] = malloc(sizeof(float *) * (y+1));
        int ii = 0;
        for (; ii < y; ii++){
            grad[i][ii] = malloc(sizeof(float) * 2);
            grad[i][ii][0] = ((rand()%5000)/2500.0-1);
            grad[i][ii][1] = ((rand()%5000)/2500.0-1);
        }
        grad[i][ii] = 0;
    }
    grad[y] = 0;
    return (grad);
}

float lerp(float a, float b, float w) {
    return (1.0 - w)*a + w*b;
}

float dotGridGradient(int ix, int iy, float x, float y)
{
    if (!grad)
        grad = grid_grad(128, 128);
    float dx = x - (float)ix;
    float dy = y - (float)iy;
    return (dx*grad[iy%128][ix%128][0] + dy*grad[iy%128][ix%128][1]);
}

float perlin(float x, float y)
{
    //printf("ici %li %i %i\n", grad, (int)x, (int)y);
    int x0 = (int)x;
    int x1 = x0 + 1;
    int y0 = (int)y;
    int y1 = y0 + 1;
    float sx = x - (float)x0;
    float sy = y - (float)y0;
    float n0, n1, ix0, ix1, value;


    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = lerp(n0, n1, sx);
    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = lerp(n0, n1, sx);
    value = lerp(ix0, ix1, sy);
    return value;
}

void free_points(float **points)
{
    for (int i = 0; points[i]; i++)
        free(points[i]);
    free(points);
}
