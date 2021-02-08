
#ifndef _DEV_SER_H
#define _DEV_SER_H

#define PORT 0x3f8          // COM1

void serial_out(char *str);
void write_serial(char c);
int  init_serial();

#endif
