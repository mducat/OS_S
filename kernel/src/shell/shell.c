
#include <dev/kbd.h>
#include <screen.h>

void send_tty(char c)
{
    if (c > 0x66)
        return;

    char print = scan_code_set_2[(uint8_t) c];

    int r = write_screen(&print, 1);

    if (r > 0)
        refresh();
}
