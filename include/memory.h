#ifndef _CRAZINES_MEMORY_H
#define _CRAZINES_MEMORY_H

#include <cstdint>

namespace nes
{
    class MMU;
    struct memory_section;

    typedef struct memory_section
    {
        uint16_t addr = 0;
        uint16_t offset = 0;
        void *ptr = nullptr;
        void (*write_handler)(MMU *mmu, memory_section s) = nullptr;
    } memory_section;

    class MMU
    {
      public:
        uint8_t read_byte(uint16_t addr);
        void write_byte(uint16_t addr, uint8_t value);
      private:
        memory_section get_section(uint16_t addr);
        static void ppu_write(MMU *mmu, memory_section s);
        uint8_t *ram = new uint8_t[0x800];
        uint8_t *ppu_registers = new uint8_t[0x8];
        uint8_t *registers = new uint8_t[0x20];
        uint8_t *lprgrom = new uint8_t[0x4000];
        uint8_t *hprgrom = new uint8_t[0x4000];
    };
}

#endif
