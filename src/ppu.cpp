#include <ppu.h>
#include <iostream>

namespace nes
{
    PPU::PPU(cpu_6502 *cpu)
    {
        this->cpu = cpu;
    }

    void PPU::step(uint16_t cpu_cycles)
    {
        uint16_t ppu_cycles = cpu_cycles * 3;

        for(uint16_t counter = 0; counter < ppu_cycles; counter++)
        {
            if((this->scanline >=0) && (this->scanline <= 240))
            {
                // draw stuff
            }

            if((this->scanline == 241) && (this->pixel == 0))
            {
                // Start vblank
                this->registers[2] |= 0x80;

                if((this->registers[0] >> 7) & 0x1)
                {
                    // NMI
                    std::cout << "PPU::step(): NMI" << std::endl;
                    this->cpu->nmi();
                }
                else std::cout << "NMI disabled" << std::endl;
            }

            if((this->scanline == 261) && (this->pixel == 0))
            {
                // end vblank
                this->registers[2] &= ~0x80;
            }

            this->pixel++;

            if(this->pixel > 340)
            {
                this->pixel = 0;
                this->scanline++;
            }

            if(this->scanline > 261)
            {
                this->scanline = 0;
            }
        }
    }

    void PPU::register_write_byte(void *ptr, uint16_t addr, uint8_t value)
    {
        PPU *ppu = (PPU *)ptr;

        addr -= 0x2000;
        addr %= 8;

        ppu->registers[addr] = value;
        std::cout << "PPU::register_write_byte(): Wrote 0x" << std::hex << +value << " to 0x" << std::hex << +addr << std::endl;

        switch(addr)
        {
            case 0: // PPUCTRL
                std::cout << "Base name table address: 0x" << std::hex << +ppu->base_name_table[(value & 0x3)] << std::endl;

                std::cout << "VRAM address increment per CPU read/write of PPUDATA: ";
                if((value >> 2) & 0x1) std::cout << "add 32, going down";
                else std::cout << "add 1, going across";
                std::cout << std::endl;

                std::cout << "Sprite pattern table address for 8x8 sprites: 0x" << std::hex << +ppu->sprite_pattern_table[((value >> 3) & 0x1)] << std::endl;

                std::cout << "Background pattern table address: 0x" << std::hex << +ppu->background_pattern_table[((value >> 4) & 0x1)] << std::endl;;

                std::cout << "Sprite size: ";
                if((value >> 5) & 0x1) std::cout << "8x16 pixels";
                else std::cout << "8x8 pixels";
                std::cout << std::endl;

                std::cout << "PPU master/slave select: ";
                if((value >> 6) & 0x1) std::cout << "output color on EXT pins";
                else std::cout << "read backdrop from EXT pins";
                std::cout << std::endl;

                std::cout << "Generate an NMI at the start of the vertical blanking interval: ";
                if((value >> 7) & 0x1) std::cout << "on";
                else std::cout << "off";
                std::cout << std::endl;

                break;
            default:
                std::cout << "Unhandled ppu register: 0x" << +addr << std::endl;
                break;
        }
    }

    uint8_t PPU::register_read_byte(void *ptr, uint16_t addr)
    {
        PPU *ppu = (PPU *)ptr;
        addr -= 0x2000;
        addr %= 8;

        std::cout << "PPU::register_read_byte(): Read 0x" << std::hex << +ppu->registers[addr] << " from 0x" << std::hex << +addr << std::endl;

        uint8_t value = ppu->registers[addr];

        switch(addr)
        {
            case 0x2:
                // end vblank
                ppu->registers[2] &= ~0x80;
                break;
            default:
                break;
        }

        return value;
    }

}
