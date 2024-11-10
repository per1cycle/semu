#ifndef SEMU_HH
#define SEMU_HH

#include <iomanip>
#include <vector>
#include <fstream>

#include <cstdint>

namespace semu
{
class Cpu
{
// in core memory.
private:
    std::vector<std::uint8_t> Memory;
    std::uint32_t VirtualPageBase;

private:
    std::vector<std::uint64_t> Registers;
    std::uint64_t PC;

public:
    Cpu();
    Cpu(const std::vector<std::uint8_t>& Image, std::uint32_t OFfset);
    ~Cpu();

public:
    int Run();
    int Step();
    std::uint32_t Fetch();

// helper and tools for debugging.
public:
    void MemoryLayout();
    
};



} // namespace semu


#endif // SEMU_HH