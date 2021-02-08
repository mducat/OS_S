
#include <types.h>

int memset(void *ptr, char c, int size)
{
    int i = 0;
    uint8_t *str = (uint8_t *) ptr;

    for (; str[i] && i < size; i++) {
        str[i] = c;
    }
    return (0);
}

int memcpy(void *ptr_d, void *ptr_s)
{
    //__try{ // Can't compile ?
    uint8_t *dest = (uint8_t *) ptr_d;
    uint8_t *src = (uint8_t *) ptr_s;
    int x = 0;

    for (; src[x]; x++)
        dest[x] = src[x];
    dest[x] = 0;
    return (0);
    //}__except{
    //    return (84);
    //}
}
