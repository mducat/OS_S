 //http://www.c-jump.com/CIS77/CPU/x86/lecture.html#X77_0120_encoding_add

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



int main() 
{
    char code[] = {
        0b00000001, 0b11000001,
    };

    

    write(1, code, sizeof(code));
}