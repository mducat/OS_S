#pragma once

#ifndef _ucp_h_
#define _ucp_h_

typedef long unsigned int u64;
typedef unsigned int u32;


#define IS_FLAG 0x8000000000000000
#define IS_SET(str) ((char *)((u64)str | (u64)0x8000000000000000))

int ucp(char *str, char *regex, ...);
char ***ucp_tab(char *str, char *regex, ...);
void ucp_free(char ***tab);
void ucpDestroy();
void ucpInit();


void *is_made1(char *str);
void *is_made2(char *str);
void *is_made3(char *str);
void *is_made4(char *str);
void *is_made5(char *str);

#endif

//linked list (lld)
typedef struct lld
{
    void *data;
    struct lld *next;
    struct lld *prev;
} lld_t;

int lld_db(lld_t *lld);
void lld_free_r(lld_t *lld);
void lld_free(lld_t *lld);
lld_t *lld_init();
void lld_insert_node(lld_t *lld, int poss, lld_t *node);
void lld_insert(lld_t *lld, int pos, void *data);
int lld_len_db(lld_t *lld);
int lld_len(lld_t *lld);
void **lld_lld_to_tab(lld_t *lld);
void *lld_pop_node(lld_t *lld, int pos);
void *lld_pop(lld_t *lld, int pos);
void lld_print_int(lld_t *lld);
void lld_print_str(lld_t *lld);
void *lld_read(lld_t *lld, int poss);
void lld_sort(lld_t *list, int (*cmp)());
void *lld_write(lld_t *lld, int poss, void *data);

char **strToWords(const char *str, char split);
char *my_putnbr_base_str(unsigned long int nbr, char *base);

char *strconcat(char const *str1, char const *str2);

#include "oss.h"

typedef struct framebuffer {
    union {
        struct {
            uint64_t x;
            uint64_t y;
            uint64_t width;
            uint64_t height;
        };
        void *rect_pos; // 
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
            uint8_t r;
            uint8_t g;
            uint8_t b;
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
void my_draw_circle(framebuffer_t *buf, Vector2u_t position, unsigned int size, Color_t *color);
void my_draw_lines(framebuffer_t *buf, Vector2f_t *vects, int n, Color_t colo);
void my_draw_square(framebuffer_t *buf, Vector2u_t position, unsigned int size, Color_t color);
void my_draw_rect(framebuffer_t *buf, int *pos_size, Color_t color);
framebuffer_t *draw(void);
void my_clear_buffer(framebuffer_t *buf, Color_t color);