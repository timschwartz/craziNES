#ifndef _CRAZINES_ROM_H
#define _CRAZINES_ROM_H

#include <cstdint>
#include <string>

namespace nes
{
    struct NES_header
    {
        uint32_t magic;
        uint8_t prgrom_size = 0;
        uint8_t chrrom_size = 0;
        uint8_t flag6 = 0;
        uint8_t flag7 = 0;

        uint8_t mapper = 0;
        uint8_t rom_upper = 0;
        uint8_t ram_size = 0;
        uint8_t vram_size = 0;
        uint8_t tv_system = 0;
        uint8_t vs_hardware = 0;
        uint8_t misc_roms = 0;
        
    };

    class ROM
    {
      public:
        ROM(std::string filename);
        ~ROM();
        NES_header get_header();
        size_t get_size();
        uint8_t *get_pointer();
      private:
        size_t sz;
        uint8_t *data;
        NES_header header;
    };
}

#endif
