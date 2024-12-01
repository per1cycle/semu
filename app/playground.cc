#include <semu.hh>
#include <iostream>
#include <bitset>

int main(int argc, char const *argv[])
{
    std::uint32_t ir = 0b00000101000000000000000001101111;
    std::uint32_t test = GET_BIT_FROM(ir, 20, 30);
    std::uint32_t ans = (ir & 0x7fe00000)>>20;
    std::cout << std::bitset<32>(test) << ' ' << std::bitset<32>(ans) << '\n'; 

    
    return 0;
}
