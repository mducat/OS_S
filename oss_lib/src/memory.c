
#include <oss.h>

void *malloc(size_t n)
{
    int num = 2;
    void *res = 0;

    asm volatile("int $0x30" :
                 : "a" (num),
                   "b" (n));

    asm("" : "=a" (res));
    return res;
}

void free(void *ptr)
{
    int num = 3;

    asm volatile("int $0x30" :
                 : "a" (num),
                   "b" (ptr));
}
