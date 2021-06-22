#include <oss.h>
#include <lld.h>
#include <my.h>

#include "omac.h"

void handle_cursor(odata_t *data, int touch)
{
    if (IS_CURSOR_DOWN(touch))  data->cursor.line += 1;
    if (IS_CURSOR_UP(touch))    data->cursor.line -= 1;
    if (IS_CURSOR_LEFT(touch))  data->cursor.column -= 1;
    if (IS_CURSOR_RIGHT(touch)) data->cursor.column += 1;

    // Set new currentLineString and currentLineStringLength
    data->currentLineString = lld_read(data->text, data->cursor.line);
    data->currentLineStringLength = strlen(data->currentLineString);

    // Prevent going over limit
    if (data->cursor.column < 0)
        data->cursor.column = 0;
    if (data->cursor.line < 0)
        data->cursor.line = 0;
    if (data->cursor.column > data->currentLineStringLength - 1)
        data->cursor.column = data->currentLineStringLength - 1;
    if (data->cursor.line > lld_len(data->text) - 1)
        data->cursor.line = lld_len(data->text) - 1;
}

// TODO : void handle_selector();

void add_character(odata_t *data, int touch)
{
    char *str = data->currentLineString;
    char *endstr = str + data->cursor.column + 1;
    char *newstr = malloc(strlen(str) + 2);
    memcpy(newstr, str, data->cursor.column + 1);
    newstr[data->cursor.column + 1] = GET_CHR(touch);
    memcpy(newstr + data->cursor.column + 2, endstr, strlen(str) - data->cursor.column);
    int line = 0;
    for (lld_t *mv = data->text->next; mv; mv = mv->next, line++) {
        if (line == data->cursor.line) {
            mv->data = newstr;
            free(str);
            data->cursor.column += 1;
            break;
        }
    }
}

void handle_touch(odata_t *data, int touch)
{     
    if ((IS_LEFT_CTRL(touch) || IS_LEFT_ALT(touch)
    ||   IS_LEFT_SHIFT(touch) || IS_ESCAPE(touch))
    &&  !IS_CURSOR(touch)) {
        // TODO
        // CTRL-C / CTRL-V
        // CTRL-A
    } else if (IS_CURSOR(touch) && IS_LEFT_CTRL(touch)) {
        // TODO handle_selector
    } else if (IS_CURSOR(touch)) {
        return handle_cursor(data, touch);
    } else if (IS_BACKSPACE(touch)) {
        // TODO FIX BUGGY
        /* EVEN A FULL REDO !!!!!!!
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
        */
    } else {
        add_character(data, touch);
    }
}