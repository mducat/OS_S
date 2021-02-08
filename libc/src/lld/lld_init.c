
#include <lld.h>
#include <stdlib.h>

lld_t *lld_init(void)
{
    lld_t *lld = malloc(sizeof(lld_t));
    lld->next = 0;
    lld->prev = 0;
    lld->data = 0;
    return (lld);
}
