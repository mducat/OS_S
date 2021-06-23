#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include "compilator.h"

typedef struct instruction {
    char **name;
    int c_size;
    OpCode_t *(*generate)(char **args);
} instruction_t;

instruction_t *generateInstruction(char *name, OpCode_t *(*func)(char **));

OpCode_t *OpCode_ADD_r_r(char **strs);
OpCode_t *OpCode_JMP_relativ(char **strs);
OpCode_t *OpCode_RET(char **strs);
OpCode_t *OpCode_MOV_r_r(char **strs);
OpCode_t *OpCode_MOV_r_mem(char **strs);
OpCode_t *OpCode_MOV_mem_r(char **strs);
OpCode_t *OpCode_CALL(char **strs);
OpCode_t *OpCode_MOV_r_li(char **strs);
OpCode_t *OpCode_CMP_r_r(char **strs);
//OpCode_t *OpCode_CMP_r_li(char **strs);
OpCode_t *OpCode_JE_i(char **strs);
OpCode_t *OpCode_JNE_i(char **strs);
OpCode_t *OpCode_JGE_i(char **strs);
OpCode_t *OpCode_JBE_i(char **strs);
OpCode_t *OpCode_JG_i(char **strs);
OpCode_t *OpCode_JB_i(char **strs);
OpCode_t *OpCode_PUSH_r(char **strs);
OpCode_t *OpCode_PUSH_i(char **strs);
OpCode_t *OpCode_POP_r(char **strs);
OpCode_t *OpCode_SYSCALL(char **strs);
OpCode_t *OpCode_INT(char **strs);
OpCode_t *OpCode_IMUL_r_r(char **strs);
OpCode_t *OpCode_IDIV_r(char **strs);
OpCode_t *OpCode_binary_4(char **strs);
OpCode_t *OpCode_binary_2(char **strs);
OpCode_t *OpCode_binary_1(char **strs);

#endif