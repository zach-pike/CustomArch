#pragma once

#include <cstdint>
#include "rom/rom.hpp"

// 
namespace CPUEmulator {
class Emulator {
public:
    enum class Instructions {
        LDI,   // Load a immediate byte into a 8-Bit register
        LDIW,  // Load a immediate word into a 16-Bit register

        ADD,   // Add a value from two 8-Bit registers and put the value in the 16-Bit register A
        ADDW,  // Add a value from two 16-Bit registers and put the value in the 16-Bit register A

        JMP,   // Takes a 16-Bit register and jumps to the location specified
        JMPI,  // Takes a 16-Bit address and jumps to the location specified

        PUSH,  // Push a 8-Bit value on the stack
        PUSHW, 
    };

    enum class U16Registers {
        A,
        B,
        C,
        D,
        SP,
        IP
    };

    enum class U8Registers {
        AL,
        AH,
        BL,
        BH,
        CL,
        CH,
        DL,
        DH,
        X,
        Y
    };

    Rom prog;
private:
    std::uint16_t AReg = 0;
    std::uint16_t BReg = 0;
    std::uint16_t CReg = 0;
    std::uint16_t DReg = 0;

    std::uint8_t XReg = 0;
    std::uint8_t YReg = 0;

    // Stack begins at 65535 in memory and extends down 
    std::uint16_t stackPointer = 0xffff;

    // Instruction pointer
    std::uint16_t instructionPointer = 0x100;

public:
    // Stack functions
    void stackPushU8(std::uint8_t value);
    void stackPushU16(std::uint16_t value);
    std::uint8_t stackPopU8();
    std::uint16_t stackPopU16();

    std::uint8_t& getU8Register(U8Registers reg);
    std::uint16_t& getU16Register(U16Registers reg);

    void setProgramMemory(Rom _prog);
    Rom& getProgramRom();

    bool step();
};
}