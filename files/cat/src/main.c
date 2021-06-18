
#include <oss.h>

#define OP_S 0b0000001 // set operand to 16/32 bit mod
#define OP_D 0b0000010 // 0 = adding from reg to mem, 1 = adding from memory to reg 

#define REG_MOD_Register_indirect               0b00000000
#define REG_MOD_one_byte_signed_displacement    0b01000000
#define REG_MOD_four_byte_signed_displacement   0b10000000
#define REG_MOD_register_displacement           0b11000000

#define CHAR_TO_LEFT_REGISTER(r) ((r) << 3)
#define CHAR_TO_RIGHT_REGISTER(r) ((r) << 0)

#define ADDRESS_TO_8CHARS(addr) (unsigned char)((addr) >> (8*0)), (unsigned char)((addr) >> (8*1)), (unsigned char)((addr) >> (8*2)), (unsigned char)((addr) >> (8*3)), (unsigned char)((addr) >> (8*4)), (unsigned char)((addr) >> (8*5)), (unsigned char)((addr) >> (8*6)), (unsigned char)((addr) >> (8*7))
#define ADDRESS_TO_4CHARS(addr) (unsigned char)((addr) >> (8*0)), (unsigned char)((addr) >> (8*1)), (unsigned char)((addr) >> (8*2)), (unsigned char)((addr) >> (8*3))
#define ADDRESS_TO_2CHARS(addr) (unsigned char)((addr) >> (8*0)), (unsigned char)((addr) >> (8*1))
#define ADDRESS_TO_1CHARS(addr) (unsigned char)((addr) >> (8*0))

#define RELATIVE_CALL_TO_8CHARS(addr, destAdrr, opcodesize) ADDRESS_TO_8CHARS(-(long int)(addr)-(long int)(opcodesize)+(long int)(destAdrr))
#define RELATIVE_CALL_TO_4CHARS(addr, destAdrr, opcodesize) ADDRESS_TO_4CHARS(-(long int)(addr)-(long int)(opcodesize)+(long int)(destAdrr))
#define RELATIVE_CALL_TO_2CHARS(addr, destAdrr, opcodesize) ADDRESS_TO_2CHARS(-(long int)(addr)-(long int)(opcodesize)+(long int)(destAdrr))
#define RELATIVE_CALL_TO_1CHARS(addr, destAdrr, opcodesize) ADDRESS_TO_1CHARS(-(long int)(addr)-(long int)(opcodesize)+(long int)(destAdrr))


#define printf(str, ...) {char buf[] = str; printf(buf __VA_OPT__(,) __VA_ARGS__); refresh();}


#define LOAD3(a, b, c) \
    *((uint64_t *) 0x1234560) = (uint64_t) a; \
    *((uint64_t *) 0x1234660) = (uint64_t) b; \
    *((uint64_t *) 0x1234760) = (uint64_t) c;

void *getFlag(void *);
void *gotoFlag(void *target);
void funcyFunc(void *back);
void funcyFunc1();
void funcyFunc2();
void funcyFunc3();


int main(int ac, char **av)
{
    /*printf("salut\n");

    char byte[] = {
        0xcd, // interupt 
            0x30,
        0xe8, // call
            ADDRESS_TO_4CHARS((long int)funcyFunc1-(long int)byte-(long int)7),
        0xc3 // ret
    };
    char heyyyyy[] = "hello there";
    printf("load\n");
    LOAD3(0, heyyyyy, 13);
    printf("goto\n");
    goto *(void *)&byte;
    printf("after goto\n");

    void *target = getFlag(0);
    printf("ret\n");
    return 0;*/
    
    if (ac < 2) {
        printf("%s [filenames ...]\n", av[0]);
        goto out;
    }

    for (int i = 1; i < ac; i++) {
        file_t *file = open(av[i]);

        if (!file) {
            printf(LINE_BR_RED FRONT_BR_RED "%s: " FRONT_BR_YELLOW "%s: "
                   BACK_WHITE FRONT_RED "File not found.\n" RESET_COLOR,
                   av[0], av[i]);
            continue;
        }

        write_raw(file->content, file->size);
        close(file);
    }

out:
    refresh();
    return 0;
}

void *getFlag(void *dsd) {
    getStack:;
    printf("amiam la stack\n");
    return &&getStack;
}

void *gotoFlag(void *target) {
    printf("here\n");
    goto *target;
    printf("wait waht\n");
    return 0;
}

void funcyFunc(void *here) {
    // write(arg);
    
    printf("hello\n");
    goto *here;
    return;
}

void funcyFunc1() {
    // write(arg);
    
    printf("hello\n");
    return;
}

void funcyFunc2() {
    // write(arg);
    
    printf("hello\n");
    return;
}

void funcyFunc3() {
    // write(arg);
    
    printf("hello\n");
    return;
}