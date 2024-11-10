#include "semu.hh"

#include <bitset>

namespace semu
{
    Cpu::Cpu()
    {
        PC = 0x0;
        Registers.resize(32, 0);
        Memory.resize(64 * 1024 * 1024, 0);
    }

    Cpu::Cpu(const std::vector<std::uint8_t>& Image, std::uint32_t Offset)
    {
        // std::cout << "[INFO]: Cpu initing." << "\n";
        Info("Cpu initing", 1, 'b');
        PC = 0;
        Registers.resize(32, 0);
        Memory.resize(64 * 1024 * 1024, 0);
        VirtualPageBase = 0x80000000;

        std::cout << "[INFO]: Register and memory initialized finished." << "\n";
        std::copy(Image.begin() + Offset, Image.end(), Memory.begin());
    }

    Cpu::~Cpu()
    {
        std::cout << "[INFO]: Power off." << "\n";
    }

    int Cpu::Run()
    {
        for (size_t i = 0; i < 4; i++)
        {
            int Result = Step();
        }
        
        return 0;
    }

    
    int Cpu::Step()
    {
        std::uint32_t IR = Fetch();
        std::uint32_t OPCode = (IR & 0x7f);
        Info("OPCODE ", std::bitset<8>(OPCode), "IR", std::bitset<32>(IR));
        // https://github.com/riscv/riscv-opcodes/blob/master/
        // Chapter 37 in riscv manual 2024/04

        switch (OPCode)
        {
        // rv32i base
        case 0x37: //LUI
            
            break;

        case 0x17: // AUIPC
            
            break;

        case 0x6f: // JAL
            
            break;
        
        case 0x67: // JALR

            break;
        
        case 0x63: // BEQ/BNE/BLT/BGE/BLTU/BGEU

            break;
        
        case 0x03: // LB/LH/LW/LBU/LHU

            break;

        case 0x23: // SB/SH/SW

            break;
        
        case 0x13: // ADDI/SLTI/SLTIU/XORI/ORI/ADDI/SLLI/SRLI/SRAI
            
            break;
        case 0x22: // ADD/SUB/SLL/SLT/SLTU/XOR/SRL/SRA/OR/AND
        
            break;
        default:
            break;
        }

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

        Info("Before",  std::bitset<32>((U0) | (U1 << 8) | (U2 << 16) | (U3 << 24)),
             "After",   std::bitset<32>((U3) | (U2 << 8) | (U1 << 16) | (U0 << 24)));
        return ((U0) | (U1 << 8) | (U2 << 16) | (U3 << 24));
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
