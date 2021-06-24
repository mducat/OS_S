#ifndef __OMAC__
#define __OMAC__

typedef struct cursor {
    int line;
    int column;
} cursor_t;

typedef struct copypaste {
    cursor_t from;
    cursor_t to;
    char *copydata;
} cpdata_t;

typedef struct _omac_data {
    cursor_t cursor;
    cpdata_t cpdata;
    // to free
    lld_t *text;
    // Given in argument
    char *filename;
    // Just returned do not free !
    file_t *file;
    bool existed;
    // Supplement not malloc !
    char *currentLineString;
    int currentLineStringLength;
} odata_t;

void clear();
void draw_title();
void display(odata_t *data);
int write_nbr(int nb);
void handle_touch(odata_t *data, int touch);

#endif