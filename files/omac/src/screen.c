#include <oss.h>
#include <my.h>

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

void print_text_no_cp(odata_t *data)
{
    int line = 0;
    for (lld_t *mv = data->text->next; mv; mv = mv->next, line++) {
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

int is_incp(odata_t *data, cursor_t pos)
{
    cursor_t cpfrom = data->cpdata.from;
    cursor_t cpto = data->cpdata.to;

    /**
     * Represent a copy of multiple lines
     * 
    **/
    if (cpfrom.line != cpto.line) {
        cursor_t cpfromtrad;
        cpfromtrad.line = cpfrom.line < cpto.line ? cpfrom.line : cpto.line;
        cpfromtrad.column = cpfrom.line < cpto.line ? cpfrom.column : cpto.column;
        cursor_t cptotrad;
        cptotrad.line = cpfrom.line > cpto.line ? cpfrom.line : cpto.line;
        cptotrad.column = cpfrom.line > cpto.line ? cpfrom.column : cpto.column;
        /**
         * Represent when the pos is maybe between the two lines but maybe not the column
         * 
        **/
        if (pos.line >= cpfromtrad.line && pos.line <= cptotrad.line) {
            /**
             * When pos is between the start line and end line so in the selector
             * 
            **/ 
            if (pos.line != cpfromtrad.line && pos.line != cptotrad.line) return 1;
            /**
             * When the pos is on the start line and after or on the start column
             * 
            **/ 
            if (pos.line == cpfromtrad.line && pos.column >= cpfromtrad.column) return 1;
            /**
             * When the pos is on the end line and before or on the start column
             * 
            **/ 
            if (pos.line == cptotrad.line && pos.column <= cptotrad.column) return 1;
        }
        /**
         * None of the previous conditions were ok so then pos is not in the selector
         * 
        **/
        return 0;
    /**
     * Represent selector on a signle line
     * 
    **/
    } else {
        /**
         * To be sure form is before to
         * 
        **/ 
        if (pos.line != cpfrom.line) return 0;
        cursor_t cpfromtrad = cpfrom.column < cpto.column ? cpfrom : cpto;
        cursor_t cptotrad   = cpfrom.column < cpto.column ? cpto : cpfrom;
        if (pos.column >= cpfromtrad.column && pos.column <= cptotrad.column) return 1;
        return 0;
    }
}

void display(odata_t *data)
{
    clear();
    draw_title();
    if (data->cpdata.from.line == -1) {
        print_text_no_cp(data);
    } else {
        int line = 0;
        for (lld_t *mv = data->text->next; mv; mv = mv->next, line++) {
            printf("\e128,128,128,0;%4d|" RESET_BACK_CLR, line);
            char *str = (char *) mv->data;
            for (int i = 0; str[i]; i++) {
                char c = str[i];
                if (line == data->cursor.line && i == data->cursor.column) {
                    printf(BACK_BR_GREEN);
                    write_raw(&c, 1);
                    printf(RESET_BACK_CLR);
                } else if (is_incp(data, (cursor_t) {line, i})) {
                    printf(BACK_BR_RED);
                    write_raw(&c, 1);
                    printf(RESET_BACK_CLR);
                } else {
                    write_raw(&c, 1);
                }
            }
            printf("\n");
        }
    }
    printf("COPY DATA :\n%s\n", data->cpdata.copydata);
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