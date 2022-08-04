#include "emulator/emulator.hpp"
#include <iostream>

int main() {
    CPUEmulator::Emulator emulator;
    CPUEmulator::Program prog;

    prog[0x100] = (std::uint8_t)CPUEmulator::Emulator::Instructions::LDIW;
    prog[0x101] = 0;
    prog[0x102] = 0;
    prog[0x103] = 1;
    prog[0x104] = 60;

    emulator.setProgramMemory(prog);

    emulator.step();

    std::cout << (int)emulator.getU16Register(CPUEmulator::Emulator::WordRegisters::IP) << std::endl;
}