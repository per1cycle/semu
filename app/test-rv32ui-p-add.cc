#include "semu.hh"

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
    if(argc < 2)
    {
        std::cerr << "Usage: ./app <path to riscv binary file>\n";
        return 1;
    }

    std::cout << std::hex;
    std::vector<std::uint8_t> Image = LoadImage(std::string(argv[1]));
    size_t Offset = 0x1000;
    
    semu::Cpu cpu = semu::Cpu(Image, Offset);
    cpu.Run();
    // cpu.MemoryLayout();
    
    return 0;
}
