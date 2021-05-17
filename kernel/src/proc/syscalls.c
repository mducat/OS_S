
#include <screen.h>

void syscall_handler(uint64_t num, uint64_t par1, uint64_t par2)
{
    write_screen((char *)par1, (size_t) par2);
    refresh();
}
