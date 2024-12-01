#include <semu.hh>
#include <iostream>
#include <bitset>

int main(int argc, char const *argv[])
{
    std::uint32_t test = GET_BIT_FROM(0b00000101000000000000000001101111, 21, 30);
    std::cout << std::bitset<32>(test); 
    
    return 0;
}
