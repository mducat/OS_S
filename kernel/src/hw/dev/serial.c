
#include <dev/serial.h>
#include <sys/io.h>

int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}

int ser_get_display_length(uint64_t nbr, char const *base)
{
    int i = 0;
    unsigned long long int len = 0;

    for (; base[len] != '\0'; len++);
    while (nbr >= len){
        nbr /= len;
        i++;
    }
    return (i);
}

void ser_my_putnbr_base(uint64_t nbr, char const *base,
                        int disp_len)
{
    unsigned long long int len = 0;
    int i = 0;
    int checker;
    char mem_display[disp_len];

    for (; base[len] != '\0'; len++);
    while (nbr >= len){
        checker = nbr % len;
        nbr /= len;
        mem_display[i++] = base[checker];
    }
    mem_display[i] = base[nbr % len];
    for (int j = disp_len - 1; j != -1; j--)
        write_serial(mem_display[j]);
}

void serial_write_addr(void *ptr)
{
    char *base = "0123456789ABCDEF";
    serial_out("0x");
    int len = ser_get_display_length((uint64_t) ptr, base);
    ser_my_putnbr_base((uint64_t) ptr, base, len);
}

void write_nbr_serial(int nb)
{
    int count = 100000000;
    int len = 0;
    char print[] = {0, 0};
    nb < 0 ? nb *= -1, serial_out("-"), len++, 1 : 0;

    while (nb / count == 0 && count > 1)
        count = count / 10;

    while (count != 0){
        print[0] = '0' + nb / count;
        serial_out(print);
        len++;
        nb -= (nb/count) * count;
        count = count / 10;
    }
}

void write_serial(char c) {
   while (is_transmit_empty() == 0);
 
   outb(PORT, c);
}

void serial_out(char *st)
{
    for (int i = 0; st[i]; i++)
        write_serial(st[i]);
}

int init_serial()
{
    outb(PORT + 1, 0x00);    // Disable all interrupts
    outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(PORT + 1, 0x00);    //                  (hi byte)
    outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
    
    // Check if serial is faulty (i.e: not same byte as sent)
    if(inb(PORT + 0) != 0xAE) {
        return 1;
    }
    
    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(PORT + 4, 0x0F);
    
    return 0;
}
