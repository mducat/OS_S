
#include <irq.h>
#include <kernel.h>
#include <screen.h>
#include <shell.h>

#include <dev/kbd.h>

#include <sys/io.h>

char next = 0;
char extra = 0;
int flags = 0;

int read(void)
{

wait:
    while ((inb(KBD_STATUS) & 1) == 0);

    uint8_t input = inb(KBD_DATA);

    if (input == 0xF0) {
        next = 1;
        goto wait;
    }

    if (input == 0xE0) {
        extra = 1;
        goto wait;
    }


    if (extra && !next && input != 0xE0 && input != 0xF0) {

        switch (input) {
        case 0x6B:
            flags |= CUR_LEFT;
            break;
        case 0x72:
            flags |= CUR_DOWN;
            break;
        case 0x74:
            flags |= CUR_RIGHT;
            break;
        case 0x75:
            flags |= CUR_UP;
            break;
        default: break;
        }

        extra = 0;

    } else {

        switch (input) {
        case 0x11:
            flags |= LEFT_ALT;
            break;
        case 0x12:
            flags |= LEFT_SHIFT;
            break;
        case 0x14:
            flags |= LEFT_CTRL;
            break;
        case 0x76:
            flags |= ESCAPE;
            break;
        default: break;
        }
        
    }

    if (next && input != 0xF0) {
        next = 0;

        if (extra && input != 0xE0) {

            switch (input) {
            case 0x6B:
                flags &= ~CUR_LEFT;
                break;
            case 0x72:
                flags &= ~CUR_DOWN;
                break;
            case 0x74:
                flags &= ~CUR_RIGHT;
                break;
            case 0x75:
                flags &= ~CUR_UP;
                break;
            default: break;
            }

            extra = 0;
            
        } else {

            switch (input) {
            case 0x11:
                flags &= ~LEFT_ALT;
                break;
            case 0x12:
                flags &= ~LEFT_SHIFT;
                break;
            case 0x14:
                flags &= ~LEFT_CTRL;
                break;
            case 0x76:
                flags &= ~ESCAPE;
                break;
            default: break;
            }
            
        }

        goto wait;
    }


    int print = scan_code_set_2_fr[(uint8_t) input];

    print |= flags;
    return (int) print;
}

void irq1_handler(void)
{
    end_of_interrupt(1);

    uint8_t status = inb(KBD_STATUS);
    static int next = 0;

    if ((status & 1) == 0)
        return;
    
    uint8_t input = inb(KBD_DATA);

    if (input == 0xF0) {
        next = 1;
        return;
    }

    if (input == 0xE0) {
        extra = 1;
        return;
    }


    if (extra && !next && input != 0xE0 && input != 0xF0) {

        switch (input) {
        case 0x6B:
            flags |= CUR_LEFT;
            break;
        case 0x72:
            flags |= CUR_DOWN;
            break;
        case 0x74:
            flags |= CUR_RIGHT;
            break;
        case 0x75:
            flags |= CUR_UP;
            break;
        default: break;
        }

        extra = 0;

    } else {

        switch (input) {
        case 0x11:
            flags |= LEFT_ALT;
            break;
        case 0x12:
            flags |= LEFT_SHIFT;
            break;
        case 0x14:
            flags |= LEFT_CTRL;
            break;
        case 0x76:
            flags |= ESCAPE;
            break;
        default: break;
        }
        
    }

    if (next && input != 0xF0) {
        next = 0;

        if (extra && input != 0xE0) {

            switch (input) {
            case 0x6B:
                flags &= ~CUR_LEFT;
                break;
            case 0x72:
                flags &= ~CUR_DOWN;
                break;
            case 0x74:
                flags &= ~CUR_RIGHT;
                break;
            case 0x75:
                flags &= ~CUR_UP;
                break;
            default: break;
            }

            extra = 0;
            
        } else {

            switch (input) {
            case 0x11:
                flags &= ~LEFT_ALT;
                break;
            case 0x12:
                flags &= ~LEFT_SHIFT;
                break;
            case 0x14:
                flags &= ~LEFT_CTRL;
                break;
            case 0x76:
                flags &= ~ESCAPE;
                break;
            default: break;
            }
            
        }

        return;
    }

    int print = scan_code_set_2_fr[(uint8_t) input];
    send_tty(print);
}
