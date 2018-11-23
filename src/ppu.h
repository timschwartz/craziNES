#pragma once

#include <cstdint>
#include <mappers/mappers.h>

namespace nes
{
    class PPU 
    {
      public:
        void step(uint16_t);
        static void register_write_byte(void *ptr, uint16_t addr, uint8_t value);
    };
}
