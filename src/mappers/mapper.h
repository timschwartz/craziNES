#pragma once

#include <cstdint>
#include <rom.h>

namespace nes
{
    class Mapper;

    typedef struct memory_section
    {
        uint16_t addr = 0;
        uint16_t offset = 0;
        void *ptr = nullptr;
        void (*write_handler)(void *, uint16_t, uint8_t) = nullptr;
    } memory_section;

    class Mapper
    {
      public:
        virtual uint8_t read_byte(uint16_t addr) = 0;
        virtual void write_byte(uint16_t addr, uint8_t value) = 0;
      protected:
        virtual memory_section get_section(uint16_t addr) = 0;
        nes::ROM *rom = nullptr;
    };
}
