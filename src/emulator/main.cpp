#include "emulator/emulator.hpp"
#include <iostream>

#include <type_traits>

// #include "assembler/asmb/asmb_core.hpp"

int main() {
    CPUEmulator::Emulator emulator;
    CPUEmulator::Rom prog;

    prog[0x100] = (std::uint8_t)CPUEmulator::Emulator::Instructions::JMP;
    prog[0x102] = 5;

    emulator.setProgramMemory(prog);

    // while();

    emulator.step();

    // Should print out 256 (Proves that the instruction pointer got set to itsself at the jmp instruction)
    std::cout << (int)emulator.getU16Register(CPUEmulator::Emulator::U16Registers::IP) << std::endl;
}