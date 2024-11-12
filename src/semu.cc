#include "semu.hh"

#include <bitset>

namespace semu {
Cpu::Cpu()
{
    Info("Cpu initing...");
    PC = 0;
    Registers.resize(32, 0);
    Memory.resize(MEM_SIZE, 0);
    VirtualPageBase = 0x80000000;
    Info("Register and memory initialized finished.");
}

Cpu::Cpu(const std::vector<std::uint8_t>& Image, std::uint32_t Offset)
{
    Info("Cpu initing...");
    PC = 0;
    Registers.resize(32, 0x0);
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
    for (size_t i = 0; i < 64; i++) {
        int Result = Step();
        RegisterLayout();
        if (Result)
            break;
    }

    return 0;
}

int Cpu::Step()
{
    std::uint32_t IR = Fetch();
    // Info(std::bitset<32>(IR));
    std::uint32_t OPCode = OP(IR);
    // https://github.com/riscv/riscv-opcodes/blob/master/
    // Chapter 37 in riscv manual 2024/04

    // Add Zicsr extension
    switch (OPCode) {
    // rv32i base
    case 0x37: { // LUI
        Info("LUI");

        break;
    }

    case 0x17: { // AUIPC
        Info("AUIPC");
        
        break;
    }

    case 0x6f: { // JAL
        int32_t TargetAddr = ((IR & 0x80000000)>>11) | ((IR & 0x7fe00000)>>20) | ((IR & 0x00100000)>>9) | ((IR&0x000ff000));
        if( TargetAddr & 0x00100000 ) TargetAddr |= 0xffe00000; // Sign extension.
        PC = TargetAddr - 4;
        Warning("JAL");
        Info("PC", PC + 4);
        break;
    }

    case 0x67: { // JALR
        Info("JALR");
        break;
    }

    case 0x63: { // BEQ/BNE/BLT/BGE/BLTU/BGEU
        break;
    }

    case 0x03: { // LB/LH/LW/LBU/LHU
        Info("LB");
        
        break;
    }

    case 0x23: { // SB/SH/SW
        Info("SB");

        break;
    }

    case 0x13: { // ADDI/SLTI/SLTIU/XORI/ORI/ADDI/SLLI/SRLI/SRAI
        std::uint32_t rd = RD(IR);
        std::uint32_t rs1 = RS1(IR);
        std::uint32_t func3 = FUNC3(IR);
        std::uint32_t func7 = FUNC7(IR);
        std::int64_t imm = IMM12(IR);
        imm = imm | ((imm & 0x800) ? 0xfffffffffffff000 : 0);

        if (func3 == 0x00) { // ADDI
            Registers[rd] = static_cast<std::int64_t>(Registers[rs1]) + imm;
        } else if (func3 == 0x2) { // SLTI
            Registers[rd] = static_cast<std::int64_t>(Registers[rs1]) < imm ? 1 : 0;
        } else if (func3 == 0x3) { // SLTIU
            Registers[rd] = Registers[rs1] < static_cast<std::uint64_t>(imm) ? 1 : 0;
        } else if (func3 == 0x4) { // XORI
            Registers[rd] = Registers[rs1] ^ imm;
        } else if (func3 == 0x6) { // ORI
            Registers[rd] = Registers[rs1] | imm;
        } else if (func3 == 0x7) { // ANDI
            Registers[rd] = Registers[rs1] & imm;
        } else if (func3 == 0x1) { // SLLI
            Registers[rd] = Registers[rs1] << (imm & 0x1f);
        } else if (func3 == 0x5) { // SRLI/SLAI
            if (func7 == 0x00) // SLRI
                Registers[rd] = Registers[rs1] >> (imm & 0x1f);
            else if (func7 == 0x20) // SLAI
                Registers[rd] = static_cast<uint64_t>(static_cast<std::int64_t>(Registers[rs1]) >> (imm & 0x1f));
        }

        break;
    }
    case 0x33: { // ADD/SUB/SLL/SLT/SLTU/XOR/SRL/SRA/OR/AND
        std::uint32_t rd = RD(IR);
        std::uint32_t rs1 = RS1(IR);
        std::uint32_t rs2 = RS2(IR);
        std::uint32_t func3 = FUNC3(IR);
        std::uint32_t func7 = FUNC7(IR);

        if (func3 == 0) { // ADD/SUB
            if (func7 == 0x00) // ADD
                Registers[rd] = Registers[rs1] + Registers[rs2];
            else if (func7 == 0x20) // SUB
                Registers[rd] = Registers[rs1] - Registers[rs2];
        } else if (func3 == 0x1) { // SLL
            Registers[rd] = Registers[rs1] << (Registers[rs2] & 0x1f);
        } else if (func3 == 0x2) { // SLT
            Registers[rd] = static_cast<std::int64_t>(Registers[rs1]) < static_cast<std::int64_t>(Registers[rs2]) ? 1 : 0;
        } else if (func3 == 0x3) { // SLTU
            Registers[rd] = Registers[rs1] < Registers[rs2] ? 1 : 0;
        } else if (func3 == 0x4) { // XOR
            Registers[rd] = Registers[rs1] ^ Registers[rs2];
        } else if (func3 == 0x5) { // SRL/SRA
            // https://stackoverflow.com/questions/7622/are-the-shift-operators-arithmetic-or-logical-in-c
            if (func7 == 0x00) { // SRL
                Registers[rd] = Registers[rs1] >> (Registers[rs2] & 0x1f);
            } else if (func7 == 0x20) { // SRA
                Registers[rd] = static_cast<std::uint64_t>(static_cast<std::int64_t>(Registers[rs1]) >> (Registers[rs2] & 0x1f));
            }
        } else if (func3 == 0x6) { // OR
            Registers[rd] = Registers[rs1] | Registers[rs2];
        } else if (func3 == 0x7) { // AND
            Registers[rd] = Registers[rs1] & Registers[rs2];
        }
        break;
    }

    // rv64i extension, in addition to rv32i

    // rv32m extension.

    // rv64m extension, in addition to rv32m

    // rv32a extension.

    // rv64a extension.

    // zicsr
    case 0x73: { // CSR
        Info("CSR");
        break;
    }

    default: { // INVALID
        Error(PC, "Instruction not found.", std::bitset<8>(OP(IR)));
        break;
    }
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

    // Info("Before",  std::bitset<32>((U0) | (U1 << 8) | (U2 << 16) | (U3 <<
    // 24)),
    //      "After",   std::bitset<32>((U3) | (U2 << 8) | (U1 << 16) | (U0 <<
    //      24)));
    return ((U0) | (U1 << 8) | (U2 << 16) | (U3 << 24));
}

void Cpu::LoadImage(const std::string& FileName, std::uint64_t Offset)
{
    std::ifstream File(FileName.c_str(), std::ios::binary);
    std::vector<std::uint8_t> Image(
        (std::istreambuf_iterator<char>(File)), 
        std::istreambuf_iterator<char>()); 
    std::copy(Image.begin() + Offset, Image.end(), Memory.begin());
}

void Cpu::MemoryLayout()
{
    Info("PC: ", PC, "Memory layout:");

    for (size_t i = 0; i < 1024; i++) {
        if (i && i % 16 == 0)
            std::cout << "\n";
        std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint16_t>(Memory[i]) << ' ';
    }
    std::cout << "\n";
}

void Cpu::RegisterLayout()
{
    Info("PC: ", PC, "Registers layout:");

    for (size_t i = 0; i < 16; i++) {
        std::cout << "\tRegister#" << i << ": " << Registers[i] << "\t\t"
                  << "Register#" << i + 16 << ": " << Registers[i + 16] << "\n";
    }
}

} // namespace semu
