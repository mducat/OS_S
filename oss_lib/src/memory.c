
#include <oss.h>

void *malloc(size_t n)
{
    int num = 2;
    void *res = 0;

    LOAD2(num, n)
    asm volatile("int $0x30");

    res = (void *) *((uint64_t *) 0x1234560);
    return res;
}

void free(void *ptr)
{
    int num = 3;

    LOAD2(num, ptr)
    asm volatile("int $0x30");
}
