
#ifndef _IM_SHOW_H

#include <types.h>

#define BMP_MAGIC (0x4D42)

typedef struct __attribute__ ((packed)) bmp_header {
    uint16_t magic;
    uint32_t size;
    uint16_t _app1;
    uint16_t _app2;
    uint32_t offset;
} bmp_header_t;

typedef struct __attribute__ ((packed)) bmp_info {
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t raw_data_size;
    int32_t h_resolution;
    int32_t v_resolution;
    uint32_t pallete_size;
    uint32_t important_colors;
} bmp_info_header_t;

#endif
