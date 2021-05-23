
#include <oss.h>

size_t write_raw(char *str, size_t size)
{
    int num = 0;
    size_t n = 0;

    asm volatile("int $0x30" :
                 : "a" (num),
                   "b" (str),
                   "c" (size));

    asm("" : "=a" (n));
    return n;
}

size_t write(char *str)
{
    int num = 0;
    size_t size = 0;
    size_t n = 0;

    while (str[size++]);

    asm volatile("int $0x30" :
                 : "a" (num),
                   "b" (str),
                   "c" (size));

    asm("" : "=a" (n));
    return n;
}

void refresh(void)
{
    int num = 1;

    asm volatile("int $0x30" :
                 : "a" (num));
}
