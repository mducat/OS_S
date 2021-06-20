

#include <lld.h>
#include <oss.h>
#include <my.h>

#include "instruction.h"
#include <lld.h>

instruction_t *generateInstruction(char *name, int id) {
    char str1[] = "0";
    char *generateInstructionChars[] = {
        str1,
        str1,
        str1,
        str1,
        str1,
        str1,
        str1,
    };
    
    instruction_t *inst = malloc(sizeof(instruction_t));
    inst->name = strToWords(name, ' ');
    inst->func_id = id;
    OpCode_t *op = OpCode_funcs(id, generateInstructionChars);
    inst->c_size = op->c_size;
    free(op);
    return inst;
}

OpCode_t *OpCode_init(int size, unsigned char *code) {
    OpCode_t *op = malloc(sizeof(OpCode_t) + size);
    op->c_size = size;
    memcpy(op->c, (char *)code, size);
    return op;
}

OpCode_t *OpCode_ADD_r_r(char **strs) {
    int r1 = strtol(strs[1]+1, 0, 0);
    int r2 = strtol(strs[2]+1, 0, 0);
    unsigned char thisOpcode[] = {
        0x48, 0x3, // add
        0xc0 | CHAR_TO_LEFT_REGISTER(r1) | CHAR_TO_RIGHT_REGISTER(r2)
    };
    OpCode_t *opcode = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return opcode;
}

