
#include <oss.h>

size_t write_raw(char *str, size_t size)
{
    int num = 0;
    size_t n = 0;

    /*asm volatile("int $0x30" :
                 : "a" (num),
                   "b" (str),
                   "c" (size));

    asm("" : "=a" (n));*/
    uint64_t *ptr1 = 0x123456;
    uint64_t *ptr2 = 0x123466;
    uint64_t *ptr3 = 0x123476;

    *ptr1 = num;
    *ptr2 = str;
    *ptr3 = size;
    return n;
}

size_t write(char *str)
{
    int num = 0;
    size_t size = 0;
    size_t n = 0;

    while (str[size++]);
    uint64_t *ptr1 = 0x123456;
    uint64_t *ptr2 = 0x123466;
    uint64_t *ptr3 = 0x123476;

    *ptr1 = num;
    *ptr2 = str;
    *ptr3 = size;

    /*asm volatile("int $0x30" :
                 : "a" (num),
                   "b" (str),
                   "c" (size));

                   asm("" : "=a" (n));*/
    return n;
}

void refresh(void)
{
    int num = 1;

    asm volatile("int $0x30" :
                 : "a" (num));
}

int read(void)
{
    int num = 9;
    int res = 0;

    asm volatile("int $0x30" :
                 : "a" (num));

    asm("" : "=a" (res));
    return res;
}
