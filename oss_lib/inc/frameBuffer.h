#pragma once

#include "my.h"
#include "lld.h"
#include "oss.h"

typedef struct framebuffer {
    union {
        struct {
            uint64_t x;
            uint64_t y;
            uint64_t width;
            uint64_t height;
        };
        rect_t rect; // 
    };
    union {
        uint8_t *pixels;
        uint32_t *pixels4;
    };
} framebuffer_t;

typedef struct Vector2f {
    float x;
    float y;    
} Vector2f_t;

typedef struct Vector2u {
    unsigned int x;
    unsigned int y;
} Vector2u_t;

typedef struct Color_t {
    union {
        struct {
            uint8_t b;
            uint8_t g;
            uint8_t r;
            uint8_t a;
        };
        uint32_t c;
    };
} Color_t;


void my_draw_line(framebuffer_t *buf, Vector2f_t vects0, Vector2f_t vects1, Color_t color);
framebuffer_t *framebuffer_create(unsigned int width, unsigned int height);
void framebuffer_destroy(framebuffer_t *framebuffer);
void my_put_pixel(framebuffer_t *framebuffer, unsigned int x, unsigned int y, Color_t color);
void my_put_pixel_fast(framebuffer_t *framebuffer, unsigned int x, unsigned int y, Color_t color);
void my_fill_buffer(framebuffer_t *buf, Color_t color);
void my_blur_buffer(framebuffer_t *buf, float ratio);
void my_draw_circle(framebuffer_t *buf, Vector2u_t position, unsigned int size, Color_t color);
void my_draw_lines(framebuffer_t *buf, Vector2f_t *vects, int n, Color_t colo);
void my_draw_square(framebuffer_t *buf, Vector2u_t position, unsigned int size, Color_t color);
void my_draw_rect(framebuffer_t *buf, int *pos_size, Color_t color);
framebuffer_t *draw(void);
void my_clear_buffer(framebuffer_t *buf, Color_t color);


//matrice
float *mat3_init(void);
float *mat3_multiply(float *mat1, float *mat2);
float *mat3_init_x(float a);
float *mat3_init_y(float a);
float *mat3_init_z(float a);
float *mat3_init_scale(float s);
float *init_p(void);
float *mat3_point(float *mat, float *p);
float *mat3_copy(float *mat);

void mat3_rx(float *mat_o, float a);
void mat3_ry(float *mat_o, float a);
void mat3_rz(float *mat_o, float a);
void mat3_tx(float *mat_o, float t);
void mat3_ty(float *mat_o, float t);
void mat3_tz(float *mat_o, float t);

void mat3_ttx(float *mat_o, float t);
void mat3_tty(float *mat_o, float t);
void mat3_ttz(float *mat_o, float t);
void mat3_rrx(float *mat_o, float a);
void mat3_rry(float *mat_o, float a);
void mat3_rrz(float *mat_o, float a);

float *mat3_inv(float *mat);