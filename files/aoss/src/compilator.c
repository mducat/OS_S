#include <lld.h>
#include <oss.h>

#include "compilator.h"
#include "instruction.h"
#include <lld.h>
#include <my.h>


void generateInstructionsSet();
void printInstructionsSet();
void freeInstructionsSet();
void rmDoubledCHar(char *str, char c);

typedef unsigned int uint;

instruction_t **instructionsSet = 0;
#define PUSHBACK(lld, data) lld_insert(lld, lld_len(lld), data)

#define printf(str, ...) {char buf[] = str; printf(buf __VA_OPT__(,) __VA_ARGS__);}

typedef struct balise {
    int adrr;
    int line;
    char *name;
} balise_t;

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

int main(int ac, char **av) {
    if (ac != 2)
        return 1;
    // init instruction set
    generateInstructionsSet();
    printInstructionsSet();

    int error_count = 0;

    //FILE *src = fopen("main.coss", "r+");
    //FILE *src = fopen("main.coss", "r+");

    file_t *src = open(av[1]);
    lld_t *file = lld_init();
    char *line = 0;
    char **file_split = strToWords(src->content, '\n');
    /*size_t line_len = 0;
    int line_len2 = 0;
    while ((line_len2 = getline(&line, &line_len, src)) > 0){
        if (!line)
            continue;
        if (line[line_len2-1] == '\n') 
            line[line_len2-1] = 0;
        lld_insert(file, lld_len(file), line);
        line = 0;
    }*/
    for (int i = 0; file_split[i]; i++) {

    }
    
    free(line);
    //fclose(src);

  


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
            for (uint j = 0; j < sizeof(aliases)/sizeof(*aliases); j += 2) {
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
        if ((long int)words[0] != -1) {
            printf(" %s", instructionsSet[(long int)words[0]]->name[0]);
        } else {
            printf(" UNKNOWN");
        }

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
                            char *base = my_putnbr_base_str(balises[j]->adrr-current_adr, "0123456789ABCDEF");
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
        if ((long int)words[0] != -1) {
            printf(" %s", instructionsSet[(long int)words[0]]->name[0]);
        } else {
            printf(" UNKNOWN");
        }
        for (int i = 1; words[i]; i++) {
            printf(" %s", words[i]);
        }
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
    //char *binary = malloc(prog_size);
    void *binary = malloc(prog_size);
    //int pagesize = sysconf(_SC_PAGE_SIZE);
    /*void *binary = 0;
    if (posix_memalign(&binary, pagesize, pagesize*4)) {
        perror("posix_memalign ");
    }
    if (mprotect(binary-(long int)binary%pagesize, (prog_size/pagesize+2)*(pagesize), PROT_EXEC | PROT_READ | PROT_WRITE)) {
        perror("mprotect ");
    }*/

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
        printf(GOODENOUGH1, GOODENOUGH2, error_count);
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

    //FILE *dest = fopen("main.oss", "wb");
    char destName[] = "main.oss";
    file_t *dest = open(destName);
    write_file(dest->name, binary, prog_size);

    //fwrite(binary, prog_size, 1, dest);
    
    //fclose(dest);

    //void *(*func)(char *) = (void *)binary;
    //char *ret = func(str);
    //printf("%p %s\n", ret, ret);

    free(binary);
    freeInstructionsSet();
    return 0;
}

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
    while (str[i] == c) 
        str[i--] = 0;
    str[i+1] = 0;
}

void generateInstructionsSet() {
    lld_t *lld = lld_init();

        {
            char str[] = "add r r"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_ADD_r_r));
        }
        {
            char str[] = "jmp _"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_JMP_relativ));
        }
        {
            char str[] = "ret"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_RET));
        }
        {
            char str[] = "mov r r"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_MOV_r_r));
        }
        {
            char str[] = "mov r r _"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_MOV_r_mem));
        }
        {
            char str[] = "mov r _ r"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_MOV_mem_r));
        }
        {
            char str[] = "call _"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_CALL));
        }
        {
            char str[] = "mov r _"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_MOV_r_li));
        }
        {
            char str[] = "cmp r r"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_CMP_r_r));
        }
        {
            char str[] = "je _"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_JE_i));
        }
        {
            char str[] = "jne _"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_JNE_i));
        }
        {
            char str[] = "jge _"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_JGE_i));
        }
        {
            char str[] = "jbe _"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_JBE_i));
        }
        {
            char str[] = "jg _"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_JG_i));
        }
        {
            char str[] = "jb _"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_JB_i));
        }
        {
            char str[] = "push r"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_PUSH_r));
        }
        {
            char str[] = "push _"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_PUSH_i));
        }
        {
            char str[] = "pop r"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_POP_r));
        }
        {
            char str[] = "syscall"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_SYSCALL));
        }
        {
            char str[] = "int _"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_INT));
        }
        {
            char str[] = "imul r r"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_IMUL_r_r));
        }
        {
            char str[] = "idiv r"; 
            PUSHBACK(lld, generateInstruction(str, &OpCode_IDIV_r));
        }



    //PUSHBACK(lld, generateInstruction("cmp r _", &OpCode_CMP_r_li));


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
