
#include <fs.h>

#include <malloc.h>
#include <string.h>
#include <screen.h>

dir_t *opendir(char const *name)
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

file_t *open(char const *name)
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

void remove_file(char const *name)
{
    lld_t *it = files;
    int pos = 0;

    for (; it && strcmp(((file_t *) (it->data))->name, name);
         it = it->next, pos++);
    if (!it)
        return;

    lld_pop(files, pos - 1);
}

void write_file(char const *name, char *content, size_t len)
{
    file_t *file = malloc(sizeof(file_t));

    file->size = len;
    file->name = strdup(name);

    file->content = malloc(len);
    memcpy(file->content, content, len);

    remove_file(name);
    lld_insert(files, lld_len(files), file);
}
