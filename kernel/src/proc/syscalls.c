
#include <dev/kbd.h>
#include <dev/serial.h>

#include <screen.h>
#include <malloc.h>
#include <fs.h>

void syscall_handler(uint64_t num, uint64_t par1, uint64_t par2, uint64_t par3)
{
    size_t n;
    void *ptr;
    int res;
    uint32_t n32;

    switch (num) {
    case 0:
        n = write_screen((char *) par1, (size_t) par2);
        *((uint64_t *) 0x1234560) = (uint64_t) n;

        break;
    case 1:
        refresh();
        break;
    case 2:
        ptr = malloc((size_t) par1);
        *((uint64_t *) 0x1234560) = (uint64_t) ptr;

        break;
    case 3:
        free((void *) par1);
        break;
    case 4:
        ptr = (void *) opendir((char *) par1);
        *((uint64_t *) 0x1234560) = (uint64_t) ptr;

        break;
    case 5:
        ptr = (void *) readdir((dir_t *) par1);
        *((uint64_t *) 0x1234560) = (uint64_t) ptr;

        break;
    case 6:
        ptr = (void *) open((char *) par1);
        *((uint64_t *) 0x1234560) = (uint64_t) ptr;

        break;
    case 7:
        remove_file((char *) par1);

        break;
    case 8:
        write_file((char *) par1, (char *) par2, (size_t) par3);

        break;
    case 9:
        res = read();
        *((uint64_t *) 0x1234560) = (uint64_t) res;

        break;
    case 10:
        draw_rect((rect_t *) par1, (uint32_t) par2);

        break;
    case 11:
        draw_circle((circle_t *) par1, (uint32_t) par2);

        break;
    case 12:
        n32 = get_screenX();
        *((uint64_t *) 0x1234560) = (uint64_t) n32;

        break;
    case 13:
        n32 = get_screenY();
        *((uint64_t *) 0x1234560) = (uint64_t) n32;

        break;
    case 14:
        serial_out_raw((char *) par1, (size_t) par2);
        break;
    }
}
