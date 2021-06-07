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


#define GOODENOUGH  ("\n\
                       ##&#&%(%%\n\
                     ####&(,    ..                                   .,(\n\
                    ####(,  .   /#(*                            / .,./\n\
                    ####*/**,#/*./,  .                          . ,#\n\
                     ####/(/%%/*/,  #,,                     .  .*/\n\
                      &##&***,,.,(/&*,%######               ,*//(,\n\
                        &%#%****,(#/&(&######### / ,        .*##.\n\
                        .##/####&###### .*#######,    .     ,,*.\n\
                      ######,* *.&##&##.,    #####%  . */  ./*(/*.\n\
                     ,######,*  %* *,&&%%     ..###* .  ,  *(,*(%\n\
,,                    ######   .     .*#&% .     *##/..*/,/*,(%,\n\
&%#(%(%(*         .   #######   . ,   .(##%#     ./##%#(*/(,(\n\
   #&#%(#(/,,,.(*/.,,/######%  . .      (%##% .     #\n\
        &&&%#/#,.   ,,######%   .   ,   #&%###    .  #\n\
       *#&%%##&* , * ,######.,     .   .(###%%#      (##\n\
         .(&%/..../   ######,.    . ,    &##((##  .. ,,#%\n\
        ,.     * (./ *######* . .       , ####(#(   . .,#\n\
      ., , //*, (%/.*.######.* , . , . ,,#&####### , . .#*\n\
")

#define GOODOSS  ("\n\
                                       ////****///////////////*///                                  \n\
                                  /////////////////////////////////////                             \n\
                              //////////////////*///////////////*//////////.                        \n\
                          ./////*///////////////////////////////*////////////**                     \n\
                       .*///////*//////*#&&@@@@&&%//////////////*///////*////////.                  \n\
                     *//////////*//////*****/((&@@&(///////////////////////////////*                \n\
                   .////////////*///////,**/(%(#@@@(/////////////////////////////////*              \n\
                  //////////////////////,*((*///&(%///////////////////////////////////*             \n\
                ////////////////////*///**##///((//#&@&&&&&&&@//////////////////////////*           \n\
               /////////////////*////////*//((((((/.%@@@&&&&&&&&//////////////////////////          \n\
              //////////////////*//////////* .//#%,.,*&&&&&&&&&&%#*/////////////*//////////         \n\
             *//////////////////*////////####. .###.,..,%&%%&&&%&%%%(*//////////////////////        \n\
             ///////////////////*/////((##%##% ..(%(.....*%%%#&&&&&&%%%/////////////////////,       \n\
            ,/////////////////////*/((###%%####./#%%,,....,#%%%&&%%&@@@&#///////////////////,       \n\
            *///*//////////////////(#####%&%%%#%*#%&/,.....*%%%%%&&%@@@@&%///////////////////       \n\
            ///////////////////////(((%##%&%%%##&%%%(,,.....%&@%&&%&&#@@&%%(/////////////////       \n\
            */*/////////////////*///###%%#%&%@%#&&%&(,,,.....%%&@@&&&&/&@%#((/*/////////*////       \n\
            ///////////////////////*(##(%@#&@&@%&@&&/,,,/....,/%@@@@@&@/(///*#///////////////       \n\
            ////*///////////////////(##%#@@%&@@&&@&&(,,,*......*&@@@@@&//(((////*////////////       \n\
             //////////////////##////&%%&&&&%@@%&@&&%/*****...#@@@@@@@@@@@%&#///////////////,       \n\
             **//////////////////@&@((##%&@@@@@&@@%%&*,,,(#&@@@@@&@@@@@@@@&%&%//////////*///        \n\
              //////////////////,,(###&&&&@@@@&@@@@@@@@@@@@&@@@@@&&@@@@@@@@%%%%////////////         \n\
               /////////////////**/(((#&&&@@@@@@@@@@@@@@@@@@&&&@&&&&@@@@@@@@&%%#//////////          \n\
                //////////////////////%%%/#%@&@@@@@@@@@@&@&&&&&&&&&&&&/((((#((/*/////////           \n\
                  //////*///*////////&&///#%&@@@@@@@@@@&&&&@@@&&&&&&&&&/////////*//////             \n\
                   *////////////*/////////#&@@@@@@&@@@&%*(@@@@&&&&&%%%&%//////////////              \n\
                     ////////////////////#&@@@@@@@&&&#//////&&@@&&&&&&&&%///////////                \n\
                       .////////*////////%@@@@&&@@&&////////*/&&@&&&&&&&&%///////*                  \n\
                          */////*///////%@%%&&&@@&#/////////////%&&&&&&&&&(///*                     \n\
                             *//////////#%%%&&@&&(///////////////#%&%&&&&&&#(                       \n\
  /#########(    _=====_         ,/////%%%%&&&&&//*////////////////%%%%%&&&&%(                      \n\
      .##########################((.((##%&&&&&#/////////////////////(%%%%&&&&%/                     \n\
       (###############################%%&&&&/(///////////(////////////#&&&&&&(                     \n\
        #############################%##&&&####((////////////////(///(((%&%&&&%*                    \n\
            (##########################%&&###########((///////((/////(//(%%&&&%/                    \n\
           /##########################%&&######(((//(///////(/(/*        /&&&&%/                    \n\
                      *##########/###%&&##############(((/.         .._-#,%%&%#/#\\,,                \n\
                        *########/##%&&%///////****/(###*,,,.     **,.     #&&#|    |\\              \n\
                        .#######((#%%&&.            .#&#%%*,,*\\,,,     _-  *%%%/      ,/            \n\
                ___ _ ,///(#####&%#%%&#   #(//((//*,... ..,*#&&@@@@&&&&%/   %%@@.     ./            \n\
           ,//,              ,,#@@@@.\\                          ..,*,\\=-_   %@@#%* ,&(//=-/'*\\      \n\
        ,*         /%&&%*    ,#*#@&/  ,.    _--=##%&&&&#         &%(*,..,/(#&#@%#%            *     \n\
       /         .(///((//(@  *%%%(    .////     ..           .%((((//,.  .&.  .    .-=&       *    \n\
      /*          .(((////(//&         /               _\\*%&#/////.          .%(/#*/   /,      ()   \n\
       #/            *//((//(/         */,((##%%%%#((./       *//               .(      #     -//   \n\
        *%%                         ,%(          /*             .&.,%((%(        \\,(    # _-//      \n\
           /(&&(.             ,%&%(/            (%     #,          ,(    (&,     //     \\#/         \n\
                 ...(((((.../                     (&&#. *#%           /    ((&._#/                  \n\
                                                          ,(&*          ./                          \n\
                                                             *#% *%&%(/*                            \n\
                                                                                                    \n\
")




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
    //char *args; // 1 = r, 2 = adrr, 0 = end 
    OpCode_t *(*generate)(char **args);
} instruction_t;

instruction_t *generateInstruction(char *name, OpCode_t *(*func)(char **)) {
    instruction_t *inst = malloc(sizeof(instruction_t));
    inst->name = strToWords(name, ' ');
    inst->generate = func;
    return inst;
}

OpCode_t *compile_ADD_r_r(char **strs) {
    int r1 = strtol(strs[1]+1, 0, 0);
    int r2 = strtol(strs[2]+1, 0, 0);
    unsigned char thisOpcode[] = {
        0x3, // add
        0xc0 | CHAR_TO_LEFT_REGISTER(r1) | CHAR_TO_RIGHT_REGISTER(r2)
    };
    OpCode_t *opcode = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return opcode;
}

OpCode_t *compile_JMP(char **strs) {
    int adrr = strtol(strs[1]+1, 0, 0);
    unsigned char thisOpcode[] = {
        0xe9, 
        ADDRESS_TO_4CHARS(adrr)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *compile_RET(char **strs) {
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
        0xc0 | CHAR_TO_LEFT_REGISTER(r1) | CHAR_TO_RIGHT_REGISTER(r2)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_mov_r_mem(char **strs) {
    int Rdest = strtol(strs[1]+1, 0, 0);
    int Radrr = strtol(strs[2]+1, 0, 0);
    int offset = strtol(strs[3]+1, 0, 0);
    
    unsigned char thisOpcode[] = {
        0x48, 0x8b,
        CHAR_TO_LEFT_REGISTER(Rdest) | CHAR_TO_RIGHT_REGISTER(Radrr), // mov r to r
        ADDRESS_TO_4CHARS(offset)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

OpCode_t *OpCode_mov_mem_r(char **strs) {
    int Rsrc = strtol(strs[1]+1, 0, 0);
    int Radrr = strtol(strs[2]+1, 0, 0);
    int offset = strtol(strs[3]+1, 0, 0);

    unsigned char thisOpcode[] = {
        0x48 | OP_D, 0x8b,
        CHAR_TO_LEFT_REGISTER(Rsrc) | CHAR_TO_RIGHT_REGISTER(Radrr), // mov r to r
        ADDRESS_TO_4CHARS(offset)
    };
    OpCode_t *op = OpCode_init(sizeof(thisOpcode), thisOpcode);
    return op;
}

instruction_t **instructionsSet = 0;
#define PUSHBACK(lld, data) lld_insert(lld, lld_len(lld), data)
void generateInstructionsSet() {
    lld_t *lld = lld_init();

    PUSHBACK(lld, generateInstruction("add r r", &compile_ADD_r_r));
    PUSHBACK(lld, generateInstruction("jmp 0", &compile_JMP));
    PUSHBACK(lld, generateInstruction("ret", &compile_RET));
    PUSHBACK(lld, generateInstruction("mov r r", &OpCode_MOV_r_r));
    PUSHBACK(lld, generateInstruction("mov r r 0", &OpCode_mov_r_mem));
    PUSHBACK(lld, generateInstruction("mov r r 0", &OpCode_mov_mem_r));

    instructionsSet = (instruction_t **)lld_lld_to_tab(lld);
    lld_free(lld);
}

void freeInstructionsSet() {
    for (int i = 0; instructionsSet[i]; i++) {
        for (int j = 0; instructionsSet[i]->name[j]; j++)
            free(instructionsSet[i]->name[j]);
        free(instructionsSet[i]->name);
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
    // "add", "i0",
    // "jmp", "i1",
    // "ret", "i2"
};

int main() {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    //strtol(str, 0, 0)
    
    // init instruction set
    generateInstructionsSet();
    printInstructionsSet();

    int error_count = 0;

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

    // apply strToWords on each line and do aliases 
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

    lld_t *lld_opcodes = lld_init();

    // compile the code
    i = 0;
    for (lld_t *mv = file->next; mv; mv = mv->next, i++) {
        printf("%4i| ", i);
        char **words = mv->data;
        for (int i = 0; words[i]; i++) {
            printf(" %s", words[i]);
        }
        // found the matching instruction
        int instruction = -1;
        for (int i = 0; instructionsSet[i]; i++) {
            if (!strcmp(words[0], instructionsSet[i]->name[0])) {
                for (int j = 1; instructionsSet[i]->name[j]; j++){
                    if (!words[j] || instructionsSet[i]->name[j][0] != words[j][0]) {
                        goto next;
                    }
                }
                instruction = i;
            }
            next:
            (void)0;
        }
        if (instruction == -1) {
            printf(" <= !!!COMPILATION ERROR LINE INGORED!!!");
            error_count++;
        } else {
            // compile an instruction
            lld_insert(lld_opcodes, lld_len(lld_opcodes), 
                instructionsSet[instruction]->generate(words)
            );
        }
        printf("\n");
    }

    OpCode_t **opcodes = (OpCode_t **)lld_lld_to_tab(lld_opcodes);

    lld_free(lld_opcodes);

    uint prog_size = 0;
    for (int i = 0; opcodes[i]; i++) {
        prog_size += opcodes[i]->c_size;
    }

    printf("programe lengh = %i\n", prog_size);

    // puts the opcodes togethers
    char *binary = malloc(prog_size);
    uint p = 0;
    for (int i = 0; opcodes[i]; i++) {
        memcpy(binary+p, opcodes[i]->c, opcodes[i]->c_size);
        p += opcodes[i]->c_size;
    }

    // free opcode array
    for (int i = 0; opcodes[i]; i++)
        free(opcodes[i]);
    free(opcodes);

    printf("Compilation: DONE\n");
        printf("the programe has compiled with %i errors\n", error_count);
    if (error_count) {
        printf("%s", GOODENOUGH);
    } else {
        printf("%s", GOODOSS);
    }   

    // free array
    for (lld_t *mv = file->next; mv; mv = mv->next, i++) {
        char **words = mv->data;
        for (int j = 0; words[j] != 0; j++)
            free(words[j]);
        free(words);
    }
    lld_free(file);

    FILE *dest = fopen("main.oss", "wb");
    fwrite(binary, prog_size, 1, dest);
    fclose(dest);


    int (*func)(int) = (void *)binary;
    int ret = func(123);
    printf("%i\n", ret);

    free(binary);

    freeInstructionsSet();
    return 0;
}