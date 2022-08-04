#include "emulator/emulator.hpp"
#include <iostream>

int main() {
    CPUEmulator::Emulator emulator;
    CPUEmulator::Rom prog;

    prog[0x100] = (std::uint8_t)CPUEmulator::Emulator::Instructions::JMP;
    prog[0x101] = 0;
    prog[0x102] = 5;
    prog[0x103] = 0;
    prog[0x104] = 0;

    emulator.setProgramMemory(prog);

    emulator.step();

    std::cout << (int)emulator.getU16Register(CPUEmulator::Emulator::U16Registers::IP) << std::endl;
}