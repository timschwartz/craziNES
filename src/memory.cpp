#include <string>

#include <memory.h>

namespace nes
{
    memory_section MMU::get_section(uint16_t addr)
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
            s.addr = ((addr - 0x2000) % 0x8) + 0x2000;
            s.offset = 0x2000;
            s.ptr = ppu_registers;
            s.write_handler = &ppu_write;
            return s;
        }

        if((addr >= 0x4000) && (addr <= 0x401F))
        {
            s.addr = addr;
            s.offset = 0x4000;
            s.ptr = registers;
            return s;
        }

        char message[1024];
        sprintf(message, "Unknown memory address 0x%.4X", addr);
        throw std::string(message);
    }

    uint8_t MMU::read_byte(uint16_t addr)
    {
        memory_section s;
        try
        {
            s = get_section(addr);
        }
        catch(std::string e)
        {
            throw e;
        }
         
        uint8_t *p = (uint8_t *)s.ptr;

        return p[s.addr - s.offset];
    }

    void MMU::write_byte(uint16_t addr, uint8_t value)
    {
        memory_section s;
        try
        {
            s = get_section(addr);
        }
        catch(std::string e)
        {
            throw e;
        }

        uint8_t *p = (uint8_t *)s.ptr;
        p[s.addr - s.offset] = value;

        if(s.write_handler == nullptr) return ;

        try
        {
            s.write_handler(this, s);
        }
        catch(std::string e)
        {
            throw e;
        }
    }

    void MMU::ppu_write(MMU *mmu, memory_section s)
    {
        char message[1024];
        sprintf(message, "Wrote 0x%X to 0x%X", mmu->read_byte(s.addr), s.addr);
        throw std::string(message);
    }
}
