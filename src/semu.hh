#ifndef SEMU_HH
#define SEMU_HH

#include <iomanip>
#include <vector>
#include <fstream>
#include <iostream>

#include <cstdint>

namespace semu
{
class Cpu
{
// in core memory.
private:
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

// core functions.
public:
    int Run();
    int Step();
    std::uint32_t Fetch();
    
// helper and tools for debugging.
public:
    void MemoryLayout();
    template<typename T, typename...Ts>
    void Info(T&& Message, Ts&& ... Further){
        if constexpr (sizeof...(Ts) == 0)
        {
            std::cout << "                                                                                                      [INFO]: " << Message << "\n";
        }
        else 
        {
            std::cout << "[INFO]: " << Message << "\n";
            Info(std::forward<Ts>(Further)...);
        }
    }
};

} // namespace semu


#endif // SEMU_HH