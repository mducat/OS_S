
#include <oss.h>
#include <lld.h>

int write_nbr(int nb);

int main(void)
{
    dir_t *dir = opendir("/");
    char *name = readdir(dir);

    char f_msg[] = " files in folder '/':\n"; 
    char f_sep[] = " - "; 
    char f_end[] = "\n"; 

    write_nbr(dir->size);
    printf(f_msg);

    while (name) {
        file_t *file = open(name);

        write_nbr(file->size);
        write(f_sep);
        write(file->name);
        write(f_end);

        close(file);

        free(name);
        name = readdir(dir);
    }
    closedir(dir);
    refresh();
    return 0;
}

int write_nbr(int nb)
{
    int count = 100000000;
    int len = 0;
    char print[] = {0, 0};
    nb < 0 ? nb *= -1, write("-"), len++, 1 : 0;

    while (nb / count == 0 && count > 1)
        count = count / 10;

    while (count != 0){
        print[0] = '0' + nb / count;
        write(print);
        len++;
        nb -= (nb/count) * count;
        count = count / 10;
    }
    return (len);
}
