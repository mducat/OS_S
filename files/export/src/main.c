
#include <oss.h>

int export(int ac, char **av)
{
    if (ac < 2) {
        printf("%s [filenames ...]\n", av[0]);
        goto out;
    }

    size_t done = 0;
    size_t size = 0;

    for (int i = 1; i < ac; i++) {
        file_t *file = open(av[i]);

        if (!file) {
            printf(LINE_BR_WHITE FRONT_RED "%s: %s: File not found.\n" RESET_COLOR,
                   av[0], av[i]);
            continue;
        }

        done++;
        size += file->size;

        export_serial(file->content, file->size);
        close(file);
    }

    if (size)
        printf(FRONT_BR_GREEN "exported %d bytes from %d files.\n" RESET_COLOR, size, done);

out:
    refresh();
    return 0;

}
