 //http://www.c-jump.com/CIS77/CPU/x86/lecture.html#X77_0120_encoding_add
 //https://www.felixcloutier.com/x86/


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
#include <stdarg.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "my.h"


#define ADDRESS_TO_8CHARS(addr) (unsigned char)((addr) >> (8*0)), (unsigned char)((addr) >> (8*1)), (unsigned char)((addr) >> (8*2)), (unsigned char)((addr) >> (8*3)), (unsigned char)((addr) >> (8*4)), (unsigned char)((addr) >> (8*5)), (unsigned char)((addr) >> (8*6)), (unsigned char)((addr) >> (8*7))
#define ADDRESS_TO_4CHARS(addr) (unsigned char)((addr) >> (8*0)), (unsigned char)((addr) >> (8*1)), (unsigned char)((addr) >> (8*2)), (unsigned char)((addr) >> (8*3))
#define ADDRESS_TO_2CHARS(addr) (unsigned char)((addr) >> (8*0)), (unsigned char)((addr) >> (8*1))
#define ADDRESS_TO_1CHARS(addr) (unsigned char)((addr) >> (8*0))

#define RELATIVE_CALL_TO_8CHARS(addr, destAdrr, opcodesize) ADDRESS_TO_8CHARS(-(long int)(addr)-(long int)(opcodesize)+(long int)(destAdrr))
#define RELATIVE_CALL_TO_4CHARS(addr, destAdrr, opcodesize) ADDRESS_TO_4CHARS(-(long int)(addr)-(long int)(opcodesize)+(long int)(destAdrr))
#define RELATIVE_CALL_TO_2CHARS(addr, destAdrr, opcodesize) ADDRESS_TO_2CHARS(-(long int)(addr)-(long int)(opcodesize)+(long int)(destAdrr))
#define RELATIVE_CALL_TO_1CHARS(addr, destAdrr, opcodesize) ADDRESS_TO_1CHARS(-(long int)(addr)-(long int)(opcodesize)+(long int)(destAdrr))

#define CHAR_TO_LEFT_REGISTER(r) ((r) << 3)
#define CHAR_TO_RIGHT_REGISTER(r) ((r) << 0)


/*
    442e:	48 c7 c2 ff ff ff ff 	mov    rdx,0xffffffffffffffff
    
    
    4442:	48 8b 45 98          	mov    rax,QWORD PTR [rbp-0x68]
    3fdd:	48 8b 05 fc 50 00 00 	mov    rax,QWORD PTR [rip+0x50fc]        # 0x90e0
    
    4004:	e8 41 e7 ff ff       	call   0x274a


    3f84:	0f 85 8b 00 00 00    	jne    0x4015

    3f6f:	e9 1c 01 00 00       	jmp    0x4090


    3edb:	74 05                	je     0x3ee2

*/

typedef struct OpCode {
    int adrr;
    int c_size;
    char c[0];
} OpCode_t;

OpCode_t *OpCode_init(int size, char *code) {
    OpCode_t *op = malloc(sizeof(OpCode_t) + size);
    op->c_size = size;
    memcpy(op->c, code, size);
    return op;
}



