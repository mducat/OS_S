
#ifndef _TYPES_H
#define _TYPES_H

#if defined __x86_64__ && !defined __ILP32__
# define __WORDSIZE        64
#else
# define __WORDSIZE        32
#endif

typedef char                    int8_t;
typedef short int               int16_t;

typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;

#if __WORDSIZE == 32

typedef long int                int32_t;
typedef long long int           int64_t;

typedef unsigned long int       uint32_t;
typedef unsigned long long int  uint64_t;

#else

typedef int                     int32_t;
typedef long int                int64_t;

typedef unsigned int            uint32_t;
typedef unsigned long int       uint64_t;

#endif

typedef long  intptr_t;
typedef _Bool bool;
typedef _Bool uint1_t;


#define true  (1)
#define false (0)

#define INT8_MIN   (-127-1)
#define INT16_MIN  (-32767-1)
#define INT32_MIN  (-2147483647-1)
#define INT64_MIN  (-9223372036854775807-1)

#define INT8_MAX   (127)
#define INT16_MAX  (32767)
#define INT32_MAX  (2147483647)
#define INT64_MAX  (9223372036854775807)

#define UINT8_MAX  (255)
#define UINT16_MAX (65535)
#define UINT32_MAX (4294967295U)
#define UINT64_MAX (18446744073709551615)

#endif
