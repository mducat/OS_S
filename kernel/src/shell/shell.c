
#include <dev/kbd.h>
#include <string.h>
#include <malloc.h>
#include <kernel.h>
#include <files.h>

#include <screen.h>
#include <shell.h>

int prompt(void)
{
    return write_screen("[oss] $ ", 8);
}

void init_shell(void)
{
    write_screen(GOODENOUGH, strlen(GOODENOUGH));
    write_screen(shell_hdr, strlen(shell_hdr));

    prompt();

    refresh();
}

void flush_cmd(char *buf, size_t buf_len)
{
    if (!strcmp(buf, "help")) {
        write_screen("No command yet.\n", 16);
        return;
    }
    if (!strcmp(buf, "test")) {
        void (*ptr)(void) = (void(*)(void))bin0;
        ptr();
        return;
    }
    write_screen(buf, buf_len);
    write_screen("\n", 1);
}

int handle_char(char c)
{
    static char *buf = 0;
    static size_t buf_len = 0;
    static size_t cursor = 0;

    if (!buf) {
        buf = malloc(1024);
        buf_len = 1024;

        memset(buf, 0, 1024);
    }

    if (IS_PRINT(c)) {
        if (cursor >= buf_len) {
            char *n_buf = malloc(buf_len + 1024);

            memcpy(n_buf, buf, buf_len);
            memset(n_buf + buf_len, 0, 1024);
            buf_len += 1024;
        }

        buf[cursor++] = c;
        buf[cursor] = 0;
        return (0);
    }

    switch (c) {
    case '\n':
        flush_cmd(buf, cursor);
        memset(buf, 0, buf_len);

        cursor = 0;

        break;
    case '\b':
        if (!cursor)
            return 1;

        buf[--cursor] = 0;
        break;
    }
    
    return 0;
}

int send_tty(char print)
{
    int written = 0;

    if (print != '\b')
        written += write_screen(&print, 1);

    if (handle_char(print))
        return written;

    if (print == '\b')
        written += write_screen(&print, 1);

    if (print == '\n')
        written += prompt();

    if (written > 0)
        refresh();
    
    return written;
}
