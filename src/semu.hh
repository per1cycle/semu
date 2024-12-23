#ifndef SEMU_HH
#define SEMU_HH

#define MEM_SIZE (64 * 1024 * 1024)

// get ir from low to high. [LOW, HIGH]
#define GET_BIT_FROM(IR, LOW, HIGH) (((IR) >> (LOW)) & ((1 << ((HIGH) - (LOW) + 1)) - 1))

#define OP(IR) ((IR)&0x7f)
#define RD(IR) (((IR) >> 7) & 0x1f)
#define RS1(IR) (((IR) >> 15) & 0x1f)
#define RS2(IR) (((IR) >> 20) & 0x1f)
#define FUNC3(IR) (((IR) >> 12) & 0x7)
#define FUNC7(IR) (((IR) >> 25) & 0x3f)
#define IMM12(IR) (((IR) >> 20) & 0xfff)

#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include <cstdint>

namespace semu {

class Cpu {
    // in core memory.
private:
    // TODO use std::share_ptr<Memory> and put memory outside.
    std::vector<std::uint8_t> Memory;
    std::uint32_t VirtualPageBase;

    // registers
    // register alias name https://en.wikichip.org/wiki/risc-v/registers
private:
    std::vector<std::uint64_t> Registers;
    std::uint64_t PC;

public:
    Cpu();
    Cpu(const std::vector<std::uint8_t>& Image, std::uint32_t OFfset);
    ~Cpu();
    void Logo();
    // core functions.
public:
    int Run();
    int Step();
    std::uint32_t Fetch();

// getter.
public:
    const std::uint64_t& GetPC() const;
    const std::vector<std::uint64_t>& GetRegs() const;
    // optimize for large memcpy.
    const std::vector<std::uint8_t>& GetMem() const;
// helper and tools for debugging.
public:
    void LoadImage(const std::string& FileName, std::uint64_t Offset);
    void RegisterLayout();
    void MemoryLayout();

    template <typename T, typename... Ts>
    void Info(T&& Message, Ts&&... Further)
    {
        if constexpr (sizeof...(Ts) == 0) {
            std::cout << "[\033[1;32mINFO\033[1;37m]: " << Message << "\n";
        } else {
            std::cout << "[\033[1;32mINFO\033[1;37m]: " << Message << "\n";
            Info(std::forward<Ts>(Further)...);
        }
    }

    template <typename T, typename... Ts>
    void Warning(T&& Message, Ts&&... Further)
    {
        if constexpr (sizeof...(Ts) == 0) {
            std::cout << "[\033[1;33mWARNING\033[1;37m]: " << Message << "\n";
        } else {
            std::cout << "[\033[1;33mWARNING\033[1;37m]: " << Message << "\n";
            Info(std::forward<Ts>(Further)...);
        }
    }

    template <typename T, typename... Ts>
    void Error(T&& Message, Ts&&... Further)
    {
        if constexpr (sizeof...(Ts) == 0) {
            std::cout << "[\033[1;31mERROR\033[1;37m]: " << Message << "\n";
        } else {
            std::cout << "[\033[1;31mERROR\033[1;37m]: " << Message << "\n";
            Error(std::forward<Ts>(Further)...);
        }
    }
};

} // namespace semu

#endif // SEMU_HH