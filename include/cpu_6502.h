#ifndef _CRAZINES_CPU_6502_H
#define _CRAZINES_CPU_6502_H

#include <cstdint>
#include <string>
#include <memory.h>
#include <opcodes.h>

namespace nes
{
    class opcode_t
    {
      private:
        void decode_instruction();
      public:
        opcode_t(nes::MMU *mmu, uint16_t addr);
        std::string to_string();
        uint32_t instruction = 0;
        uint8_t opcode = 0;
        uint16_t imm = 0;
        uint8_t sz = 1;
    };

   class cpu_6502
   {
     public:
       cpu_6502();
       uint16_t get_PC();
       std::string get_state();
       uint8_t get_status();
       std::string step();
       void load_rom(std::string filename);
       MMU *mmu;
     private:
       uint8_t A = 0, X = 0, Y = 0, status = 0x24, SP = 0xFD;
       uint16_t PC = 0;
       uint32_t cycles = 0;

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
       void setD();
       void clearD();

       void (*opcode[0x100])(cpu_6502 *, opcode_t *);
       static void not_implemented(cpu_6502 *, opcode_t *);

       static void brk(cpu_6502 *cpu, opcode_t *op); // 0x00
       static void bpl(cpu_6502 *cpu, opcode_t *op); // 0x10
       static void clc(cpu_6502 *cpu, opcode_t *op); // 0x18
       static void jsr(cpu_6502 *cpu, opcode_t *op); // 0x20
       static void bit_zp(cpu_6502 *cpu, opcode_t *op); // 0x24
       static void and_imm(cpu_6502 *cpu, opcode_t *op); // 0x29
       static void sec(cpu_6502 *cpu, opcode_t *op); // 0x38
       static void rti(cpu_6502 *cpu, opcode_t *op); // 0x40
       static void jmp_abs(cpu_6502 *cpu, opcode_t *op); // 0x4C
       static void bvc(cpu_6502 *cpu, opcode_t *op); // 0x50
       static void rts(cpu_6502 *cpu, opcode_t *op); // 0x60
       static void bvs(cpu_6502 *cpu, opcode_t *op); // 0x70
       static void sei(cpu_6502 *cpu, opcode_t *op); // 0x78
       static void sta_zp(cpu_6502 *cpu, opcode_t *op); // 0x85
       static void stx_zp(cpu_6502 *cpu, opcode_t *op); // 0x86
       static void sta_abs(cpu_6502 *cpu, opcode_t *op); // 0x8D
       static void bcc(cpu_6502 *cpu, opcode_t *op); // 0x90
       static void txs(cpu_6502 *cpu, opcode_t *op); // 0x9A
       static void ldx_imm(cpu_6502 *cpu, opcode_t *op); // 0xA2
       static void lda_imm(cpu_6502 *cpu, opcode_t *op); // 0xA9
       static void lda_abs(cpu_6502 *cpu, opcode_t *op); // 0xAD
       static void bcs(cpu_6502 *cpu, opcode_t *op); // 0xB0
       static void clv(cpu_6502 *cpu, opcode_t *op); // 0xB8
       static void bne(cpu_6502 *cpu, opcode_t *op); // 0xD0
       static void cld(cpu_6502 *cpu, opcode_t *op); // 0xD8
       static void nop_ea(cpu_6502 *cpu, opcode_t *op); // 0xEA
       static void beq(cpu_6502 *cpu, opcode_t *op); // 0xF0
   };
}

#endif
