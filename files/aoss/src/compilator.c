#include <my.h>

#include "compilator.h"
#include "instructions.h"

#ifdef __OSS__
#include <lld.h>
#include <oss.h>
#include <string.h>


#else

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#endif



typedef unsigned int uint;

void rmDoubledCHar(char *str, char c) {
    int p = 0;
    int i = 0;
    while (str[p] == c)
        p++;
    for (; str[p]; p++) {
        if (str[p] == c && str[p+1] == c)
            continue;
        str[i++] = str[p];
    }
    i--;
    if (i < 0)
        i = 0;
    while (str[i] == c) 
        str[i--] = 0;
    str[i+1] = 0;
}

instruction_t **instructionsSet = 0;
#define PUSHBACK(lld, data) lld_insert(lld, lld_len(lld), data)
void generateInstructionsSet() {
    lld_t *lld = lld_init();

    PUSHBACK(lld, generateInstruction("add r r", &OpCode_ADD_r_r));
    PUSHBACK(lld, generateInstruction("jmp _", &OpCode_JMP_relativ));
    PUSHBACK(lld, generateInstruction("ret", &OpCode_RET));
    PUSHBACK(lld, generateInstruction("mov r r", &OpCode_MOV_r_r));
    PUSHBACK(lld, generateInstruction("mov r r _", &OpCode_MOV_r_mem));
    PUSHBACK(lld, generateInstruction("mov r _ r", &OpCode_MOV_mem_r));
    PUSHBACK(lld, generateInstruction("call _", &OpCode_CALL));
    PUSHBACK(lld, generateInstruction("mov r _", &OpCode_MOV_r_li));
    PUSHBACK(lld, generateInstruction("cmp r r", &OpCode_CMP_r_r));
    //PUSHBACK(lld, generateInstruction("cmp r _", &OpCode_CMP_r_li));
    PUSHBACK(lld, generateInstruction("je _", &OpCode_JE_i));
    PUSHBACK(lld, generateInstruction("jne _", &OpCode_JNE_i));
    PUSHBACK(lld, generateInstruction("jge _", &OpCode_JGE_i));
    PUSHBACK(lld, generateInstruction("jbe _", &OpCode_JBE_i));
    PUSHBACK(lld, generateInstruction("jg _", &OpCode_JG_i));
    PUSHBACK(lld, generateInstruction("jb _", &OpCode_JB_i));
    PUSHBACK(lld, generateInstruction("push r", &OpCode_PUSH_r));
    PUSHBACK(lld, generateInstruction("push _", &OpCode_PUSH_i));
    PUSHBACK(lld, generateInstruction("pop r", &OpCode_POP_r));
    PUSHBACK(lld, generateInstruction("syscall", &OpCode_SYSCALL));
    PUSHBACK(lld, generateInstruction("int _", &OpCode_INT));
    PUSHBACK(lld, generateInstruction("imul r r", &OpCode_IMUL_r_r));
    //PUSHBACK(lld, generateInstruction("idiv r", &OpCode_IDIV_r));
    PUSHBACK(lld, generateInstruction("b4 _", &OpCode_binary_4));
    PUSHBACK(lld, generateInstruction("b2 _", &OpCode_binary_2));
    PUSHBACK(lld, generateInstruction("b1 _", &OpCode_binary_1));
    PUSHBACK(lld, generateInstruction("sub r r", &OpCode_sub_r_r));
    

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

char *my_putnbr_base_str_int(unsigned int nbr, char *base){
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

int main(int ac, char **av) {

    if (ac != 2){
        printf("need a file at first arg\n");
        return 1;
    }
    int error_count = 0;

    #ifdef __OSS__
    file_t *src = open(av[1]);
    
    #else
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    FILE *src = fopen(av[1], "r+");
    
    #endif
    if (!src){
        printf("cannot open: %s\n", av[1]);
        return 1;
    }
    lld_t *file = lld_init();

    #ifdef __OSS__
    char *fileBuf = malloc(src->size+1);
    memcpy(fileBuf, src->content, src->size);
    fileBuf[src->size] = 0;

    char **file_split = strToWords(fileBuf, '\n');
    free(fileBuf);

        for (int i = 0; file_split[i]; i++) {
        lld_insert(file, lld_len(file), file_split[i]);
    }
    free(file_split);

    #else
    char *line = 0;
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
    
    #endif

    // init instruction set
    generateInstructionsSet();
    printInstructionsSet();


    // apply strToWords on each line and do aliases 
    int aliasesNb = 16;
    char aliases[] = "rax\0r0\0\0rcx\0r1\0\0rdx\0r2\0\0rbx\0r3\0\0rsp\0r4\0\0rbp\0r5\0\0rsi\0r6\0\0rdi\0r7\0\0";
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
            for (int j = 0; j < aliasesNb; j += 2) {
                if (!strcmp(words[i], &aliases[4*j])) {
                    free(words[i]);
                    words[i] = strdup(&aliases[4*(j+1)]);
                    break;
                }
            }
        }
        free(mv->data);
        mv->data = words;
    }
    
    #ifdef __OSS__
    refresh();
    #endif

    printf("1 ############################\n");

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
                printf("    '%s'", balise->name);
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
            if (words[0] && !strcmp(words[0], instructionsSet[i]->name[0])) {
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
        if (instruction == -1)
            for (int i = 0; words[i]; i++)
                free(words[i]);
        else
            free(words[0]);
        words[0] = (void *)(long int)instruction;
        printf("\n");
    }


        // replace balise with relative range

    balise_t **balises = (balise_t **)lld_lld_to_tab(lld_balises);
    lld_free(lld_balises);

    #ifdef __OSS__
    refresh();
    #endif

    printf("2 ############################\n");


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
        if ((long int)words[0] != -1)
            for (int i = 1; words[i]; i++)
                printf(" %s", words[i]);
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
                            char *base = my_putnbr_base_str_int((balises[j]->adrr-current_adr), "0123456789ABCDEF");
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

    #ifdef __OSS__
    refresh();
    #endif

    printf("3 ############################\n");

    line_count = 0;
    for (lld_t *mv = file->next; mv; mv = mv->next, line_count++) {
        printf("%4i| ", line_count);
        char **words = mv->data;
        if ((long int)words[0] != -1)
            printf(" %s", instructionsSet[(long int)words[0]]->name[0]);
        else 
            printf(" UNKNOWN");
        if ((long int)words[0] != -1)
            for (int i = 1; words[i]; i++)
                printf(" %s", words[i]);
        printf("\n");
    }


    OpCode_t **opcodes = (OpCode_t **)lld_lld_to_tab(lld_opcodes);

    lld_free(lld_opcodes);

    int prog_size = 0;
    for (int i = 0; opcodes[i]; i++) {
        prog_size += opcodes[i]->c_size;
    }

    printf("programe lengh = %i\n", prog_size);

    // puts the opcodes togethers
    void *binary = malloc(prog_size+3)+3;

    int p = 0;
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
        if ((long int)words[0] != -1)
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

    int file_name_len = 0;
    for (; av[1][file_name_len] != '.' && av[1][file_name_len]; file_name_len++);
    char *destName = malloc(file_name_len+5);
    memcpy(destName, av[1], file_name_len);
    memcpy(destName+file_name_len, ".oss", 5);
    char OSS[] = "OSS";
    memcpy(((char *)binary)-3, OSS, 3);

    #ifdef __OSS__
    write_file(destName, ((char *)binary)-3, prog_size+3);
    #else
    FILE *dest = fopen(destName, "wb");
    fwrite(binary, prog_size, 1, dest);
    fclose(dest);
    #endif

    free(destName);
    

    free(binary-3);
    freeInstructionsSet();
    return 0;
}