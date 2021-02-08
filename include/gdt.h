
#ifndef _SEG_H
#define _SEG_H

#include <types.h>

typedef struct {
    uint32_t base;
    uint32_t limit;
    uint8_t access;
} gdt_entry_t;

typedef struct {
    uint1_t present;
    uint8_t privilege;
    uint1_t system;
    uint1_t executable;
    uint1_t direction;
    uint1_t read_write;
} seg_access_t;

void setup_gdt(void);

uint16_t get_kernel_code_location(void);

#endif
