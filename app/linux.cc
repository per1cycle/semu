/**
 * TODO run linux in this program.
 */
#include "semu.hh"

int main(int argc, char const *argv[])
{
    semu::Cpu cpu;
    cpu.LoadImage("Image");
    return 0;
}
