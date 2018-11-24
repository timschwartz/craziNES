#include <string>
#include <cstring>
#include <iostream>

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

        if(s.read_handler != nullptr)
        {
            uint8_t result;
            try
            {
                result = s.read_handler(s.ptr, addr);
            }
            catch(std::string e)
            {
                throw e;
            }
            return result;
        }
         
        uint8_t *p = (uint8_t *)s.ptr;

        return p[s.addr - s.offset];
    }

    void Mapper::write_byte(uint16_t addr, uint8_t value)
    {
        nes::memory_section s;
        try
        {
            s = get_section(addr);
        }
        catch(std::string e)
        {
            throw e;
        }

        if(s.write_handler != nullptr)
        {
            try
            {
                s.write_handler(s.ptr, addr, value);
            }
            catch(std::string e)
            {
                throw e;
            }
            return;
        }

        uint8_t *p = (uint8_t *)s.ptr;
        p[s.addr - s.offset] = value;
    }
}
