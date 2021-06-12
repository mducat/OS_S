
#include <oss.h>

size_t write_raw(char *str, size_t size)
{
    int num = 0;
    size_t n = 0;

    LOAD3(num, str, size)
    asm volatile ("int $0x30");

    n = (size_t) *((uint64_t *) 0x1234560);
    return n;
}

size_t write(char *str)
{
    int num = 0;
    size_t size = 0;
    size_t n = 0;

    while (str[size++]);

    LOAD3(num, str, size)
    asm volatile ("int $0x30");

    n = (size_t) *((uint64_t *) 0x1234560);
    return n;
}

void refresh(void)
{
    int num = 1;

    LOAD1(num)
    asm volatile("int $0x30");
}

int read(void)
{
    int num = 9;
    int res = 0;

    LOAD2(num, res)
    asm volatile("int $0x30");

    return res;
}
