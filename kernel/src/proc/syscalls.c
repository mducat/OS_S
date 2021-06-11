
#include <dev/kbd.h>

#include <screen.h>
#include <malloc.h>
#include <fs.h>

void syscall_handler(uint64_t num, uint64_t par1, uint64_t par2, uint64_t par3)
{
    size_t n;
    int res;
    void *ptr;

    switch (num) {
    case 0:
        n = write_screen((char *) par1, (size_t) par2);
        //asm volatile("" : : "a" (n));

        break;
    case 1:
        refresh();
        break;
    case 2:
        ptr = malloc((size_t) par1);
        asm volatile("" : : "a" (ptr));

        break;
    case 3:
        free((void *) par1);
        break;
    case 4:
        ptr = (void *) opendir((char *) par1);
        asm volatile("" : : "a" (ptr));

        break;
    case 5:
        ptr = (void *) readdir((dir_t *) par1);
        asm volatile("" : : "a" (ptr));

        break;
    case 6:
        ptr = (void *) open((char *) par1);
        asm volatile("" : : "a" (ptr));

        break;
    case 7:
        remove_file((char *) par1);

        break;
    case 8:
        write_file((char *) par1, (char *) par2, (size_t) par3);

        break;
    case 9:
        res = read();
        asm volatile("" : : "a" (res));

        break;
    case 10:
        draw_rect((rect_t *) par1, (uint32_t) par2);

        break;
    case 11:
        draw_circle((circle_t *) par1, (uint32_t) par2);

        break;
    }
}
