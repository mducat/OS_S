#include <oss.h>
#include <lld.h>
#include <my.h>

#include "omac.h"

void updateCurrentInformation(odata_t *data)
{
    data->currentLineString = lld_read(data->text, data->cursor.line);
    data->currentLineStringLength = strlen(data->currentLineString);
}

void handle_cursor(odata_t *data, int touch)
{
    if (IS_CURSOR_DOWN(touch))  data->cursor.line += 1;
    if (IS_CURSOR_UP(touch))    data->cursor.line -= 1;
    if (IS_CURSOR_LEFT(touch))  data->cursor.column -= 1;
    if (IS_CURSOR_RIGHT(touch)) data->cursor.column += 1;

    // Prevent going over limit
    if (data->cursor.line < 0)
        data->cursor.line = 0;
    if (data->cursor.line > lld_len(data->text) - 1)
        data->cursor.line = lld_len(data->text) - 1;
    if (data->cursor.column < 0)
        data->cursor.column = 0;
    updateCurrentInformation(data);
    if (data->cursor.column > data->currentLineStringLength)
        data->cursor.column = data->currentLineStringLength;
    
    // Set new currentLineString and currentLineStringLength
    updateCurrentInformation(data);
}

// TODO : void handle_selector();

void add_character(odata_t *data, int touch)
{
    char *str = data->currentLineString;
    char *endstr = str + data->cursor.column;
    char *newstr = malloc(strlen(str) + 2);
    // Create new str
    memcpy(newstr, str, data->cursor.column);
    newstr[data->cursor.column] = GET_CHR(touch);
    memcpy(newstr + data->cursor.column + 1, endstr, strlen(str) - data->cursor.column + 2);
    // Set in LLD
    lld_pop(data->text, data->cursor.line);
    lld_insert(data->text, data->cursor.line, newstr);
    // Update column
    updateCurrentInformation(data);
    data->cursor.column++;
}

void add_newline(odata_t *data, int touch)
{
    char *str = data->currentLineString;
    char *newstr = malloc(strlen(str) - data->cursor.column + 1);
    char *oldstr = malloc(data->cursor.column + 2);
    memcpy(oldstr, str, data->cursor.column);
    memcpy(newstr, str + data->cursor.column, strlen(str) - data->cursor.column);
    lld_pop(data->text, data->cursor.line);
    lld_insert(data->text, data->cursor.line, oldstr);
    lld_insert(data->text, data->cursor.line + 1, newstr);
    data->cursor.line++;
    data->cursor.column = 0;
    updateCurrentInformation(data);
}

void remove_character(odata_t *data, int touch)
{
    char *str = data->currentLineString;

    if (strlen(data->currentLineString) == 0) {
        if (lld_len(data->text) == 1) return;
        lld_pop(data->text, data->cursor.line);
        data->cursor.line--;
    } else if (data->cursor.column == 0) {
        if (data->cursor.line == 0) return;
        char *laststr = lld_read(data->text, data->cursor.line - 1);
        char *newstr = malloc(strlen(str) + strlen(laststr) + 1);
        memcpy(newstr, laststr, strlen(laststr));
        memcpy(newstr + strlen(laststr), str, strlen(str));
        lld_pop(data->text, data->cursor.line);
        lld_pop(data->text, data->cursor.line - 1);
        lld_insert(data->text, data->cursor.line - 1, newstr);
        data->cursor.line--;
        data->cursor.column = strlen(newstr);
    } else {
        char *newstr = malloc(strlen(str));
        memcpy(newstr, str, data->cursor.column - 1);
        memcpy(newstr + data->cursor.column - 1, str + data->cursor.column, strlen(str) - data->cursor.column);
        lld_pop(data->text, data->cursor.line);
        lld_insert(data->text, data->cursor.line, newstr);
        data->cursor.column--;
    }
    updateCurrentInformation(data);
}

void handle_touch(odata_t *data, int touch)
{     
    if ((IS_LEFT_CTRL(touch) || IS_LEFT_ALT(touch)
    ||   IS_LEFT_SHIFT(touch) || IS_ESCAPE(touch))
    &&  !IS_CURSOR(touch)) {
        // TODO
        // CTRL-C / CTRL-V / CTRL-X
        // CTRL-A
        // CTRL-S
        char c = GET_CHR(touch);
        if (IS_LEFT_CTRL(touch) && c == 's') {
            int len = 1;
            for (lld_t *mv = data->text->next; mv; mv = mv->next)
                len += strlen(mv->data) + 1;
            char *newstr = malloc(len);
            int clen = 0;
            for (lld_t *mv = data->text->next; mv; mv = mv->next) {
                memcpy(newstr + clen, mv->data, strlen(mv->data));
                clen += strlen(mv->data);
                newstr[clen++] = "\n";
            }
            write_file(data->filename, newstr, len);
        }
    } else if (IS_CURSOR(touch) && IS_LEFT_CTRL(touch)) {
        // TODO handle_selector
    } else if (IS_CURSOR(touch)) {
        return handle_cursor(data, touch);
    } else if (IS_BACKSPACE(touch)) {
        remove_character(data, touch);
    } else if (IS_ENTER(touch)) {
        add_newline(data, touch);
    } else {
        add_character(data, touch);
    }
}