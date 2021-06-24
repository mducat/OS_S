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

void add_newline(odata_t *data)
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

void remove_character(odata_t *data)
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

void save(odata_t *data)
{
    int len = 1;
    for (lld_t *mv = data->text->next; mv; mv = mv->next)
        len += strlen(mv->data) + 1;
    char *newstr = malloc(len);
    int clen = 0;
    for (lld_t *mv = data->text->next; mv; mv = mv->next) {
        memcpy(newstr + clen, mv->data, strlen(mv->data));
        clen += strlen(mv->data);
        newstr[clen++] = '\n';
    }
    write_file(data->filename, newstr, len);
}

void handle_touch(odata_t *data, int touch)
{     
    if ((IS_LEFT_CTRL(touch) || IS_LEFT_ALT(touch)
    ||   IS_LEFT_SHIFT(touch) || IS_ESCAPE(touch))
    &&  !IS_CURSOR(touch)) {
        // TODO
        // CTRL-C / CTRL-V
        // CTRL-S
        char c = GET_CHR(touch);
        if (IS_LEFT_CTRL(touch) && c == 's') {
            save(data);
        }
        if (IS_LEFT_CTRL(touch) && c == 'c') {
            char *copystrings = 0;
            if (data->cpdata.from.line == -1)
                copystrings = strdup(data->currentLineString);
            else {
                cursor_t cpfrom = data->cpdata.from;
                cursor_t cpto   = data->cpdata.to;

                cursor_t cpfromtrad;
                cursor_t cptotrad;

                if (cpfrom.line == cpto.line) {
                    cpfromtrad  = cpfrom.column < cpto.column ? cpfrom : cpto;
                    cptotrad    = cpto.column > cpfrom.column ? cpto : cpfrom;
                } else {
                    cpfromtrad = cpfrom.line < cpto.line ? cpfrom : cpto;
                    cptotrad   = cpfrom.line > cpto.line ? cpfrom : cpto;
                }
                int len = 0;
                for (int i = cpfromtrad.line; i <= cptotrad.line; i++) {
                    char *str = (char *) lld_read(data->text, i);
                    if (i == cpfromtrad.line) {
                        if (cpfromtrad.line == cptotrad.line) {
                            len += cptotrad.column - cpfromtrad.column + 1;
                        } else {
                            len += strlen(str) - cpfromtrad.column + 1;
                        }
                    } else if (i == cptotrad.line) {
                        len += cptotrad.column + 2;
                    } else {
                        len += strlen(str) + 1;
                    }
                }
                copystrings = malloc(len);
                int dist = 0;
                for (int i = cpfromtrad.line; i <= cptotrad.line; i++) {
                    char *str = (char *) lld_read(data->text, i);
                    if (i == cpfromtrad.line) {
                        if (cpfromtrad.line == cptotrad.line)
                            memcpy(copystrings, str + cpfromtrad.column, cptotrad.column - cpfromtrad.column);
                        else {
                            memcpy(copystrings, str + cpfromtrad.column, strlen(str) - cpfromtrad.column + 1);
                            dist += strlen(str) - cpfromtrad.column;
                            copystrings[dist++] = '\n';
                        }
                    } else if (i == cptotrad.line) {
                        memcpy(copystrings + dist, str, cptotrad.column);
                        dist += cptotrad.column;
                    } else {
                        memcpy(copystrings + dist, str, strlen(str));
                        dist += strlen(str);
                        copystrings[dist++] = '\n';
                    }
                }
                data->cpdata.copydata = copystrings;
            }
        }
        if (IS_LEFT_CTRL(touch) && c == 'v') {
            if (data->cpdata.copydata == 0)
                return;
            char **newstr = strToWords(data->cpdata.copydata, '\n');
            char *currentfirststr = data->currentLineString;
            int firstlinelength = data->cursor.column + strlen(newstr[0] + 1);
            char *newfirststr = malloc(firstlinelength);
            memcpy(newfirststr, currentfirststr, data->cursor.column);
            memcpy(newfirststr + data->cursor.column, newstr[0], strlen(newstr[0]));
            newfirststr[data->cursor.column + strlen(newstr[0])] = '\0';
            lld_pop(data->text, data->cursor.line);
            lld_insert(data->text, data->cursor.line, newfirststr);
            int i = 1;
            if (newstr[i]) {
                for (; newstr[i + 1]; i++) {
                    lld_insert(data->text, data->cursor.line + i, newstr[i]);
                }
            }
            char *endstr = newstr[i];
            int endlinelength = strlen(endstr) + 1 + strlen(currentfirststr) - data->cursor.column;
            char *newendstr = malloc(endlinelength);
            memcpy(newendstr, endstr, strlen(endstr));
            memcpy(newendstr + strlen(endstr), currentfirststr + data->cursor.column, strlen(currentfirststr) - data->cursor.column);
            lld_insert(data->text, data->cursor.line + i + 1, newendstr);
        }
    } else if (IS_CURSOR(touch) && IS_LEFT_CTRL(touch)) {
        if (data->cpdata.from.line == -1) {
            data->cpdata.from.line = data->cursor.line;
            data->cpdata.from.column = data->cursor.column;
            data->cpdata.to.line = data->cursor.line;
            data->cpdata.to.column = data->cursor.column;
        } else {
            handle_cursor(data, touch);
            data->cpdata.to.line = data->cursor.line;
            data->cpdata.to.column = data->cursor.column;
        }
    } else if (IS_CURSOR(touch)) {
        data->cpdata.from   = (cursor_t) {-1, -1};
        data->cpdata.to     = (cursor_t) {-1, -1};
        return handle_cursor(data, touch);
    } else if (IS_BACKSPACE(touch)) {
        remove_character(data);
    } else if (IS_ENTER(touch)) {
        add_newline(data);
    } else {
        add_character(data, touch);
    }
}