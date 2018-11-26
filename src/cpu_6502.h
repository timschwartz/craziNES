#ifndef _CRAZINES_CPU_6502_H
#define _CRAZINES_CPU_6502_H

#include <cstdint>
#include <string>
#include <mappers/mapper.h>
#include <mappers/mappers.h>
#include <rom.h>
#include <opcodes.h>
#include <ppu.h>

namespace nes
{
    class cpu_6502;
    class PPU;

    class opcode_t
    {
      private:
        void decode_instruction();
        cpu_6502 *cpu;
      public:
        opcode_t(nes::cpu_6502 *cpu, uint16_t addr);
        std::string to_string();
        uint32_t instruction = 0;
        uint8_t opcode = 0;
        uint16_t imm = 0;
        uint8_t sz = 1;
        uint16_t addr = 0;
    };

   class cpu_6502
   {
     public:
       cpu_6502(void *);
       uint16_t get_PC();
       uint8_t get_A();
       std::string get_state();
       uint8_t get_status();
       uint32_t get_cycles();
       std::string step();
       void load_rom(std::string filename);
       Mapper *mapper;
       void nmi();
     private:
/** Make an NES class and move this there */
       nes::ROM *rom = nullptr;
       nes::NES_header header;
       PPU *ppu = nullptr;
/******************************************/

       uint8_t A = 0, X = 0, Y = 0, status = 0x24, SP = 0xFD;
       uint16_t PC = 0;
       uint32_t cycles = 0, last_cycles = 0;

       void push8(uint8_t value);
       uint8_t pop8();
       void push16(uint16_t value);
       uint16_t pop16();
       void setN(uint8_t check);
       bool checkN();
       void setV(uint8_t check);
       void clearV();
       bool checkV();
       void setZ(uint8_t check);
       bool checkZ();
       void setC();
       void clearC();
       bool checkC();
       void setI();
       void clearI();
       void setD();
       void clearD();

       void (*opcode[0x100])(cpu_6502 *, opcode_t *);
       static void not_implemented(cpu_6502 *, opcode_t *);

       static void brk(cpu_6502 *cpu, opcode_t *op); // 0x00
       static void slo_zp(cpu_6502 *, opcode_t *); // 0x07
       static void php(cpu_6502 *cpu, opcode_t *op); // 0x08
       static void ora_imm(cpu_6502 *cpu, opcode_t *op); // 0x09
       static void slo_abs(cpu_6502 *cpu, opcode_t *op); // 0x0F
       static void bpl(cpu_6502 *cpu, opcode_t *op); // 0x10
       static void clc(cpu_6502 *cpu, opcode_t *op); // 0x18
       static void jsr(cpu_6502 *cpu, opcode_t *op); // 0x20
       static void and_ind_x(cpu_6502 *cpu, opcode_t *op); // 0x21
       static void bit_zp(cpu_6502 *cpu, opcode_t *op); // 0x24
       static void plp(cpu_6502 *cpu, opcode_t *op); // 0x28
       static void and_imm(cpu_6502 *cpu, opcode_t *op); // 0x29
       static void bmi(cpu_6502 *cpu, opcode_t *op); // 0x30
       static void rla_zp_x(cpu_6502 *, opcode_t *); // 0x37
       static void sec(cpu_6502 *cpu, opcode_t *op); // 0x38
       static void rti(cpu_6502 *cpu, opcode_t *op); // 0x40
       static void pha(cpu_6502 *cpu, opcode_t *op); // 0x48
       static void eor_imm(cpu_6502 *cpu, opcode_t *op); // 0x49
       static void jmp_abs(cpu_6502 *cpu, opcode_t *op); // 0x4C
       static void bvc(cpu_6502 *cpu, opcode_t *op); // 0x50
       static void cli(cpu_6502 *, opcode_t *op); // 0x58
       static void rts(cpu_6502 *cpu, opcode_t *op); // 0x60
       static void pla(cpu_6502 *cpu, opcode_t *op); // 0x68
       static void adc_imm(cpu_6502 *cpu, opcode_t *op); // 0x69
       static void bvs(cpu_6502 *cpu, opcode_t *op); // 0x70
       static void sei(cpu_6502 *cpu, opcode_t *op); // 0x78
       static void sta_ind_x(cpu_6502 *, opcode_t *); // 0x81
       static void sta_zp(cpu_6502 *cpu, opcode_t *op); // 0x85
       static void stx_zp(cpu_6502 *cpu, opcode_t *op); // 0x86
       static void dey(cpu_6502 *, opcode_t *); // 0x88
       static void sta_abs(cpu_6502 *cpu, opcode_t *op); // 0x8D
       static void stx_abs(cpu_6502 *cpu, opcode_t *op); // 0x8E
       static void bcc(cpu_6502 *cpu, opcode_t *op); // 0x90
       static void sta_ind_y(cpu_6502 *, opcode_t *op); // 0x91
       static void txs(cpu_6502 *cpu, opcode_t *op); // 0x9A
       static void ldy_imm(cpu_6502 *cpu, opcode_t *op); // 0xA0
       static void ldx_imm(cpu_6502 *cpu, opcode_t *op); // 0xA2
       static void lda_zp(cpu_6502 *, opcode_t *); // 0xA5
       static void ldx_zp(cpu_6502 *, opcode_t *); // 0xA6
       static void lda_imm(cpu_6502 *cpu, opcode_t *op); // 0xA9
       static void lda_abs(cpu_6502 *cpu, opcode_t *op); // 0xAD
       static void bcs(cpu_6502 *cpu, opcode_t *op); // 0xB0
       static void lda_zp_x(cpu_6502 *cpu, opcode_t *op); // 0xB5
       static void clv(cpu_6502 *cpu, opcode_t *op); // 0xB8
       static void lda_abs_x(cpu_6502 *, opcode_t *); //  0xBD
       static void cpy_imm(cpu_6502 *cpu, opcode_t *op); // 0xC0
       static void cmp_ind_x(cpu_6502 *, opcode_t *); // 0xC1
       static void nop_imm_C2(cpu_6502 *, opcode_t *); // 0xC2
       static void dec_zp(cpu_6502 *cpu, opcode_t *op); // 0xC6
       static void cmp_imm(cpu_6502 *cpu, opcode_t *op); // 0xC9
       static void dex(cpu_6502 *cpu, opcode_t *op); // 0xCA
       static void dec_abs(cpu_6502 *cpu, opcode_t *op); // 0xCE
       static void bne(cpu_6502 *cpu, opcode_t *op); // 0xD0
       static void cmp_zp_x(cpu_6502 *, opcode_t *); // 0xD5
       static void cld(cpu_6502 *cpu, opcode_t *op); // 0xD8
       static void cpx_imm(cpu_6502 *cpu, opcode_t *op); // 0xE0
       static void nop_ea(cpu_6502 *cpu, opcode_t *op); // 0xEA
       static void sbc_imm(cpu_6502 *, opcode_t *); // 0xE9
       static void beq(cpu_6502 *cpu, opcode_t *op); // 0xF0
       static void nop_zp_F4(cpu_6502 *, opcode_t *); // 0xF4
       static void sed(cpu_6502 *, opcode_t *); // 0xF8
   };
}

#endif
