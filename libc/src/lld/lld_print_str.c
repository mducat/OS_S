
#include <lld.h>
#include <stdlib.h>

//#include "screen.h"

void lld_print_str(lld_t *lld)
{
    for (lld = lld->next; lld; lld = lld->next){
        //write_screen(lld->data, my_strlen(lld->data));
        //write_screen(" | ", 3);
    }
    //write_screen("\n", 1);
}
