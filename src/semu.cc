#include "semu.hh"

#include <iostream>

namespace semu
{
    Cpu::Cpu()
    {
        std::cout << "[INFO]: Cpu initing." << std::endl;
        Pc = 0;
        Registers.resize(32);
        std::cout << "Registers size: " << Registers.size() << std::endl;
    }

    Cpu::~Cpu()
    {
        std::cout << "[INFO]: Power off." << std::endl;
    }

    int Cpu::run()
    {
        try
        {
            int ret = step();
            
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }

    int Cpu::step()
    {
        Pc += 8;
    }

} // namespace semu
