
#ifndef _OS_S_BOOT
#define _OS_S_BOOT

#include <efi.h>

#define CHK_STATUS(x, y, ...)                  \
    if (EFI_ERROR(x)) {                        \
        Print(y, ## __VA_ARGS__);              \
        return (x);                            \
    }

#define TRIGGER(x, y, ...)                      \
    {                                           \
        Print(y, ## __VA_ARGS__);               \
        return (x);                             \
    }

#define CHK_FWD(x)                             \
    if (EFI_ERROR(x))                          \
        return (x);                            \

#endif
