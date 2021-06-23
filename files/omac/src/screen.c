#include <oss.h>

#include "omac.h"

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

void display(odata_t *data)
{
    clear();
    draw_title();
    int line = 0;
    for (lld_t *mv = data->text->next; mv; mv = mv->next, line++) {
        // Line prefix
        printf("\e128,128,128,0;%4d|" RESET_BACK_CLR, line);
        char *str = (char *) mv->data;
        if (line == data->cursor.line) {
            if (data->cursor.column == strlen(str)) {
                printf("%s" BACK_BR_GREEN " \n" RESET_BACK_CLR, str);
            } else {
                int y = 0;
                for (y = 0; y < data->cursor.column; y++) {
                    char t = str[y];
                    write_raw(&t, 1);
                }
                char c = str[y];
                printf(BACK_BR_GREEN);
                write_raw(&c, 1);
                printf(RESET_BACK_CLR);
                printf("%s\n", str + y + 1);
            }
        } else {
            printf("%s\n", str);
        }
    }
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