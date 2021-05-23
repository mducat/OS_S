
#include <oss.h>
#include <lld.h>

int main(void)
{
    write("OSS terminal. commands:\n");
    write("help: display this message.\n");
    write("cat: concatenates files into display.\n");
    write("ls: list files.\n");
    refresh();
    return 0;
}
