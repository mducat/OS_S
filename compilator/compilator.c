 //http://www.c-jump.com/CIS77/CPU/x86/lecture.html#X77_0120_encoding_add
 //https://www.felixcloutier.com/x86/

/* registers id 
    000 eAX
    001 eCX
    010 eDX
    011 eBX
    100 SIB
    101 eBP
    110 eSI
    111 eDI
*/


// op code byte
#define OP_S 0b0000001 // set operand to 16/32 bit mod
#define OP_D 0b0000010 // 0 = adding from reg to mem, 1 = adding from memory to reg 
// d = 0 | add [ebx], al
// d = 1 | add al, [ebx]



// register byte
#define REG_MOD_Register_indirect               0b00000000
#define REG_MOD_one_byte_signed_displacement    0b01000000
#define REG_MOD_four_byte_signed_displacement   0b10000000
#define REG_MOD_register_displacement           0b11000000

#define REG_eAX                                 0b00000000
#define REG_eCX                                 0b00001000
#define REG_eDX                                 0b00010000
#define REG_eBX                                 0b00011000
#define REG_SIB                                 0b00100000
#define REG_eBP                                 0b00101000
#define REG_eSI                                 0b00110000
#define REG_eDI                                 0b00111000

#define REG_RM_eAX                              0b00000000
#define REG_RM_eCX                              0b00000001
#define REG_RM_eDX                              0b00000010
#define REG_RM_eBX                              0b00000011
#define REG_RM_SIB                              0b00000100
#define REG_RM_eBP                              0b00000101
#define REG_RM_eSI                              0b00000110
#define REG_RM_eDI                              0b00000111


// Scale index byte
#define SCALE_INDEX_MULT_1      0b00000000
#define SCALE_INDEX_MULT_2      0b01000000
#define SCALE_INDEX_MULT_4      0b10000000
#define SCALE_INDEX_MULT_8      0b11000000

#define SCALE_INDEX_Reg_EAX     0b00000000
#define SCALE_INDEX_Reg_ECX     0b00001000
#define SCALE_INDEX_Reg_EDX     0b00010000
#define SCALE_INDEX_Reg_EBX     0b00011000
#define SCALE_INDEX_Reg_ill     0b00100000
#define SCALE_INDEX_Reg_EBP     0b00101000
#define SCALE_INDEX_Reg_ESI     0b00110000
#define SCALE_INDEX_Reg_EDI     0b00111000

#define SCALE_BASE_Reg_EAX      0b00000000
#define SCALE_BASE_Reg_ECX      0b00000001
#define SCALE_BASE_Reg_EDX      0b00000010
#define SCALE_BASE_Reg_EBX      0b00000011
#define SCALE_BASE_Reg_ESP      0b00000100
#define SCALE_BASE_Reg_EBP      0b00000101 // Displacement-only if MOD = 00, EBP if MOD = 01 or 10
#define SCALE_BASE_Reg_ESI      0b00000110
#define SCALE_BASE_Reg_EDI      0b00000111

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "my.h"

char instructions[][16] = {
    "ret", //0xc3
    "add", //0x00    
};

/*    
1370:	e8 5b fd ff ff       	call   0x10d0
133d:	ff d2                	call   rdx
*/

 
void here(){}


void print() {
    printf("hello there\n");
}

#define __NR_write 4

#define ADDRESS_TO_8CHARS(addr) addr >> (8*0), addr >> (8*1), addr >> (8*2), addr >> (8*3), addr >> (8*4), addr >> (8*5), addr >> (8*6), addr >> (8*7)
#define ADDRESS_TO_4CHARS(addr) addr >> (8*0), addr >> (8*1), addr >> (8*2), addr >> (8*3)
#define ADDRESS_TO_2CHARS(addr) addr >> (8*0), addr >> (8*1)

#define RELATIVE_CALL_TO_4CHARS(addr, destAdrr, opcodesize) ADDRESS_TO_4CHARS(-addr-opcodesize+destAdrr)

#define M_TEST(val) val/2, val

char **strToWords(const char *str, char split) {
    lld_t *lld = lld_init();
    int p = -1;
    int i = 0;
    for (; str[i]; i++) {
        if (str[i] == split) {
            i--;
            int len = i-p;
            char *str2 = malloc(len+1);
            str2[len] = 0;
            memcpy(str2, str+p+1, len);
            lld_insert(lld, lld_len(lld), str2);
            i++;
            p = i;
        }
    }
    int len = i-p-1;
    char *str2 = malloc(len+1);
    str2[len] = 0;
    memcpy(str2, str+p+1, len);
    lld_insert(lld, lld_len(lld), str2);
    p = i;
    char **tab = (char **)lld_lld_to_tab(lld);
    lld_free(lld);
    return tab;
}

void parseFile(char **file, FILE *dest) {
    //fprintf(dest, "wesh\n");
    for (int i = 0; file[i]; i++){
        char **line = strToWords(file[i], ' ');
        for (int j = 0; line[j]; j++){
            printf("|%s", line[j]);
        }
        printf("|\n");
    }
}

int main() {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    void (*printHello)() = (void *)print;

    

    char code[] = {
        //0b00000001, 0b11000001, // add something
        //0x be 00 01 00 00, // mov esi 0x100
        
        //0x48, 0xb8, ADDRESS_TO_CHARS((long int)printHello), // mov rax (printHello)
        //0xff, 0xd0, // call rax
        0xe8, ADDRESS_TO_4CHARS((int)(code+5) -(int)printHello),

        0xc3 // ret
    };
    unsigned char opcode[] = {RELATIVE_CALL_TO_4CHARS(0x12c2, 0x1110, 5)};
    printf("call %x %x %x %x\n", opcode[0], opcode[1], opcode[2], opcode[3]);

    printf("sizeof(code) = %li\n", sizeof(code));
    long int size = sizeof(code);
    char *code2 = malloc(size);
    for (int i = 0; i < size; i++)
        code2[i] = code[i];

    int a = 0x123456;
    printf("hi %x\n", a);
    (void)a;

    mprotect(code2, size, PROT_EXEC);
    void (*func)() = (void *)code2;
    //printf("%p %p %p\n", func, code2, &code2);

    //here();
    func();

    printf("END\n");
    free(code2);

    FILE *src = fopen("main.coss", "r+");
    lld_t *file = lld_init();
    char *line = 0;
    char go = 1;
    size_t line_len = 0;
    int line_len2 = 0;
    while ((line_len2 = getline(&line, &line_len, src)) > 0){
        if (!line)
            continue;
        if (line[line_len2-1] == '\n') 
            line[line_len2-1] = 0;
        lld_insert(file, lld_len(file), line);
        line = 0;
    }
    free(line);
    fclose(src);
    printf("comiling...\n");
    printf("printing\n");
    char **file_tab = (char **)lld_lld_to_tab(file);
    lld_free(file);
    for (int i = 0; file_tab[i]; i++)
        printf("%s", file_tab[i]);
    printf("PARSING\n");

    FILE *dest = fopen("main.oss", "w");
    parseFile(file_tab, dest);

    // free file
    for (int i = 0; file_tab[i]; i++)
        free(file_tab[i]);
    free(file_tab);
}