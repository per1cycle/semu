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

Cpu::Cpu(const std::vector<std::uint8_t> &Image, std::uint32_t Offset)
{
    Info("Cpu initing...");
    PC = 0;
    Registers.resize(32, 0);
    Memory.resize(64 * 1024 * 1024, 0);
    VirtualPageBase = 0x80000000;
    Info("Register and memory initialized finished.");
    std::copy(Image.begin() + Offset, Image.end(), Memory.begin());
}

Cpu::~Cpu()
{
    Info("Power off");
}

int Cpu::Run()
{
    for (size_t i = 0; i < 256; i++)
    {
        int Result = Step();
    }

    return 0;
}

int Cpu::Step()
{
    std::uint32_t IR = Fetch();
    std::uint32_t OPCode = OP(IR);
    // https://github.com/riscv/riscv-opcodes/blob/master/
    // Chapter 37 in riscv manual 2024/04

    // Add Zicsr extension
    switch (OPCode)
    {
    // rv32i base
    case 0x37: // LUI
    {
        Info("LUI");
        break;
    }

    case 0x17: // AUIPC
    {
        Info("AUIPC");
        break;
    }

    case 0x6f: // JAL
    {
        Info("JAL");
        break;
    }

    case 0x67: // JALR
    {
        Info("JALR");
        break;
    }

    case 0x63: // BEQ/BNE/BLT/BGE/BLTU/BGEU
    {
        Info("BEQ");
        break;
    }

    case 0x03: // LB/LH/LW/LBU/LHU
    {
        Info("LB");
        break;
    }

    case 0x23: // SB/SH/SW
    {
        Info("SB");
        break;
    }

    case 0x13: // ADDI/SLTI/SLTIU/XORI/ORI/ADDI/SLLI/SRLI/SRAI
    {
        std::uint8_t rd = RD(IR);
        std::uint8_t func3 = FUNC3(IR);
        std::uint8_t rs1 = RS1(IR);
        std::int16_t imm = IMM12(IR);
        
        if(func3 == 0x00) // ADDI
        {
            
        }

        break;
    }
    case 0x33: // ADD/SUB/SLL/SLT/SLTU/XOR/SRL/SRA/OR/AND
    {
        std::uint8_t rd = RD(IR);
        std::uint8_t rs1 = RS1(IR);
        std::uint8_t rs2 = RS2(IR);
        std::uint8_t func3 = FUNC3(IR);
        std::uint8_t func7 = FUNC7(IR);

        if (func3 == 0) // ADD/SUB
        {
            if (func7 == 0x00) // ADD
                Registers[rd] = Registers[rs1] + Registers[rs2];
            else if (func7 == 0x20) // SUB
                Registers[rd] = Registers[rs1] - Registers[rs2];
        }
        else if (func3 == 0x1) // SLL
        {
            Registers[rd] = Registers[rs1] << (Registers[rs2] & 0x1f);
        }
        else if (func3 == 0x2) // SLT
        {
            Registers[rd] =
                static_cast<std::int64_t>(Registers[rs1]) < static_cast<std::int64_t>(Registers[rs2]) ? 1 : 0;
        }
        else if (func3 == 0x3) // SLTU
        {
            Registers[rd] = Registers[rs1] < Registers[rs2] ? 1 : 0;
        }
        else if (func3 == 0x4) // XOR
        {
            Registers[rd] = Registers[rs1] ^ Registers[rs2];
        }
        else if (func3 == 0x5) // SRL/SRA
        {
            // https://stackoverflow.com/questions/7622/are-the-shift-operators-arithmetic-or-logical-in-c
            if (func7 == 0x00) // SRL
            {
                Registers[rd] = Registers[rs1] >> (Registers[rs2] & 0x1f);
            }
            else if (func7 == 0x20) // SRA
            {
                Registers[rd] =
                    static_cast<std::uint64_t>(static_cast<std::int64_t>(Registers[rs1]) >> (Registers[rs2] & 0x1f));
            }
        }
        else if (func3 == 0x6) // OR
        {
            Registers[rd] = Registers[rs1] | Registers[rs2];
        }
        else if (func3 == 0x7) // AND
        {
            Registers[rd] = Registers[rs1] & Registers[rs2];
        }
        break;
    }
    case 0x73: {
        Info("CSR");
        break;
    }
    default:
        Error(PC, "Instruction not found.", std::bitset<8>(OP(IR)));
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

    // Info("Before",  std::bitset<32>((U0) | (U1 << 8) | (U2 << 16) | (U3 << 24)),
    //      "After",   std::bitset<32>((U3) | (U2 << 8) | (U1 << 16) | (U0 << 24)));
    return ((U0) | (U1 << 8) | (U2 << 16) | (U3 << 24));
}

void Cpu::MemoryLayout()
{
    for (size_t i = 0; i < 1024; i++)
    {
        if (i && i % 16 == 0)
            std::cout << "\n";
        std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint16_t>(Memory[i]) << ' ';
    }
    std::cout << "\n";
}

} // namespace semu
