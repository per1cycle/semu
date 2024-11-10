#ifndef SEMU_HH
#define SEMU_HH

#include <vector>
#include <fstream>

#include <cstdint>

namespace semu
{
class Cpu
{
private:
    std::vector<std::uint64_t> Registers;
    std::uint64_t Pc;
public:
    Cpu();
    ~Cpu();

public:
    int run();
    int step();

};



} // namespace semu


#endif // SEMU_HH