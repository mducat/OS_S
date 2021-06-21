#include <oss.h>

int get_screenX()
{
    int num = 12;
    int res = 0;

    LOAD1(num);
    asm volatile("int $0x30");
    res = (size_t) *((uint64_t *) 0x1234560);
    return res;
}

int get_screenY()
{
    int num = 13;
    int res = 0;

    LOAD1(num)
    asm volatile("int $0x30");
    res = (size_t) *((uint64_t *) 0x1234560);
    return res;
}
