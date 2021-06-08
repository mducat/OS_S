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

//Booooa %4i Erreur, c'est pas si mal !   38




#define GOODENOUGH  "\%s%3i Warning?                                            \n\
                                          C'est pas si mal!                                         \n\n", "\n\
                                     #@@&&&@/                                                       \n\
                                   @@@@%%%/*   ,                                                    \n\
                                 @@@@@&%(,       .@                                      .   .*#(   \n\
                                @@@@@%/.       .#*(((                                  ,  .. *#     \n\
                                 @@@%/*/,*..**,,(&*,  &                                 .  *%       \n\
                                 @@@@&#//@**%(,(.  . . ,                           .    *((         \n\
                                  @@@@@**/(#*,*,/,,,/%/*% *@                         .,//           \n\
                                    @@@@/***,,..**(&***/@@@@@@@@@                  //(((/           \n\
                                     @%&@@/****,,%#/(#(&@@@@@@@@@@@  . .,*         , #(@.           \n\
                                      %@&,%@@#(((#@@@@@@.  (@@@@@@@@@   *  .       ,,/ (.,          \n\
                                    @@@@@#*  .((%&@&&%/&(,    /@@@@@@@,   .*  &   * (*,*  .         \n\
                                  @@@@@@@@ , *  .  &@&%%& ..    ,@@@@@@.,  (.,*  *//#**#@,          \n\
                                 .@@@@@@@@@ ,/ &.    .#@&@%        %@@@@   ,,/., .*(,//&/           \n\
       %                       .  ,@@@@@@@@ . ,    *. .*@&@%         @@@%...,.**/ (**%*             \n\
      *%&&%%#((***                 @@@@@@@@  . ,  .   ../@%@%@   .  . &@@&##.,,(#*(#                \n\
         @%%###(#%(/*,%  ,*.*..,. #@@@@@@@@,  .  . . , ..(#%&%@       ./@                           \n\
             &#(#(/(/****/,.,/,*.  @@@@@@@@ ,  . , ,  .  ,@%@%#(/,      ,@                          \n\
                 #&&&&&#(#.,/(%./.#@@@@@@@@*      .  . , .#@#@#@##, ..  ./@#                        \n\
                 @@&%%%##@(&    . ,@@@@@@@@     .     , . (@@#@#@#( .  . .%@@                       \n\
                  .&@%((, & .  * * @@@@@@@@    * ,         &#@(%(##  .,. . *@@                      \n\
                   ,.  , ,*.    ,. @@@@@@@@       .  ,     ,%(###((#. .  ., .@@                     \n\
                 . ...    (/,  /.(.@@@@@@@@*  , ,    .  . ,/#@##((&#&  ,   . @@                     \n\
               /..&,.,(#.& /**( ,./@@@@@@@@.*  , *  . . ..,/@((#(##@#&  .,.   @                     \n\
                                                                                                    \n\
                                            "

//72

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
           ,//,              ,,#@@@@.\\                          ..,*,\\=-_   %@@#%*-,&(//=-/'*\\      \n\
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
                                              0 Warning                                             \n\
                                    MEILLEUR PROGRAMMEUR FRANCAI                                    \n\
                                                                                                    \n\
")

char *my_putnbr_base(unsigned long int nbr, char *base)
{
    int len = strlen(base);
    int i;
    static char to_print[65];
    static char to_print2[65];
    //nbr < 0 ? nbr *= -1 : 0;
    for (int y = 0; y < 65; to_print[y] = 0, to_print2[y] = 0, y++);
    for (len = 0; base[len]; len++);
    for (i = 0; nbr != 0; i++){
        to_print[i] = base[nbr%len];
        nbr /= len;
    }
    int a = 0;
    for (i = i-1; i >= 0; i--, a++)
        to_print2[a] = to_print[i];
    to_print2[0] == 0 ? to_print2[0] = base[0] : 0;
    return (to_print2);
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
    int c_size;
    OpCode_t *(*generate)(char **args);
} instruction_t;

char *generateInstructionChars[32] = {
    "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"
};


