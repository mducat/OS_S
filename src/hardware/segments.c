
#include "segments.h"
#include "interrupts.h"

int encode_entry(uint8_t *dest, gdt_entry_t source)
{
    if (source.limit > UINT16_MAX * 16 && (source.limit % 4096) != 0xFFF)
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
    uint8_t res = 1 << 7;
    uint8_t priv_secured = info.privilege & 0x3;
    uint1_t descriptor = info.system;

    res |= info.read_write << 1;
    res |= info.direction  << 2;
    res |= info.executable << 3;
    res |= descriptor      << 4;
    res |= priv_secured    << 5;
    return (res);
}

void load_gdt(uint8_t *gdt)
{
/*  ptr[0] = 65535 << 16;
    ptr[1] = ((uintptr_t) gdt) & 0xFFFFFFFF;
    asm volatile("lgdt (%0)"
                 :
                 : "p" ((char *) ptr + 2)
                 :);*/
    uintptr_t gdt_loc = (uintptr_t) gdt;
    uint32_t ptr[4] = {0};
  
    ptr[0] = (65535) | (gdt_loc & 0xFFFF) << 16;
    ptr[1] = (gdt_loc & 0xFFFF0000) >> 16;
    asm volatile("lgdt %0"
                 : 
                 : "m" (ptr)
                 :);
                 
}

uint16_t get_kernel_code_location(void)
{
    return (0x38); // TODO: get kernel code location dynamically
}

void setup_gdt(void)
{
    static uint8_t *gdt = (uint8_t *) 0x1000;//[256];
    seg_access_t kcode_access = {
        .read_write = true,
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
        .limit = 0xFFFFFFFF,
        .access = kernel_access
    };
    gdt_entry_t kdata_segment = {
        .base = 0,
        .limit = 0xFFFFFFFF,
        .access = kernel_data
    };
    for (int i = 0; i < 65536; i++)
        gdt[i] = 0;
    encode_entry(gdt + 0x08, kcode_segment);
    encode_entry(gdt + 0x10, kdata_segment);

    load_gdt(gdt);
}
