#include <string>
#include <cstring>

#include <mappers/mapper.h>

namespace nes
{
    uint8_t Mapper::read_byte(uint16_t addr)
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

    void Mapper::write_byte(uint16_t addr, uint8_t value)
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

    void Mapper::ppu_write(Mapper *mapper, memory_section s)
    {
        char message[1024];
        sprintf(message, "Wrote 0x%X to 0x%X", mapper->read_byte(s.addr), s.addr);
        throw std::string(message);
    }
}
