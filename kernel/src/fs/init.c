
#include <fs.h>
#include <files_raw.h>

#include <malloc.h>
#include <string.h>

void init_file_system(void)
{
    files = lld_init();

    for (int i = 0; f_raw[i]; i++) {
        file_t *cur = malloc(sizeof(file_t));

        cur->name = strdup(f_nam[i]);
        cur->size = f_siz[i];
        cur->content = f_raw[i];
        //memcpy(cur->content, f_raw[i], f_siz[i]);

        lld_insert(files, 0, cur);
    }
}
