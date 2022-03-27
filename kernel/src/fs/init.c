
#include <fs.h>
//#include <files_raw.h>

#include <malloc.h>
#include <string.h>
#include <screen.h>

void init_file_system(file_t **list)
{
    files = lld_init();

    for (int i = 0; list[i]; i++) {
        /*file_t *cur = malloc(sizeof(file_t));

        cur->name = strdup(list[i]->name);
        cur->size = list[i]->size;
        cur->content = malloc(cur->size);
        memcpy(cur->content, list[i]->content, list[i]->size);*/

        //write_screen(cur->name, strlen(cur->name));

        lld_insert(files, 0, list[i]);
    }
}
