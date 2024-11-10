#include "semu.hh"

#include <bitset>
#include <iostream>

namespace semu
{
    Cpu::Cpu()
    {
        std::cout << "[INFO]: Cpu initing." << "\n";
        PC = 0;
        Registers.resize(32, 0);
        Memory.resize(64 * 1024 * 1024, 0);
        std::cout << "[INFO]: Register and memory initialized finished." << "\n";
    }

    Cpu::Cpu(const std::vector<std::uint8_t>& Image, std::uint32_t Offset)
    {
        std::cout << "[INFO]: Cpu initing." << "\n";
        PC = 0;
        Registers.resize(32, 0);
        Memory.resize(64 * 1024 * 1024, 0);
        std::cout << "[INFO]: Register and memory initialized finished." << "\n";
        for(size_t i = 0; i < Image.size(); i ++)
        {
            Memory[i] = Image[i + Offset];
        }
        std::cout << "[INFO]: " << "Load image." << '\n';
    }

    Cpu::~Cpu()
    {
        std::cout << "[INFO]: Power off." << "\n";
    }

    int Cpu::Run()
    {
        for (size_t i = 0; i < 16; i++)
        {
            int Result = Step();

        }
        
        return 0;
    }

    int Cpu::Step()
    {
        std::uint32_t IR = Fetch();
        PC += 4;
        return 0;
    }

    std::uint32_t Cpu::Fetch()
    {
        
        /**
         * 0    1   2   3
         * 12   34  56  78
         * 
         * 3    2   1   0
         * 78   56  34  12
         */
        std::uint8_t U0 = Memory[PC];
        std::uint8_t U1 = Memory[PC + 1];
        std::uint8_t U2 = Memory[PC + 2];
        std::uint8_t U3 = Memory[PC + 3];

        std::cout << "[INFO]: " << "Before:\t" <<
                std::bitset<32>((U0) | (U1 << 8) | (U2 << 16) | (U3 << 24))  << '\n';
        std::cout << "[INFO]: " << "After: \t" <<
                 std::bitset<32>((U3) | (U2 << 8) | (U1 << 16) | (U0 << 24)) << '\n';

        return ((U3) | (U2 << 8) | (U1 << 16) | (U0 << 24));
    }
    
    void Cpu::MemoryLayout()
    {
        for(size_t i = 0; i < 1024; i ++)
        {
            if(i && i % 16 == 0)
                std::cout << "\n";
             std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint16_t>(Memory[i]) << ' ';
        }
        std::cout << "\n";
    }

} // namespace semu
