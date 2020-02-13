/*
** EPITECH PROJECT, 2020
** OS_S
** File description:
** Global descriptor table loading
*/

#include "segments.h"

int encode_entry(uint8_t *dest, gdt_entry_t source)
{
    if (source.limit > UINT16_MAX * 16 && (source.limit % 4096) != 0)
        return (84);
    if (source.limit > UINT16_MAX * 16){
        source.limit >>= 12;
        dest[6] = 1 << 6 | 1 << 7;
    }else{
        dest[6] = 1 << 6;
    }

    source.limit &= 0xFFFFF;
    dest[0]  = source.limit & 0xFF;
    dest[1]  = source.limit >> 8;
    dest[6] |= source.limit >> 16;

    dest[2] = (source.base >> 0 ) & 0xFF;
    dest[3] = (source.base >> 8 ) & 0xFF;
    dest[4] = (source.base >> 16) & 0xFF;
    dest[7] = (source.base >> 24) & 0xFF;

    dest[5] = source.access;
    return (0);
}

uint8_t encode_access(seg_access_t info)
{
    uint8_t res = 1 << 6;
    uint8_t priv_secured = info.privilege & 0x3;

    res |= info.read_write << 1;
    res |= info.direction  << 2;
    res |= info.executable << 3;
    res |= info.system     << 4;
    res |= priv_secured    << 5;
    return (res);
}

void activate_interrupts(void)
{
    asm volatile("sti");
}

void deactivate_interrupts(void)
{
    asm volatile("cli");
}

void load_gdt(uint8_t *gdt)
{
    asm volatile("lgdt (%0)"
                 : /* no output */
                 : "p" (gdt)
                 :);
}

void setup_gdt(void)
{
    uint8_t gdt[256];
    seg_access_t kcode_access = {
        .read_write = false,
        .direction = 0,
        .executable = true,
        .system = true,
        .privilege = 0
    };
    uint8_t kernel_access = encode_access(kcode_access);

    seg_access_t kdata_access = {
        .read_write = true,
        .direction = 0,
        .executable = false,
        .system = true,
        .privilege = 0
    };
    uint8_t kernel_data = encode_access(kdata_access);

    gdt_entry_t kcode_segment = {
        .base = 0,
        .limit = 0xFFFFF,
        .access = kernel_access
    };
    gdt_entry_t kdata_segment = {
        .base = 0,
        .limit = 0xFFFFF,
        .access = kernel_data
    };
    encode_entry(gdt + 0, kcode_segment);
    encode_entry(gdt + 8, kdata_segment);

    load_gdt(gdt);
    activate_interrupts();
}
