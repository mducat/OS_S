
#ifndef _OS_S_BOOT
#define _OS_S_BOOT

# ifdef _GNU_EFI

// bootloader macros

#  define CHK_STATUS(x, y, ...)                \
    if (EFI_ERROR(x)) {                        \
        Print(y, ## __VA_ARGS__);              \
        return (x);                            \
    }

#  define TRIGGER(x, y, ...)                    \
    {                                           \
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


typedef struct boot_s {
    uint8_t sig;
} boot_t;

#endif
