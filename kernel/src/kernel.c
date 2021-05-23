
#include <fs.h>
#include <gdt.h>
#include <irq.h>
#include <malloc.h>
#include <string.h>
#include <shell.h>

#include <screen.h>
#include <kboot.h>

void freeze(void)
{
    __asm__ __volatile__ ("cli\n\r"
                          "hlt");
}

void k_start(boot_t *data)
{
    if (data->sig != 0x42)
        freeze(); // kernel panic: Where was this booted from ??

    init_malloc(MEM_START, MEM_END);
    init_file_system();

    disp = malloc(sizeof(disp_t));
    disp->screen = data->screen;
    disp->back = malloc(data->screen->buf_size);
    memset(disp->back, 0, data->screen->buf_size);

    init_interrupts();
    init_shell();

    asm volatile("hlt");
    // prevent $pc from exiting kernel
}
