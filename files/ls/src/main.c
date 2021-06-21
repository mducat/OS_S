
#include <oss.h>

int main(void)
{
    dir_t *dir = opendir("/");
    char *name = readdir(dir);

    printf(" files in folder '/':\n");

    while (name) {
        file_t *file = open(name);

        printf("%d - %s\n", file->size, file->name);

        close(file);

        free(name);
        name = readdir(dir);
    }
    closedir(dir);
    refresh();
    return 0;
}

