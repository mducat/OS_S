
#include <oss.h>

#include <fs.h>

dir_t *opendir(char *name)
{
    int num = 4;
    dir_t *dir = 0;

    asm volatile("int $0x30" :
                 : "a" (num),
                   "b" (name));

    asm("" : "=a" (dir));
    return dir;
}

char *readdir(dir_t *dir)
{
    int num = 5;
    char *name = 0;

    asm volatile("int $0x30" :
                 : "a" (num),
                   "b" (dir));

    asm("" : "=a" (name));
    return name;
}

file_t *open(char *path)
{ 
    int num = 6;
    file_t *file = 0;

    asm volatile("int $0x30" :
                 : "a" (num),
                   "b" (path));

    asm("" : "=a" (file));
    return file;
}

void close(file_t *file)
{
    free(file->name);
    free(file->content);
    free(file);
}

void closedir(dir_t *dir)
{
    free(dir->name);
    free(dir);
}
