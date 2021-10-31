/*
** EPITECH PROJECT, 2019
** my_str_isprintable
** File description:
** hello
*/

#include "my.h"
#include "../world.h"

float **copy_points(float **points)
{
    int nb_p = 0;
    for (; points[nb_p]; nb_p++);
    float **new_points = malloc(sizeof(float *) * (nb_p+1));
    new_points[nb_p] = 0;
    for (int i = 0; i < nb_p; i++){
        new_points[i] = malloc(sizeof(float) * 4);
        new_points[i][0] = points[i][0];
        new_points[i][1] = points[i][1];
        new_points[i][2] = points[i][2];
        new_points[i][3] = points[i][3];
    }
    return (new_points);
}

float **rotate_points(float **points, float *mat)
{
    int i = 0;
    for (i = 0; points[i]; i++);
    float **new_points = malloc(sizeof(float *) * (i+1));
    new_points[i] = 0;
    for (i = 0; points[i]; i++) {
        new_points[i] = mat3_point(mat, points[i]);
    }
    return (new_points);
}

float **create_mesh(int x, int y, int d, int xof, int yof)
{
    float **mesh = malloc(sizeof(float *) * (x+1));
    int i = 0;
    for (; i < x; i++){
        mesh[i] = malloc(sizeof(float) * (y+1));
        int ii = 0;
        for (; ii < y; ii++){
            mesh[i][ii] = 0;
            for (int d_nb = 0; d_nb < d; d_nb++) {
                mesh[i][ii] += perlin((float)(i+xof)/(1.1*pow(2, d_nb)), (float)(ii+yof)/(1.1*pow(2, d_nb)))*pow(2, d_nb);
            }
            mesh[i][ii] -= 5;
        }
        mesh[i][ii] = 0;
    }
    mesh[i] = 0;
    return (mesh);
}

void update_mesh(float **mesh, int x, int y, int d, int xof, int yof)
{
    
    for (int i = 0; i < x; i++){
        for (int ii = 0; ii < y; ii++){
            mesh[i][ii] = 0;
            for (int d_nb = 0; d_nb < d; d_nb++) {
                mesh[i][ii] += perlin((float)(i+xof)/(1.1*pow(2, d_nb)), (float)(ii+yof)/(1.1*pow(2, d_nb)))*pow(2, d_nb);
            }
            mesh[i][ii] -= 5;
        }
    }
}


void drop_water(float **mesh, int *xy, float f, float n)
{
    char ok = 0;
    int x = xy[0];
    int y = xy[1];
    if (!(x <= 1 || x >= xy[2] - 2 || y <= 1 || y >= xy[3] - 2)){
        float here = mesh[x][y];
        for (int i = 0; i < 16; i++){
            int xx = rand() % 3 - 1 + x;
            int yy = rand() % 3 - 1 + y;
            if (here > mesh[xx][yy]){
                float d = pow(here - mesh[xx][yy], 0.5) * f - n;
                int xxyy[] = {xx, yy, xy[2], xy[3]};
                drop_water(mesh, xxyy, f, n+d);
                mesh[x][y] -= d/2, mesh[x-1][y] -= d/8, mesh[x+1][y] -= d/8;
                mesh[x][y-1] -= d/8, mesh[x][y+1] -= d/8, ok = 1;
                break;
            }
        }
    }
    !ok ? mesh[x+1][y] += n/8, mesh[x-1][y] += n/8,
    mesh[x][y+1] += n/8, mesh[x][y-1] += n/8, mesh[x][y] += n/2 : 0;
}

void free_mesh(float **mesh, int x, int y)
{
    (void)y;
    int i = 0;
    for (; i < x; i++)
        free(mesh[i]);
    free(mesh);
}
