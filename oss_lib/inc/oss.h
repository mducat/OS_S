
#ifndef _OSS_H
#define _OSS_H

#include <types.h>
#include <fs.h>

/* SCREEN */

size_t write_raw(char *, size_t);
size_t write(char *);
void refresh(void);

int printf(const char *format, ...);

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

#endif