OpCode_t *OpCode_JMP_relativ(char **strs) {
    int adrr = strtol(strs[1]+1, 0, 0);
    unsigned char thisOpcode[] = {
        0xe9, 
        ADDRESS_TO_4CHARS(adrr)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_RET(char **strs) {
    unsigned char thisOpcode[] = {
        0xc3 
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_MOV_r_r(char **strs) {
    int r2 = strtol(strs[1]+1, 0, 0);
    int r1 = strtol(strs[2]+1, 0, 0);
    unsigned char thisOpcode[] = {
        0x48, 0x89, // mov r to r
        REG_MOD_register_displacement | CHAR_TO_LEFT_REGISTER(r1) | CHAR_TO_RIGHT_REGISTER(r2)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_MOV_r_mem(char **strs) {
    int Rsrc = strtol(strs[1]+1, 0, 0);
    int Radrr = strtol(strs[2]+1, 0, 0);
    int offset = strtol(strs[3]+1, 0, 0);
    
    unsigned char thisOpcode[] = {
        0x48, 0x89 |OP_D,
        REG_MOD_four_byte_signed_displacement | CHAR_TO_LEFT_REGISTER(Rsrc) | CHAR_TO_RIGHT_REGISTER(Radrr), // mov r to r
        ADDRESS_TO_4CHARS(offset)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

// 3ef0:	48 89 7d e8          	mov    QWORD PTR [rbp-0x18],rdi
// 3ef4:	48 89 75 e0          	mov    QWORD PTR [rbp-0x20],rsi

OpCode_t *OpCode_MOV_mem_r(char **strs) {
    int Radrr = strtol(strs[1]+1, 0, 0);
    int offset = strtol(strs[2]+1, 0, 0);
    int Rsrc = strtol(strs[3]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x48, 0x89,
        REG_MOD_four_byte_signed_displacement | CHAR_TO_LEFT_REGISTER(Rsrc) | CHAR_TO_RIGHT_REGISTER(Radrr), // mov r to r
        ADDRESS_TO_4CHARS(offset)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_MOV_r_li(char **strs) {
    char Rdest = strtol(strs[1]+1, 0, 0);
    long int val = strtol(strs[2]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x48, 
        REG_MOD_four_byte_signed_displacement | CHAR_TO_LEFT_REGISTER(0b111) | CHAR_TO_RIGHT_REGISTER(Rdest), // mov r to r
        ADDRESS_TO_8CHARS(val)
    };

    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_CALL(char **strs) {
    int adrr = strtol(strs[1]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0xe8,
        ADDRESS_TO_4CHARS(adrr)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_JE(char **strs) {
    int adrr = strtol(strs[1]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x74,
        ADDRESS_TO_1CHARS(adrr)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_CMP_r_r(char **strs) {
    int r1 = strtol(strs[1]+1, 0, 0);
    int r2 = strtol(strs[2]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x48, 0x3b,
        REG_MOD_register_displacement | CHAR_TO_LEFT_REGISTER(r1) | CHAR_TO_RIGHT_REGISTER(r2), // mov r to r
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

/*OpCode_t *OpCode_CMP_r_li(char **strs) {
    int r1 = strtol(strs[1]+1, 0, 0);
    long int nb = strtol(strs[2]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x48, 0x83,
        REG_MOD_register_displacement | CHAR_TO_LEFT_REGISTER(7) | CHAR_TO_RIGHT_REGISTER(r1), // mov r to r
        ADDRESS_TO_8CHARS(nb)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}*/

OpCode_t *OpCode_JE_i(char **strs) {
    int nb = strtol(strs[1]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x0f, 0x84,
        ADDRESS_TO_4CHARS(nb)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_JNE_i(char **strs) {
    int nb = strtol(strs[1]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x0f, 0x85,
        ADDRESS_TO_4CHARS(nb)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_JBE_i(char **strs) {
    int nb = strtol(strs[1]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x0f, 0x86,
        ADDRESS_TO_4CHARS(nb)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_JGE_i(char **strs) {
    int nb = strtol(strs[1]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x0f, 0x8d,
        ADDRESS_TO_4CHARS(nb)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_JG_i(char **strs) {
    int nb = strtol(strs[1]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x0f, 0x8f,
        ADDRESS_TO_4CHARS(nb)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_JB_i(char **strs) {
    int nb = strtol(strs[1]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x0f, 0x82,
        ADDRESS_TO_4CHARS(nb)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_PUSH_r(char **strs) {
    int r1 = strtol(strs[1]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x50 + r1
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_PUSH_i(char **strs) {
    int li1 = strtol(strs[1]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x40, 0x68,
        ADDRESS_TO_4CHARS(li1)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_POP_r(char **strs) {
    int r1 = strtol(strs[1]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x58 + r1
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_SYSCALL(char **strs) {
    unsigned char thisOpcode[] = {
        0x0f, 0x05
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_INT(char **strs) {
    char code = strtol(strs[1]+1, 0, 0);
    unsigned char thisOpcode[] = {
        0xcd, 
        ADDRESS_TO_1CHARS(code)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}


OpCode_t *OpCode_IMUL_r_r(char **strs) {
    int r1 = strtol(strs[1]+1, 0, 0);
    int r2 = strtol(strs[2]+1, 0, 0);
    unsigned char thisOpcode[] = {
        0x48, 0x0f, 0xaf,
        REG_MOD_register_displacement | CHAR_TO_LEFT_REGISTER(r1) | CHAR_TO_RIGHT_REGISTER(r2), // mov r to r
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_IDIV_r(char **strs) {
    int r1 = strtol(strs[1]+1, 0, 0);
    unsigned char thisOpcode[] = {
        0x48, 0xf7,
        REG_MOD_register_displacement | CHAR_TO_LEFT_REGISTER(0b111) | CHAR_TO_RIGHT_REGISTER(r1), // mov r to r
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_funcs(int id, char **strs) {
    if (id == 0) {
        return OpCode_ADD_r_r(strs);
    } else if (id == 1) {
        return OpCode_JMP_relativ(strs);
    } else if (id == 2) {
        return OpCode_RET(strs);
    } else if (id == 3) {
        return OpCode_MOV_r_r(strs);
    } else if (id == 4) {
        return OpCode_MOV_r_mem(strs);
    } else if (id == 5) {
        return OpCode_MOV_mem_r(strs);
    } else if (id == 6) {
        return OpCode_MOV_r_li(strs);
    } else if (id == 7) {
        return OpCode_CALL(strs);
    } else if (id == 8) {
        return OpCode_JE(strs);
    } else if (id == 9) {
        return OpCode_CMP_r_r(strs);
    } else if (id == 10) {
        return OpCode_JE_i(strs);
    } else if (id == 11) {
        return OpCode_JNE_i(strs);
    } else if (id == 12) {
        return OpCode_JBE_i(strs);
    } else if (id == 13) {
        return OpCode_JGE_i(strs);
    } else if (id == 14) {
        return OpCode_JG_i(strs);
    } else if (id == 15) {
        return OpCode_JB_i(strs);
    } else if (id == 16) {
        return OpCode_PUSH_r(strs);
    } else if (id == 17) {
        return OpCode_PUSH_i(strs);
    } else if (id == 18) {
        return OpCode_POP_r(strs);
    } else if (id == 19) {
        return OpCode_SYSCALL(strs);
    } else if (id == 20) {
        return OpCode_INT(strs);
    } else if (id == 21) {
        return OpCode_IMUL_r_r(strs);
    } else if (id == 22) {
        return OpCode_IDIV_r(strs);
    }
    printf("youre not supose to be here\n");
    return 0;
}