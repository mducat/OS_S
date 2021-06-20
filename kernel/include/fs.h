
#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include <types.h>
#include <lld.h>

typedef struct file_s {
    char * name;
    char * content;
    size_t size;
} file_t;

typedef struct dir_s {
    char  * name;
    size_t  size;
    size_t  idx;
} dir_t;

extern lld_t *files;

void init_file_system(void);

#ifndef _SYSCALL_OPEN
#define _SYSCALL_OPEN

dir_t *opendir(char *name);
void closedir(dir_t *dir);

char *readdir(dir_t *dir);

file_t *open(char *path);
void close(file_t *file);

void write_file(char *name, char *content, size_t len);
void remove_file(char *name);

#endif

#endif
