#ifndef _CRAZINES_CPU_6502_H
#define _CRAZINES_CPU_6502_h

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
       std::string step();
       void load_rom(std::string filename);
       MMU *mmu;
     private:
       uint8_t A = 0, X = 0, Y = 0, status = 0x24, SP = 0xFD;
       uint16_t PC = 0;
       uint32_t cycles = 0;

       void setZ(uint16_t check);

       void (*opcode[0xFF])(cpu_6502 *, opcode_t *);
       static void not_implemented(cpu_6502 *, opcode_t *);

       static void jmp_abs(cpu_6502 *cpu, opcode_t *op); // 0x4C
       static void stx_zp(cpu_6502 *cpu, opcode_t *op); // 0x86
       static void ldx_imm(cpu_6502 *cpu, opcode_t *op); // 0xA2
   };
}

#endif
