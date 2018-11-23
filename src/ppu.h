#pragma once

#include <cstdint>

namespace nes
{
    class PPU 
    {
      public:
        void step(uint16_t);
        void write_byte(uint16_t addr, uint8_t value);
    };
}