instruction_t *generateInstruction(char *name, OpCode_t *(*func)(char **)) {
    instruction_t *inst = malloc(sizeof(instruction_t));
    inst->name = strToWords(name, ' ');
    inst->generate = func;
    OpCode_t *op = func(generateInstructionChars);
    inst->c_size = op->c_size;
    free(op);
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

OpCode_t *compile_JMP_relativ(char **strs) {
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

instruction_t **instructionsSet = 0;
#define PUSHBACK(lld, data) lld_insert(lld, lld_len(lld), data)
void generateInstructionsSet() {
    lld_t *lld = lld_init();

    PUSHBACK(lld, generateInstruction("add r r", &compile_ADD_r_r));
    PUSHBACK(lld, generateInstruction("jmp _", &compile_JMP_relativ));
    PUSHBACK(lld, generateInstruction("ret", &compile_RET));
    PUSHBACK(lld, generateInstruction("mov r r", &OpCode_MOV_r_r));
    PUSHBACK(lld, generateInstruction("mov r r _", &OpCode_MOV_r_mem));
    PUSHBACK(lld, generateInstruction("mov r _ r", &OpCode_MOV_mem_r));
    PUSHBACK(lld, generateInstruction("call _", &OpCode_CALL));
    PUSHBACK(lld, generateInstruction("mov r _", &OpCode_MOV_r_li));

    instructionsSet = (instruction_t **)lld_lld_to_tab(lld);
    lld_free(lld);
}


typedef struct balise {
    int adrr;
    int line;
    char *name;
} balise_t;

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
    int line_count = 0;
    printf("\n");
    for (lld_t *mv = file->next; mv; mv = mv->next, line_count++) {
        // rm double spaces
        rmDoubledCHar(mv->data, ' ');
        printf("%4i|  %s\n", line_count, (char *)mv->data);

        char **words = strToWords(mv->data, ' ');
        
        // iter through words
        for (int i = 0; words[i]; i++) {
            // apply aliases
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
    
    printf("############################\n");

    lld_t *lld_balises = lld_init();

    // compile the code
    line_count = 0;
    int current_adr = 0;
    for (lld_t *mv = file->next; mv; mv = mv->next, line_count++) {
        printf("%4i| ", line_count);
        char **words = mv->data;

        // find balises
        for (int i = 0; words[i]; i++) {
            if (words[i][0] == '!') {
                balise_t *balise = malloc(sizeof(balise_t));
                balise->name = strdup(words[i]+1);
                balise->adrr = current_adr;
                balise->line = line_count;
                lld_insert(lld_balises, lld_len(lld_balises), balise);
                printf("\t'%s'", balise->name);
                //remove balise
                free(words[i]);
                for (int j = i; words[j]; j++) {
                    words[j] = words[j+1];
                }
            }
            // print line without balises
            if (words[i])
                printf(" %s", words[i]);
        }

        // found the matching instruction
        int instruction = -1;
        for (int i = 0; instructionsSet[i]; i++) {
            if (!strcmp(words[0], instructionsSet[i]->name[0])) {
                int j = 1;
                for (; instructionsSet[i]->name[j]; j++){
                    if (!words[j] || instructionsSet[i]->name[j][0] != words[j][0]) {
                        goto next;
                    }
                }
                if (instructionsSet[i]->name[j] == words[j]){
                    instruction = i;
                } else {
                    goto next;
                }
            }
            next:
            (void)0;
        }

        if (instruction == -1) {
            printf("\t<= C KOI SA?""?!");
            error_count++;
        } else {
            current_adr += instructionsSet[instruction]->c_size;
        }

        // change instruction name to instruction id
        free(words[0]);
        words[0] = (void *)(long int)instruction;
        printf("\n");
    }


        // replace balise with relative range

    balise_t **balises = (balise_t **)lld_lld_to_tab(lld_balises);
    lld_free(lld_balises);


    printf("############################\n");


    // compile instructions
    lld_t *lld_opcodes = lld_init();
    line_count = 0;
    current_adr = 0;
    for (lld_t *mv = file->next; mv; mv = mv->next, line_count++) {
        printf("%4i| ", line_count);
        char **words = mv->data;
        if ((long int)words[0] != -1)
            printf(" %s", instructionsSet[(long int)words[0]]->name[0]);
        else 
            printf(" UNKNOWN");
        for (int i = 1; words[i]; i++) {
            printf(" %s", words[i]);
        }
        printf("\n");
        // compile an instruction
        long int instruction = (long int)words[0];

        if (instruction != -1){
            current_adr += instructionsSet[instruction]->c_size;
            // reaplace by balise values
            for (int i = 1; words[i]; i++) {
                if (words[i][1] == ':'){
                    for (int j = 0; balises[j]; j++) {
                        if (!strcmp(words[i]+2, balises[j]->name)) {
                            // replace balise
                            char *old = words[i];
                            words[i] = malloc(64);
                            words[i][0] = old[0];
                            memcpy(words[i]+1, "0x", 2);
                            char *base = my_putnbr_base(balises[j]->adrr-current_adr, "0123456789ABCDEF");
                            memcpy(words[i]+3, base, strlen(base)+1);
                            free(old);
                            goto instend;
                        }
                    }
                    printf("BALISE !%s NOT FOUND\n", words[i]+2);
                }
                instend:
                (void)0;
            }
            lld_insert(lld_opcodes, lld_len(lld_opcodes), 
                instructionsSet[instruction]->generate(words)
            );
        }
    }
    printf("############################\n");

    line_count = 0;
    for (lld_t *mv = file->next; mv; mv = mv->next, line_count++) {
        printf("%4i| ", line_count);
        char **words = mv->data;
        if ((long int)words[0] != -1)
            printf(" %s", instructionsSet[(long int)words[0]]->name[0]);
        else 
            printf(" UNKNOWN");
        for (int i = 1; words[i]; i++) {
            printf(" %s", words[i]);
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
        printf(GOODENOUGH, error_count);
    } else {
        printf("%s", GOODOSS);
    }   

    // free array
    line_count = 0;
    for (lld_t *mv = file->next; mv; mv = mv->next, line_count++) {
        char **words = mv->data;
        for (int j = 1; words[j] != 0; j++)
            free(words[j]);
        free(words);
    }
    lld_free(file);

    //free balises
    for (int i = 0; balises[i] != 0; i++) {
        free(balises[i]->name);
        free(balises[i]);
    }
    free(balises);

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