
#include <types.h>

void *memset(void *dest, char c, size_t size)
{
    uint8_t *ptr = (uint8_t *) dest;

    while (size--)
        *ptr++ = c;
    return dest;
}

void *memcpy(void *dest, void const *src, size_t n)
{
    uint8_t *p_dest = (uint8_t *) dest;
    uint8_t *p_src  = (uint8_t *) src;

    while (n--)
        *p_dest++ = *p_src++;
    return dest;
}
