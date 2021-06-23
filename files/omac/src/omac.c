
#include <oss.h>
#include <lld.h>
#include <my.h>

#include "omac.h"

int write_nbr(int nb);
void finish(int hasCreated, lld_t *display);

void get_file(const char *filename, odata_t *data)
{
    data->existed = 1;
    data->filename = filename;
    data->file = open(filename);
    if (data->file == 0) {
        write_file(filename, "\0", 1);
        data->file = open(filename);
        data->existed = 0;
    }
    char **tmp = strToWords(data->file->content, '\n');
    data->text = lld_init();
    for (int i = 0; tmp[i]; i++) {
        lld_insert(data->text, lld_len(data->text), tmp[i]);
    }
    lld_pop(data->text, lld_len(data->text) - 1);
}

void init_data(odata_t *data, char **av)
{
    data->cursor = (cursor_t) {0, 0};
    data->cpdata = (cpdata_t) {(cursor_t) {0, 0}, (cursor_t) {0, 0}};
    get_file(av[1], data);
    
    data->currentLineString = lld_read(data->text, 0);
    data->currentLineStringLength = strlen(data->currentLineString);
}

int main(int ac, char **av)
{
    odata_t data;
    if (ac != 2) {
        write("USAGE: omac <filename>");
        return 0;
    }
    init_data(&data, av);
    while (1) {
        display(&data);
        refresh();
        int rs = read();
        char c = GET_CHR(rs);
        if (c == 'x' && IS_LEFT_CTRL(rs)) return 0;
        else handle_touch(&data, rs);
    }
    return 0;
}

void finish(int hasCreated, lld_t *screen)
{
    if (hasCreated) {
        // Maybe delete file if empty
    }
    lld_free(screen);
}
