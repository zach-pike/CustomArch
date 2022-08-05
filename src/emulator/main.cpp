#include "emulator/emulator.hpp"
#include <iostream>

// #include "assembler/asmb/asmb_core.hpp"

int main() {
    CPUEmulator::Emulator emulator;
    CPUEmulator::Rom prog;
    emulator.setFlag(CPUEmulator::Emulator::FlagRegister::CARRY_FLAG, 1);

    // prog[0x100] = (std::uint8_t)CPUEmulator::Emulator::Instructions::HLT;

    // emulator.setProgramMemory(prog);

    // while(emulator.step());

    // Should print out 256 (Proves that the instruction pointer got set to itsself at the jmp instruction)
    // std::cout << (int)emulator.getU16Register(CPUEmulator::Emulator::U16Registers::IP) << std::endl;


}