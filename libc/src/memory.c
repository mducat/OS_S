
#include <stdint.h>

void *memset(void *ptr, char c, int size)
{
    int i = 0;
    char *str = (char *) ptr;

    while (size--)
        str[i++] = c;
    return (ptr);
}

void *memcpy(void *dest, void *src, size_t n)
{
    char *p_dest = (char *) dest;
    char *p_src = (char *) src;
    int i = 0;

    while (n--) {
        p_dest[i] = p_src[i];
        i++;
    }
    return (dest);
}
