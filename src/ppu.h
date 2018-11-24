#pragma once

#include <cstdint>
#include <mappers/mappers.h>
#include <cpu_6502.h>

namespace nes
{
    class cpu_6502;

    class PPU 
    {
      public:
        PPU(nes::cpu_6502 *);
        void step(uint16_t);
        static void register_write_byte(void *ptr, uint16_t addr, uint8_t value);
        static uint8_t register_read_byte(void *ptr, uint16_t addr);
      private:
        uint8_t registers[8];
        uint16_t scanline = 0;
        uint16_t pixel = 0;
        nes::cpu_6502 *cpu = nullptr;
    };
}
