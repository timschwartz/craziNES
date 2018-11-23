#pragma once

#include <cstdint>
#include <mappers/mapper.h>
#include <ppu.h>

namespace nes
{
    struct memory_section;
    class PPU;
    class Mapper;

    class Mapper0: public Mapper
    {
      public:
        Mapper0(nes::ROM *, nes::PPU *);
        uint8_t read_byte(uint16_t addr);
        void write_byte(uint16_t addr, uint8_t value);
      private:
        nes::memory_section get_section(uint16_t addr);
        uint8_t *ram = new uint8_t[0x800];
        uint8_t *registers = new uint8_t[0x20];
        uint8_t *lprgrom = new uint8_t[0x4000];
        uint8_t *hprgrom = new uint8_t[0x4000];
        nes::PPU *ppu = nullptr;
    };
}
