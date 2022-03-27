
#include <dev/kbd.h>
#include <string.h>
#include <malloc.h>
#include <kernel.h>

#include <screen.h>
#include <shell.h>
#include <fs.h>
#include <windows.h>
#include <utils.h>

int prompt(void)
{
    return write_screen("[oss] $ ", 8);
}

void do_reset(reset_t reset, reset_type_t type) {
    static reset_t s_res = 0;

    if (reset != 0)
        s_res = reset;
    else
        s_res(type, 0, 0, 0);
}

void init_shell(void)
{
    send_tty(3); // END OF TEXT

    write_screen(GOODENOUGH, strlen(GOODENOUGH));
    write_screen(SHELL_HDR, strlen(SHELL_HDR));

    prompt();

    refresh();
}

void reinit_shell(void)
{
    clear();
    send_tty(3); // END OF TEXT

    write_screen(GOODENOUGH, strlen(GOODENOUGH));
    write_screen(SHELL_HDR, strlen(SHELL_HDR));

    refresh();
}

void flush_cmd(char *buf, size_t buf_len)
{
    char **av = strToWords(buf, ' ');
    file_t *file = open(av[0]);
    int ac = 0;

    while (av[++ac]);

    if (!buf_len)
        return;

    if (!strcmp(av[0], "window")) {
        if (!g_WindowManager) {
            write_screen("WindowManager_init\n", 19);
            g_WindowManager = WindowManager_init();
            Window_t *win = WindowManager_create_Window(g_WindowManager, 200, 200);
            win->pos[0] = 200;
            win->pos[1] = 100;
            Color_t color;
            color.r = 255;
            color.g = 0;
            color.b = 0;
            color.a = 255;
            my_draw_circle(win->buffer, (Vector2u_t){50, 50}, 5, color);
        }
        WindowManager_draw_all(g_WindowManager);
        write_screen("done\n", 5);
        return;
    }

    if (!strcmp(av[0], "windowmv")) {
        if (ac != 4) {
            write_screen("windowmv id x y\n", 16);
            return;
        }
        int id = my_getnbr(av[1]);

        Window_t *win = WindowManager_get_Window(g_WindowManager, id);
        if (win) {
            win->pos[0] = my_getnbr(av[2]);
            win->pos[1] = my_getnbr(av[3]);
            WindowManager_draw_all(g_WindowManager);
            refresh();
        } else {
            write_screen("no window with this id\n", 23);
        }
        return;
    }


    if (!strcmp(av[0], "clear")) {
        reinit_shell();
        return;
    }

    if (!strcmp(av[0], "shutdown"))
        do_reset(0, shutdown_reset);

    if (!strcmp(av[0], "reboot"))
        do_reset(0, cold_reset);

    if (!file) {
        write_screen("No such file.\n", 14);
        return;
    }

    if (exec(file, ac, av))
        write_screen("This file is not executable.\n", 29);

    free(file->name);
    free(file->content);
    free(file);
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

        //[[fallthrough]];
    case 3:
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