OpCode_t *OpCode_add_r_r(char Rdest, char Rsrc) {
    unsigned char thisOpcode[] = {
        0x01, // add
        REG_MOD_Register_indirect | CHAR_TO_LEFT_REGISTER(Rdest) | CHAR_TO_RIGHT_REGISTER(Rsrc)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

/*OpCode_t *OpCode_call_abs(int adrr) // do not use this{
    unsigned char thisOpcode[] = {
        0xe8, // call 
        RELATIVE_CALL_TO_4CHARS(code2+12, printHello, 5)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}*/


OpCode_t *OpCode_call_relativ(int adrr) {
    unsigned char thisOpcode[] = {
        0xe8, // call 
        ADDRESS_TO_4CHARS(adrr-5)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

// char registers[][10] = { 
//     "eAX", //000
//     "eCX", //001
//     "eDX", //010
//     "eBX", //011
//     "SIB", //100
//     "eBP", //101
//     "eSI", //110
//     "eDI", //111
// };

/*OpCode_t *OpCode_syscall() {
    unsigned char thisOpcode[] = {
        //0x48, 0x8b,
        //CHAR_TO_LEFT_REGISTER(Rdest) | CHAR_TO_RIGHT_REGISTER(Rsrc), // mov r to r
        //ADDRESS_TO_4CHARS(offset)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}*/

    

OpCode_t *OpCode_mov_r_mem(char Rdest, char Radrr, int offset) {
    unsigned char thisOpcode[] = {
        0x48, 0x8b,
        CHAR_TO_LEFT_REGISTER(Rdest) | CHAR_TO_RIGHT_REGISTER(Radrr), // mov r to r
        ADDRESS_TO_4CHARS(offset)
    };

    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_mov_mem_r(char Rsrc, char Radrr, int offset) {
    unsigned char thisOpcode[] = {
        0x48 | OP_D, 0x8b,
        CHAR_TO_LEFT_REGISTER(Rsrc) | CHAR_TO_RIGHT_REGISTER(Radrr), // mov r to r
        ADDRESS_TO_4CHARS(offset)
    };

    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_mov_r_r(char Rdest, char Rsrc) {
    unsigned char thisOpcode[] = {
        0x48, 0x89, // mov r to r
        REG_MOD_Register_indirect | CHAR_TO_LEFT_REGISTER(Rsrc) | CHAR_TO_RIGHT_REGISTER(Rdest)
    };

    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_mov_r_li(char Rdest, long int val) {
    unsigned char thisOpcode[] = {
        48, 
        REG_MOD_four_byte_signed_displacement | CHAR_TO_LEFT_REGISTER(0b111) | CHAR_TO_RIGHT_REGISTER(Rdest), // mov r to r
        ADDRESS_TO_8CHARS(val)
    };

    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_jmp_relativ(int val) {
    unsigned char thisOpcode[] = {
        0xe9, 
        ADDRESS_TO_4CHARS(val)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_ret(int val) {
    unsigned char thisOpcode[] = {
        0xc3, 
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}


/*char instructions[][256] = {
    "write",
    "read",
    "call",
    "add",
    "return",
    "push",
    "pop",
    "jmp",
    "je"
};*/

typedef struct instruction {
    char *name;
    void (*func)();
    char *ucp;
    int *(*params)(char *);
} instruction_t;

typedef struct balise {
    char *name;
    uint adrr;
} balise_t;

instruction_t *init_instruction(char *name, void (*func)(), char *ucp, ...){

    //int (*func)(char *);
    va_list va;
    va_start(va, ucp);

    instruction_t *instruc = malloc(sizeof(instruction_t));
    instruc->name = strdup(name);
    instruc->func = func;

    instruc->ucp = malloc(strlen(name)+strlen(ucp)+1);
    memcpy(instruc->ucp, name, strlen(name));
    memcpy(instruc->ucp+strlen(name), ucp, strlen(ucp)+1);
    return instruc;
}

instruction_t **instructions = 0;


void print_instructions_set() {
    for (int i = 0; instructions[i]; i++) {
        printf("%s | %s | %p\n", instructions[i]->name, instructions[i]->ucp, instructions[i]->func);
    }
}

char return_ucp() {
    
}

#define LLD_ADD_INSTRUCTION(ins) lld_insert(lld, lld_len(lld), ins)
void init_instruction_array() {
    lld_t *lld = lld_init();

    LLD_ADD_INSTRUCTION(init_instruction("return", "", (void *)&OpCode_ret));
    LLD_ADD_INSTRUCTION(init_instruction("jmp", "", (void *)&OpCode_ret));

    instructions = (instruction_t *)lld_lld_to_tab(lld);
}


char registers[][10] = { 
    "r0", //000
    "r1", //001
    "r2", //010
    "r3", //011
    "r4", //100
    "r5", //101
    "r6", //110
    "r7", //111
};

char registerIdOf(char *str) {
    for (int i = 0; i < sizeof(registers); i++) {
        if (strcmp(registers[i], str) == 0)
            return i;
    }
    return -1;
}

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
    printf("id of r1 %i\n", registerIdOf("r1"));
    printf("id of r4 %i\n", registerIdOf("r4"));
    printf("id of r41 %i\n", registerIdOf("r41"));
}


long int print() {
    printf("hello there\n");
    return 3210987; 
}

int illegal_things_test(char *a, int b, long int c) {
    printf("%s %i %li\n", a, b, c);
    return 4567;
}

void rm_char(char *str, char c) {
    int p = 0;
    for (int i = 0; str[p-1]; i++, p++){
        if (str[i] == c);
            p++;
        str[i] = str[p];
    }
}

void refacto_file(lld_t *file) {
    int i = 0;
    for (lld_t *mv = file->next; mv; mv = mv->next, i++){
        if (strlen(mv->data)) {
            lld_pop(file, i);
            i--;
        } else {
            // split line in words
        }

    }
}

int main() {
    init_instruction_array();
    print_instructions_set();
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    void (*printHello)() = (void *)print;

    
    unsigned char *code2 = malloc(0x1000);

    unsigned char code[] = {
        //0b00000001, 0b11000001, // add something
        //0x be 00 01 00 00, // mov esi 0x100
        
        0x48, 0xb8, ADDRESS_TO_8CHARS((long int)printHello), // mov rax (printHello)
        0xff, 0xd0, // call rax
        0xe8, RELATIVE_CALL_TO_4CHARS(code2+12, printHello, 5),
        //0x48, 0xb8, ADDRESS_TO_8CHARS(123456789),
        0xc3 // ret
    };
    printf("code %p\n", code);

    for (int i = 0; i < sizeof(code); i++){
        printf("%.2x ", code[i]);
    }
    printf("\n");

    printf("sizeof(code) = %li\n", sizeof(code));
    for (int i = 0; i < sizeof(code); i++)
        code2[i] = code[i];

    printf("mprotect %i\n", mprotect(code2, sizeof(code), PROT_EXEC | PROT_READ | PROT_WRITE));
    perror("hello ");
    long int (*func)() = (void *)code2;
    //printf("%p %p %p\n", func, code2, &code2);

    long int ret = func();
    printf("ret = %li\n", ret);

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
    refacto_file(file);
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