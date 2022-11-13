#include <fstream>
#include <cstring>
#include "rom.hpp"

namespace nes
{
    ROM::ROM(std::string filename)
    {
        std::fstream rom_file;
        char message[1024];

        try
        {
            rom_file.open(filename.c_str(), std::fstream::in);
        }
        catch (std::ios_base::failure &e)
        {
            throw e.what();
        }

        if(!rom_file.good())
        {
            std::string msg = "Couldn't open file " +  filename;
            throw msg;
        }

        rom_file.seekg(0, std::ios::end);
        sz = rom_file.tellg();

        data = new uint8_t[sz];
        memset(data, 0, sz);
        rom_file.seekg(0, std::ios::beg);
        rom_file.read((char *)data, sz);
        rom_file.close();

        memcpy(&header.magic, data, 4);
        switch(header.magic)
        {
            case 0x4E45531A:
                break;
            case 0x1A53454E:
                break;
            default:
                sprintf(message, "%s is not an NES rom.\nHeader: 0x%.8X\n", filename.c_str(), header.magic);
                throw std::string(message);
                break;
        }
      
        header.prgrom_size = data[4];
        header.chrrom_size = data[5];

        header.flag6 = data[6];
        header.flag7 = data[7];

        switch((header.flag7 >> 2) & 3)
        {
            case 0: // NES format
                return;
                break;
            case 2:
                // NES 2.0 format
                break;
            default:
                sprintf(message, "%s is not an NES rom.\nFlag 7: 0x%.8X\n", filename.c_str(), header.magic);
                throw std::string(message);
                break;
        }
        
    }

    ROM::~ROM()
    {
        delete data;
    }

    NES_header ROM::get_header()
    {
        return header;
    }

    size_t ROM::get_size()
    {
        return sz;
    }

    uint8_t *ROM::get_pointer()
    {
        return data;
    }
}
