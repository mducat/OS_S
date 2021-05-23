
#include <oss.h>
#include <lld.h>

int main(void)
{
    dir_t *dir = opendir("/");
    char *name = readdir(dir);

    write("folder '/':\n");

    while (name) {
        file_t *file = open(name);

        write(file->name);
        write(file->content);
        write("\n");

        close(file);

        free(name);
        name = readdir(dir);
    }
    closedir(dir);
    refresh();
    return 0;
}
