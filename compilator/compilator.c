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

void rmDoubledCHar(char *str, char c) {
    int p = 0;
    int i = 0;
    while (str[p] == c) 
        p++;
    for (; str[p]; p++){
        if (str[p] == c && str[p+1] == c)
            continue;
        str[i++] = str[p];
    }
    i--;
    while (str[i] == c) 
        str[i--] = 0;
    str[i+1] = 0;
}

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

typedef struct instruction {
    char **name;
    char *args; // 1 = r, 2 = adrr, 0 = end 
    OpCode_t *(*generate)(char **args);
} instruction_t;

instruction_t *generateInstruction(char *name, OpCode_t *(*func)(char **)) {
    instruction_t *inst = malloc(sizeof(instruction_t));
    inst->name = strToWords(name, ' ');
    inst->generate = func;
    return inst;
}

OpCode_t *compileAdd(char **strs) {
    unsigned char thisOpcode[] = {
        0x01, // add
        REG_MOD_Register_indirect | CHAR_TO_LEFT_REGISTER(0) | CHAR_TO_RIGHT_REGISTER(0)
    };
    OpCode_t *opcode = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return opcode;
}

OpCode_t *compileJMP(char **strs) {
    unsigned char thisOpcode[] = {
        0xe9, 
        ADDRESS_TO_4CHARS(atoi(strs[0]))
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

instruction_t **instructionsSet = 0;
#define PUSHBACK(lld, data) lld_insert(lld, lld_len(lld), data)
void generateInstructionsSet() {
    lld_t *lld = lld_init();
    

    PUSHBACK(lld, generateInstruction("add R R", &compileAdd));
    PUSHBACK(lld, generateInstruction("jmp x", &compileJMP));


    instructionsSet = (instruction_t **)lld_lld_to_tab(lld);
    lld_free(lld);
}

void freeInstructionsSet() {
    for (int i = 0; instructionsSet[i]; i++) {
        free(instructionsSet[i]);
    }
    free(instructionsSet);
}

void printInstructionsSet() {
    printf("-----SET-----\n");
    for (int i = 0; instructionsSet[i]; i++) {
        printf("%4i| ", i);
        for (int j = 0; instructionsSet[i]->name[j]; j++) {
            printf(" %s", instructionsSet[i]->name[j]);
        }
        printf("\n");
    }
    printf("---END SET---\n");
}


char registers[][3] = {
    "rax",
    "rcx",
    "rdx",
    "rbx",
    "rsp",
    "rbp",
    "rsi",
    "rdi"
};

char aliases[][32] = {
    "rax", "r0",
    "rcx", "r1",
    "rdx", "r2",
    "rbx", "r3",
    "rsp", "r4",
    "rbp", "r5",
    "rsi", "r6",
    "rdi", "r7",
    "add", "i0",
    "jmp", "i1",
    "add", "i2",
};

int main() {
    // init instruction set
    generateInstructionsSet();
    printInstructionsSet();

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

    int i = 0;
    printf("\n");
    for (lld_t *mv = file->next; mv; mv = mv->next, i++) {
        // rm double spaces
        rmDoubledCHar(mv->data, ' ');
        printf("%4i|  %s\n", i, (char *)mv->data);
        char **words = strToWords(mv->data, ' ');
        // aply aliases
        for (int i = 0; words[i]; i++) {
            for (int j = 0; j < sizeof(aliases)/sizeof(*aliases); j += 2) {
                if (!strcmp(words[i], aliases[j])) {
                    free(words[i]);
                    words[i] = strdup(aliases[j+1]);
                    break;
                }
            }
        }
        free(mv->data);
        mv->data = words;
    }
    printf("##############\n");

    i = 0;
    for (lld_t *mv = file->next; mv; mv = mv->next, i++) {
        printf("%4i| ", i);
        char **words = mv->data;
        for (int i = 0; words[i]; i++) {
            printf(" %s", words[i]);
        }
        printf("\n");
    }

    // free array
    for (lld_t *mv = file->next; mv; mv = mv->next, i++) {
        char **words = mv->data;
        for (int j = 0; words[j] != 0; j++)
            free(words[j]);
        free(words);
    }
    lld_free(file);

    freeInstructionsSet();
    return 0;
}