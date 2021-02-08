
#ifndef _DEV_PS2_H
#define _DEV_PS2_H

#define PS2_CMD    (0x64)
#define PS2_DATA   (0x60)


#define PS2_ACK          (0xFA)
#define PS2_FAIL         (0xFC)
#define PS2_STEST_REP    (0x55)

#define PS2_CWD_R_CONF       (0x20)
#define PS2_CWD_W_CONF       (0x60)

#define PS2_CWD_DOWN_FIRST   (0xAD)
#define PS2_CWD_DOWN_SECOND  (0xA7)

#define PS2_CWD_UP_FIRST     (0xAE)
#define PS2_CWD_UP_SECOND    (0xA8)

#define PS2_CWD_SELF_TEST    (0xAA)
#define PS2_CWD_TEST_FIRST   (0xAB)
#define PS2_CWD_TEST_SECOND  (0xA9)

#define PS2_RESET   (0xFF)

void init_ps2(void);

#endif
