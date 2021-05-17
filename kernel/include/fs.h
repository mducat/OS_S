
#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include <types.h>

typedef struct file_s {
    char *name;
    char *content;
    size_t size;
} file_t;

#endif
