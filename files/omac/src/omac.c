
#include <oss.h>
#include <lld.h>
#include <my.h>

#define printf(str, ...) {char buf[] = str; printf(buf __VA_OPT__(,) __VA_ARGS__);}

int write_nbr(int nb);
void display(lld_t *screen, int *cline, int *ccolumn, int touch);
void finish(int hasCreated, lld_t *display);

int main(int ac, char **av)
{
    char USAGE[] = "USAGE: omac <filename>\n";
    char FILE[] = "FILE IS ";
    char FILEOPEN[] = "FILE IS OPEN\n";
    char NEWLINE[] = "\n";
    char FILEDONTEXIST[] = "File doesnt exist !\n";
    char FILECREATED[] = "File has been created !\n";
    write(USAGE);
    write_nbr(ac);
    write(NEWLINE);

    if (ac != 2) {
        write(USAGE);
        return 0;
    }
    char *filename = av[1];
    write(FILE);
    write(filename);
    write(NEWLINE);
    int hasCreated = 0;
    file_t *src = open(filename);
    write(FILEOPEN);
    if (src == 0) {
        write(FILEDONTEXIST);
        write_file(filename, "\0", 1);
        src = open(filename);
        hasCreated = 1;
        write(FILECREATED);
        write_nbr(src->size);
    }
    char **file_split = strToWords(src->content, '\n');
    lld_t *screen = lld_init();
    for (int i = 0; file_split[i]; i++) {
        lld_insert(screen, lld_len(screen), file_split[i]);
    }
    lld_pop(screen, lld_len(screen) - 1);
    int line = 0;
    int column = 0;
//    display(screen, &line, &column);
    char *currentStr = lld_read(screen, line);
    int len = strlen(currentStr);
    int rs = 0;
    while (1) {
        display(screen, &line, &column, rs);
        printf("[[%s]]", currentStr);
        printf("[[%d|%d|%d]]", line, column, len);
        refresh();
        rs = read();
        handle_touch(screen, &line, &column, rs);
        char c = GET_CHR(rs);
        if (c == 'x' && IS_LEFT_CTRL(rs)) return 0;
        currentStr = lld_read(screen, line);
        len = strlen(currentStr);
        if (column < 0) column = 0;
        if (line < 0) line = 0;
        if (column > len - 1) column = len - 1;
        if (line > lld_len(screen) - 1) line = lld_len(screen) - 1;
    }
    return 0;
}

void clear()
{
    int x = get_screenX();
    int y = get_screenY();
    char RESET_SCREEN[] = {3};
    rect_t t = (rect_t) {0, 0, x, y};

    draw_rect(&t, RGBA_TO_COLOR(0, 0, 0, 255));
    write_raw(RESET_SCREEN, 1);
}

void draw_title()
{
    int x = get_screenX();
    x -= 10;
    x -= (9 * 4);
    int characterCount = (x / 9) / 2;
    printf("\n" BACK_BR_RED);
    for (int i = 0; i < characterCount; i++)
        printf(" ");
    printf("OMAC");
    for (int i = 0; i < characterCount; i++)
        printf(" ");
    printf(RESET_BACK_CLR "\n");
}

void handle_touch(lld_t *screen, int *cline, int *ccolumn, int touch)
{     
    if ((IS_LEFT_CTRL(touch) || IS_LEFT_ALT(touch)
    ||   IS_LEFT_SHIFT(touch) || IS_ESCAPE(touch))
    &&  !IS_CURSOR(touch)) {

    } else if (IS_CURSOR(touch) && IS_LEFT_CTRL(touch)) {
        // TODO text selection
    } else if (IS_CURSOR(touch)) {
        if (IS_CURSOR_DOWN(touch))  *cline += 1;
        if (IS_CURSOR_UP(touch))    *cline -= 1;
        if (IS_CURSOR_LEFT(touch))  *ccolumn -= 1;
        if (IS_CURSOR_RIGHT(touch)) *ccolumn += 1;
    } else if (IS_BACKSPACE(touch)) {
        char *str = (char *) lld_read(screen, *cline);
        char *newstr = malloc(strlen(str));
        int line = 0;
        memcpy(newstr, str, *ccolumn);
        memcpy(newstr + *ccolumn, str + *ccolumn + 1, strlen(str) - *ccolumn - 1);
        for (lld_t *mv = screen->next; mv; mv = mv->next, line++) {
            if (line == *cline) {
                mv->data = newstr;
                free(str);
                *ccolumn += 1;
                break;
            }
        }
    } else {
        char *str = (char *) lld_read(screen, *cline);
        char *endstr = str + *ccolumn + 1;
        char *newstr = malloc(strlen(str) + 2);
        memcpy(newstr, str, *ccolumn + 1);
        newstr[*ccolumn + 1] = GET_CHR(touch);
        memcpy(newstr + *ccolumn + 2, endstr, strlen(str) - *ccolumn);
        int line = 0;
        for (lld_t *mv = screen->next; mv; mv = mv->next, line++) {
            if (line == *cline) {
                mv->data = newstr;
                free(str);
                *ccolumn += 1;
                break;
            }
        }
    }
}

void display(lld_t *screen, int *cline, int *ccolumn, int touch)
{
    clear();
    draw_title();
    int line = 0;
    for (lld_t *mv = screen->next; mv; mv = mv->next, line++) {
        printf("\e128,128,128,0;%4d|" RESET_BACK_CLR, line);
        char *str = (char *) mv->data;
        if (line == *cline) {
            int y = 0;
            for (y = 0; y < *ccolumn; y++) {
                char t = str[y];
                write_raw(&t, 1);
            }
            char c = str[y];
            printf(BACK_BR_GREEN);
            write_raw(&c, 1);
            printf(RESET_BACK_CLR);
            printf("%s\n", str + y + 1);
        } else {
            printf("%s\n", str);
        }
    }
}

void finish(int hasCreated, lld_t *screen)
{
    if (hasCreated) {
        // Maybe delete file if empty
    }
    lld_free(screen);
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
