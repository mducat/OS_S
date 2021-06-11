
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

/* COLOR */

#define RESET_BACK_CLR  "\e0,0,0,0;"
#define RESET_FRONT_CLR "\e0,255,255,1;"
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
