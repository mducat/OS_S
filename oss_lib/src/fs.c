
#include <oss.h>

#include <fs.h>

dir_t *opendir(char const *name)
{
    int num = 4;
    dir_t *dir = 0;

    LOAD2(num, name)
    asm volatile("int $0x30");

    dir = (dir_t *) *((uint64_t *) 0x1234560);
    return dir;
}

char *readdir(dir_t *dir)
{
    int num = 5;
    char *name = 0;

    LOAD2(num, dir)
    asm volatile("int $0x30");

    name = (char *) *((uint64_t *) 0x1234560);
    return name;
}

file_t *open(char const *path)
{ 
    int num = 6;
    file_t *file = 0;

    LOAD2(num, path)
    asm volatile("int $0x30");

    file = (file_t *) *((uint64_t *) 0x1234560);
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

void remove_file(char const *name)
{
    int num = 7;

    LOAD2(num, name)
    asm volatile("int $0x30");
}

void write_file(char const *name, char *content, size_t len)
{
    int num = 8;

    LOAD4(num, name, content, len)
    asm volatile("int $0x30");
}
