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
