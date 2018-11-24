#include <string>
#include <cstring>
#include <iostream>

#include <mappers/mapper0.h>

namespace nes
{
    Mapper0::Mapper0(nes::ROM *rom, nes::PPU *ppu)
    {
        this->rom = rom;
        this->ppu = ppu;

        nes::NES_header header = rom->get_header();

        if(header.prgrom_size == 1)
        {
            memcpy(lprgrom, rom->get_pointer() + 16, 0x4000);
            memcpy(hprgrom, rom->get_pointer() + 16, 0x4000);
            return;
        }

        if(header.prgrom_size == 2)
        {
            memcpy(lprgrom, rom->get_pointer() + 16, 0x4000);
            memcpy(hprgrom, rom->get_pointer() + 16 + 0x4000, 0x4000);
            return;
        }

        throw std::string("Unknown prgrom_size: " + header.prgrom_size);
    }

    memory_section Mapper0::get_section(uint16_t addr)
    {
        memory_section s;

        if((addr >= 0) && (addr <= 0x1FFF))
        {
            s.addr = addr % 0x800;
            s.offset = 0x0;
            s.ptr = ram;
            return s; 
        }

        if((addr >= 0x2000) && (addr <= 0x3FFF))
        {
            s.ptr = this->ppu;
            s.write_handler = &PPU::register_write_byte;
            s.read_handler = &PPU::register_read_byte;
            return s;
        }

        if((addr >= 0x4000) && (addr <= 0x401F))
        {
            s.addr = addr;
            s.offset = 0x4000;
            s.ptr = registers;
            return s;
        }

        if((addr >= 0x6000) && (addr <= 0x7FFF))
        {
            s.addr = addr;
            s.offset = 0x6000;
            s.ptr = prgram;
            return s;
        }

        if((addr >= 0x8000) && (addr <= 0xBFFF))
        {
            s.addr = addr;
            s.offset = 0x8000;
            s.ptr = lprgrom;
            return s;
        }

        if((addr >= 0xC000) && (addr <= 0xFFFF))
        {
            s.addr = addr;
            s.offset = 0xC000;
            s.ptr = hprgrom;
            return s;
        }

        char message[1024];
        sprintf(message, "Unknown memory address 0x%.4X", addr);
        throw std::string(message);
    }
}
