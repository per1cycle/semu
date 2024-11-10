#include "semu.hh"

#include <iomanip>
#include <iostream>

/**
 * load os image or binary file
 * @note run executable file need manually set offset.
 */

std::vector<std::uint8_t> LoadImage(std::string FileName)
{
    std::ifstream Image(FileName.c_str(), std::ios::binary);

    return std::vector<std::uint8_t>(
        (std::istreambuf_iterator<char>(Image)), 
        std::istreambuf_iterator<char>()
    );
}
// notice the endian.
int main(int argc, char const *argv[])
{
    std::vector<std::uint8_t> Image = LoadImage("/home/z/Projects/Github/dev/semu/riscv-tests/isa/rv64ui-p-add");
    size_t Offset = 0x1000;
    std::cout << Image.size() << '\n';
    
    for(size_t i = 0; i + Offset < Image.size(); i += 1)
    {
        if(i && i % 32 == 0)
        {
            std::cout << '\n';
        }
        std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint16_t>(Image[i + Offset]);
    }

    return 0;
}
