
#ifndef _DEV_SER_H
#define _DEV_SER_H

#define PORT 0x3f8          // COM1

#include <types.h>

void serial_out(char *str);
void write_serial(char c);
void write_nbr_serial(int nb);
void serial_write_addr(void *ptr);
void serial_out_raw(char *st, size_t len);
int  init_serial();

#endif
