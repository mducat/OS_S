
#ifndef _OSS_H
#define _OSS_H

#include <types.h>
#include <fs.h>

/* SCREEN */

size_t write_raw(char *, size_t);
size_t write(char *);
void refresh(void);

int read(void);

int printf(const char *format, ...);

/* COLORS */

#define BACK_BLACK         "\e0,0,0,0;"
#define BACK_RED           "\e204,0,0,0;"
#define BACK_GREEN         "\e78,154,0,0;"
#define BACK_YELLOW        "\e196,160,0,0;"
#define BACK_BLUE          "\e114,159,207,0;"
#define BACK_MAGENTA       "\e117,80,207,0;"
#define BACK_CYAN          "\e6,152,154,0;"
#define BACK_WHITE         "\e211,215,207,0;"
#define BACK_BR_BLACK      "\e85,87,83,0;"
#define BACK_BR_RED        "\e239,41,41,0;"
#define BACK_BR_GREEN      "\e138,226,52,0;"
#define BACK_BR_YELLOW     "\e252,233,79,0;"
#define BACK_BR_BLUE       "\e50,175,255,0;"
#define BACK_BR_MAGENTA    "\e173,127,168,0;"
#define BACK_BR_CYAN       "\e52,226,226,0;"
#define BACK_BR_WHITE      "\e255,255,255,0;"

#define FRONT_BLACK         "\e0,0,0,1;"
#define FRONT_RED           "\e204,0,0,1;"
#define FRONT_GREEN         "\e78,154,0,1;"
#define FRONT_YELLOW        "\e196,160,0,1;"
#define FRONT_BLUE          "\e114,159,207,1;"
#define FRONT_MAGENTA       "\e117,80,207,1;"
#define FRONT_CYAN          "\e6,152,154,1;"
#define FRONT_WHITE         "\e211,215,207,1;"
#define FRONT_BR_BLACK      "\e85,87,83,1;"
#define FRONT_BR_RED        "\e239,41,41,1;"
#define FRONT_BR_GREEN      "\e138,226,52,1;"
#define FRONT_BR_YELLOW     "\e252,233,79,1;"
#define FRONT_BR_BLUE       "\e50,175,255,1;"
#define FRONT_BR_MAGENTA    "\e173,127,168,1;"
#define FRONT_BR_CYAN       "\e52,226,226,1;"
#define FRONT_BR_WHITE      "\e255,255,255,1;"

#define LINE_BLACK         "\e0,0,0,2;"
#define LINE_RED           "\e204,0,0,2;"
#define LINE_GREEN         "\e78,154,0,2;"
#define LINE_YELLOW        "\e196,160,0,2;"
#define LINE_BLUE          "\e114,159,207,2;"
#define LINE_MAGENTA       "\e117,80,207,2;"
#define LINE_CYAN          "\e6,152,154,2;"
#define LINE_WHITE         "\e211,215,207,2;"
#define LINE_BR_BLACK      "\e85,87,83,2;"
#define LINE_BR_RED        "\e239,41,41,2;"
#define LINE_BR_GREEN      "\e138,226,52,2;"
#define LINE_BR_YELLOW     "\e252,233,79,2;"
#define LINE_BR_BLUE       "\e50,175,255,2;"
#define LINE_BR_MAGENTA    "\e173,127,168,2;"
#define LINE_BR_CYAN       "\e52,226,226,2;"
#define LINE_BR_WHITE      "\e255,255,255,2;"

#define RESET_BACK_CLR  "\e0,0,0,0;"
#define RESET_FRONT_CLR "\e255,255,255,1;"
#define RESET_LINE_CLR  "\e0,0,0,2;"

#define RESET_COLOR  RESET_BACK_CLR RESET_FRONT_CLR RESET_LINE_CLR

#define _OSS_LIB

#include <screen.h>

void draw_circle(circle_t *circle, uint32_t color);
void draw_rect(rect_t *rect, uint32_t color);

/* MEMORY */

void *malloc(size_t n);
void free(void *ptr);

/* FILE SYSTEM */

#define _SYSCALL_OPEN

dir_t *opendir(char *name);
void closedir(dir_t *dir);

char *readdir(dir_t *dir);

file_t *open(char *path);
void close(file_t *file);

void remove_file(char *name);
void write_file(char *name, char *content, size_t len);

#endif
