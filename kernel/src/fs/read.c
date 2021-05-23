
#include <fs.h>

#include <malloc.h>
#include <string.h>

dir_t *opendir(char *name)
{
    dir_t *dir = malloc(sizeof(dir_t));

    dir->name = strdup((char *) name); // no actual dirs
    dir->size = lld_len(files);
    dir->idx  = 0;
    return dir;
}

char *readdir(dir_t *dir)
{
    if (dir->idx + 1 > (size_t) lld_len(files))
        return 0;

    file_t *at = lld_read(files, dir->idx++);
    char *name = strdup(at->name);

    return name;
}

file_t *open(char *name)
{
    file_t *file = 0;
    file_t *disk = 0;
    lld_t *it = files;

    for (; it && strcmp(((file_t *) (it->data))->name, name);
         it = it->next);
    if (!it)
        return 0;
    disk = it->data;

    file = malloc(sizeof(file_t));
    file->size = disk->size;
    file->name = strdup(disk->name);

    file->content = malloc(disk->size);
    memcpy(file->content, disk->content, disk->size);

    return file;
}
