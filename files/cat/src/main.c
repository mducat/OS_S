
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
            printf("\e255,0,0,2;\e255,0,0,1;%s: \e255,255,0,1;%s:"
                   " \e255,255,255,0;\e100,0,0,1;File not found.\n" RESET_COLOR,
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
