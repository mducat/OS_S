
#include <oss.h>

void write(char *str, size_t size)
{
    int num = 0;

    asm volatile("int $0x30" :
                 : "a" (num),
                   "b" (str),
                   "c" (size));
}
