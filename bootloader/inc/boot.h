
#ifndef _OS_S_BOOT
#define _OS_S_BOOT

# ifdef _GNU_EFI

// bootloader macros

#  define CHK_STATUS(x, y, ...)                \
    if (EFI_ERROR(x)) {                        \
        Print(L"%H");                          \
        Print(y, ## __VA_ARGS__);              \
        return (x);                            \
    }

#  define TRIGGER(x, y, ...)                    \
    {                                           \
        Print(L"%H");                           \
        Print(y, ## __VA_ARGS__);               \
        return (x);                             \
    }

#  define CHK_FWD(x)                           \
    if (EFI_ERROR(x))                          \
        return (x);                            \


// internal kernel access functions

EFI_STATUS get_kernel(EFI_HANDLE, EFI_FILE **);
EFI_STATUS load_kernel(EFI_HANDLE, EFI_PHYSICAL_ADDRESS *);

EFI_STATUS get_file_info(EFI_FILE *, EFI_FILE_INFO **);

# endif

typedef struct screen_s {
    uint32_t version;
    uint32_t format;

    uint32_t x_len;
    uint32_t y_len;
    uint32_t pix_per_line; // x_len with padding

    uint32_t buf_size;
    void *   p_loc;
} screen_t;

typedef struct mem_map_s {
    uint32_t type;
    uint32_t pad;
    void *   v_start;
    void *   p_start;
    uint64_t n_pages;
    uint64_t attr;
} mmap_t;

typedef enum {
    cold_reset,
    warm_reset,
    shutdown_reset
} reset_type_t;


#ifndef EFIAPI

//#include "../lib/inc/x86_64/efibind.h"

#define EFIAPI  __attribute__((ms_abi))

#endif


typedef uint64_t
(EFIAPI * reset_t)(
    reset_type_t r_type;
    uint64_t     r_status;
    uint64_t     r_size;
    uint16_t *   r_data;
);

typedef struct boot_s {
    uint8_t          sig;
    screen_t *       screen;
    mmap_t   *       mem_map;
    reset_t          reset;
} boot_t;

#endif
