#include "semu.hh"

#include <bitset>

namespace semu {
Cpu::Cpu()
{
    Info("Cpu initing...");
    Logo();

    PC = 0;
    Registers.resize(32, 0);
    Memory.resize(MEM_SIZE, 0);
    VirtualPageBase = 0x80000000;
    Info("Register and memory initialized finished.");
}

Cpu::Cpu(const std::vector<std::uint8_t>& Image, std::uint32_t Offset)
{
    Info("Cpu initing...");
    Logo();
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

void Cpu::Logo()
{
    // http://www.patorjk.com/software/taag/#p=display&f=Big&t=Semu
    std::string Logo = 
    "\n\
     _____                      \n\
    / ____|                     \n\
    | (___   ___ _ __ ___  _   _ \n\
    \\___ \\ / _ \\ '_ ` _ \\| | | |\n\
     ____) |  __/ | | | | | |_| |\n\
    |_____/ \\___|_| |_| |_|\\__,_|\n\
                                \n\
                                ";
    std::cout << Logo << std::endl;
}

int Cpu::Run()
{
    for (size_t i = 0; i < 256; i++) {
        int Result = Step();
        Info(PC);
        // RegisterLayout();
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
    // and Chapter 37 in riscv manual 2024/04

    switch (OPCode) {
    // https://stackoverflow.com/questions/52574537/risc-v-pc-absolute-vs-pc-relative
    // rv32i base
    case 0x37: { // LUI
        Info("LUI");
        std::uint32_t rd = RD(IR);
        Registers[rd] = static_cast<std::uint64_t>(GET_BIT_FROM(IR, 12, 31) << 12);
        break;
    }

    case 0x17: { // AUIPC
        // Info("AUIPC");
        std::uint32_t rd = RD(IR);
        std::uint32_t imm = static_cast<std::int64_t>(GET_BIT_FROM(IR, 12, 31) << 12);
        Registers[rd] = PC + imm;
        break;
    }

    case 0x6f: { // JAL
        // std::int32_t TargetAddr = ((IR & 0x80000000)>>11) | ((IR & 0x7fe00000)>>20) | ((IR & 0x00100000)>>9) | ((IR&0x000ff000));
        std::int32_t TargetAddr = (
                                    (GET_BIT_FROM(IR, 21, 30) << 1) | 
                                    (GET_BIT_FROM(IR, 20, 20) << 11) |
                                    (GET_BIT_FROM(IR, 12, 19) << 12) |
                                    (GET_BIT_FROM(IR, 31, 31) << 20));
                                    
        if ( TargetAddr & 0x00100000 ) TargetAddr |= 0xffe00000; // Sign extension.
        // if ( TargetAddrFoo & 0x00100000 ) TargetAddrFoo |= 0xffe00000; // Sign extension.

        // if (TargetAddr != TargetAddrFoo)
        //  Error("Error in get bit!", 
        //     "PC",
        //     PC,
        //     "IR",
        //     IR,
        //     std::bitset<32>(TargetAddr),
        //     std::bitset<32>(TargetAddrFoo));
        PC = TargetAddr - 4;
        Warning("JAL");
        Info("PC", PC + 4);
        break;
    }

    case 0x67: { // JALR
 
        break;
    }

    case 0x63: { // BEQ/BNE/BLT/BGE/BLTU/BGEU
        // std::uint32_t imm1 = (IR >> 8) & 
        std::uint32_t op = OP(IR);
        std::uint32_t rs1 = RS1(IR);
        std::uint32_t rs2 = RS2(IR);
        std::uint32_t func3 = FUNC3(IR);
        // get imm
        std::uint32_t imm = GET_BIT_FROM(IR, 8, 11) << 1             // 1-4
                            | (GET_BIT_FROM(IR, 25, 30) << 5)   // 5-10
                            | (GET_BIT_FROM(IR, 7, 7) << 11)    // 11
                            | (GET_BIT_FROM(IR, 31, 31) << 12); // 12

        if (func3 == 0x0 && Registers[rs1] == Registers[rs2]) { // BEQ
            PC = imm;
        } else if (func3 == 0x1 && Registers[rs1] != Registers[rs2]) { // BNE
            PC = imm;
        } else if (func3 ==  0x4 && static_cast<std::int64_t>(Registers[rs1]) < static_cast<std::int64_t>(Registers[rs2]) ) { // BLT
            PC = imm;
        } else if (func3 == 0x5 && static_cast<std::int64_t>(Registers[rs1]) >= static_cast<std::int64_t>(Registers[rs2])) { // BGE
            PC = imm;
        } else if (func3 == 0x6 && Registers[rs1] < Registers[rs2]) { // BLTU
            PC = imm;
        } else if (func3 == 0x7 && Registers[rs1] >= Registers[rs2]) {  // BGEU
            PC = imm;
        }

        break;
    }

    case 0x03: { // LB/LH/LW/LBU/LHU
        std::uint32_t func3 = FUNC3(IR);
        
        if (func3 == 0x0) { // LB

        } else if (func3 == 0x1) { // LH

        } else if (func3 == 0x2) { // LW

        } else if (func3 == 0x4) { // LBU

        } else if (func3 == 0x5) { // LHU

        }

        break;
    }

    case 0x23: { // SB/SH/SW
        Info("SB Not implemented");
        std::uint32_t func3 = FUNC3(IR);
        
        if (func3 == 0x0) { // SB

        } else if (func3 == 0x1) { // SH

        } else if (func3 == 0x2) { // SW

        }

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

        // rv32m extension.

        break;
    }

    case 0x0f: { // Fence
        Info("Fence not implemented");
        break;
    }
    // rv64i extension, in addition to rv32i
    
    // rv64m extension, in addition to rv32m

    // rv32a extension.

    // rv64a extension.

    // zicsr
    case 0x73: { // CSR
        Info("CSR Not implemented.");
        break;
    }

    default: { // INVALID
        Error(PC, "Instruction not found.", std::bitset<8>(OP(IR)));
        return -1;
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

const std::uint64_t& Cpu::GetPC() const { return PC; }

const std::vector<std::uint64_t>& Cpu::GetRegs() const { return Registers; }
// optimize for large memcpy.
const std::vector<std::uint8_t>& Cpu::GetMem() const { return Memory; }

// TODO move to utils.
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
