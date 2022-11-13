#pragma once

#include <cstdint>
#include "nes/mappers/mappers.hpp"
#include <cpu_6502.h>

namespace nes
{
    class cpu_6502;

    class PPU 
    {
      public:
        PPU(nes::cpu_6502 *, void *);
        void step(uint16_t);
        static void register_write_byte(void *ptr, uint16_t addr, uint8_t value);
        static uint8_t register_read_byte(void *ptr, uint16_t addr);
        void chrrom_map(void *ptr);
      private:
        void *screen = nullptr;
        uint8_t registers[8];
        uint8_t *chrrom = nullptr;
        uint8_t *vram = new uint8_t[0x800];

        uint16_t base_name_table[4] = { 0x2000, 0x2400, 0x2800, 0x2C00 };
        uint16_t sprite_pattern_table[2] = { 0x0000, 0x1000 };
        uint16_t background_pattern_table[2] = { 0x0000, 0x1000 };

        bool latch = false;
        uint16_t ppu_addr = 0;
        uint16_t ppu_scroll = 0;

        uint16_t scanline = 0;
        uint16_t pixel = 0;
        nes::cpu_6502 *cpu = nullptr;

        memory_section get_section(uint16_t addr);
        void write_byte(uint16_t addr, uint8_t value);
    };
}
