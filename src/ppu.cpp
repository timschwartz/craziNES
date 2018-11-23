#include <ppu.h>
#include <iostream>

namespace nes
{
    void PPU::step(uint16_t cycles)
    {
    }

    void PPU::register_write_byte(void *ptr, uint16_t addr, uint8_t value)
    {
        PPU *ppu = (PPU *)ptr;
        ppu->step(0);
std::cout << "register_write_byte()" << std::endl;
    }
}
