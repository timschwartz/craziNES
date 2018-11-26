#ifndef _CRAZINES_OPCODES_H
#define _CRAZINES_OPCODES_H

#define BRK        0x00

#define NOP_zp_04  0x04
#define ORA_zp     0x05

#define SLO_zp     0x07
#define PHP        0x08
#define ORA_imm    0x09

#define NOP_abs_0C 0x0C

#define SLO_abs    0x0F
#define BPL        0x10

#define NOP_zp_14  0x14

#define CLC        0x18

#define NOP_1A     0x1A

#define NOP_abs_1C 0x1C

#define JSR        0x20
#define AND_ind_x  0x21

#define BIT_zp     0x24

#define PLP        0x28
#define AND_imm    0x29

#define BIT_abs    0x2C

#define BMI        0x30

#define NOP_zp_34  0x34

#define NOP_3A     0x3A

#define RLA_zp_x   0x37
#define SEC        0x38

#define RTI        0x40

#define NOP_zp_44  0x44

#define PHA        0x48
#define EOR_imm    0x49

#define LSR_acc    0x4A

#define JMP_abs    0x4C

#define BVC        0x50

#define NOP_zp_54  0x54

#define CLI        0x58

#define NOP_5A     0x5A

#define NOP_abs_5C 0x5C

#define RTS        0x60

#define NOP_zp_64  0x64

#define PLA        0x68
#define ADC_imm    0x69

#define BVS        0x70

#define NOP_zp_74  0x74

#define SEI        0x78

#define NOP_7A     0x7A

#define NOP_abs_7C 0x7C

#define NOP_imm_80 0x80
#define STA_ind_x  0x81

#define STY_zp_x   0x84
#define STA_zp     0x85
#define STX_zp     0x86

#define DEY        0x88

#define STY_abs    0x8C
#define STA_abs    0x8D

#define STX_abs    0x8E

#define BCC        0x90
#define STA_ind_y  0x91

#define TXS        0x9A

#define LDY_imm    0xA0

#define LDX_imm    0xA2

#define LDA_zp     0xA5
#define LDX_zp     0xA6

#define LDA_imm    0xA9

#define LDA_abs    0xAD

#define BCS        0xB0

#define LDY_zp_x   0xB4
#define LDA_zp_x   0xB5

#define CLV        0xB8

#define LDA_abs_x  0xBD

#define CPY_imm    0xC0
#define CMP_ind_x  0xC1
#define NOP_imm_C2 0xC2

#define DEC_zp     0xC6

#define INY        0xC8
#define CMP_imm    0xC9

#define DEX        0xCA

#define DEC_abs    0xCE

#define BNE        0xD0

#define NOP_zp_D4  0xD4
#define CMP_zp_x   0xD5

#define CLD        0xD8

#define NOP_DA     0xDA

#define NOP_abs_DC 0xDC

#define CPX_imm    0xE0

#define INC_zp     0xE6

#define INX        0xE8
#define SBC_imm    0xE9

#define NOP_EA     0xEA

#define BEQ        0xF0

#define NOP_zp_F4  0xF4

#define NOP_FA     0xFA

#define NOP_abs_FC 0xFC

#define SED        0xF8

#endif
