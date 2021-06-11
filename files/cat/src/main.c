
#include <oss.h>

int main(int ac, char **av)
{
    int data = read();

    printf("received char : '%c'\n", data);

    if (ac < 2) {
        printf("%s [filenames ...]\n", av[0]);
        goto out;
    }

    for (int i = 1; i < ac; i++) {
        file_t *file = open(av[i]);

        if (!file) {
            printf(LINE_BR_RED FRONT_BR_RED "%s: " FRONT_BR_YELLOW "%s: "
                   BACK_WHITE FRONT_RED "File not found.\n" RESET_COLOR,
                   av[0], av[i]);
            continue;
        }

        write_raw(file->content, file->size);
        close(file);
    }

out:
    refresh();
    return 0;
}
