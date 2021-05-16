
#include <types.h>

void *memset(void *dest, char c, size_t size)
{
    uint8_t *ptr = (uint8_t *) dest;

    while (size--)
        *ptr++ = c;
    return dest;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    uint64_t *p_dest = (uint64_t *) dest;
    uint64_t *p_src  = (uint64_t *) src;

    while ((n -= 8) >0)
        *p_dest++ = *p_src++;
    return dest;
}
