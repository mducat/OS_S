
#include <lld.h>
#include <stdlib.h>

//#include "screen.h"

void lld_print_int(lld_t *lld)
{
    for (lld = lld->next; lld; lld = lld->next){
        //my_put_nbr((long int)lld->data);
        //write_screen(" ", 1);
    }
    //write_screen("\n", 1);
}
