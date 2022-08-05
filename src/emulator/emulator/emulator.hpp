#pragma once

#include <cstdint>
#include "rom/rom.hpp"

// 
namespace CPUEmulator {
class Emulator {
public:
    enum class Instructions {
        // Add
        ADD,   // Add a value from two 8-Bit registers and put the value in the 16-Bit register A
        ADDW,  // Add a value from two 16-Bit registers and put the value in the 16-Bit register A

        ADDI,  // Add a register(8-Bit) and a immediate(8-Bit) and store the result in the A register.
        ADDIW, // Add a register(16-Bit) and a immediate(16-Bit) and store the result in the A register.

        // Jump
        JMP,   // Takes a 16-Bit register and jumps to the location specified
        JMPI,  // Takes a 16-Bit address and jumps to the location specified

        JEQ,   // Jump if equal to a address stored in a 16-Bit register
        JNE,   // Jump if not equal to a address stored in a 16-Bit register

        JEQI,  // Jump if equal to a immediate address
        JNEI,  // Jump if not equal to a immediate address

        // Stack push/pop
        PUSH,  // Push a 8-Bit value on the stack
        PUSHW, // Push a 16-bit value on the stack

        POP,   // Pop a 8-Bit value off the stack
        POPW,  // Pop a 16-Bit value off the stack

        // Move
        MOV,   // Move a 8-Bit value between registers
        MOVW,  // Move a 16-Bit value between registers

        // Load immediate
        LDI,   // Load a immediate byte into a 8-Bit register
        LDIW,  // Load a immediate word into a 16-Bit register

        // Load from address
        LDFA,  // Load a value by derefrenceing a 16-Bit register and loading that value into the specified 8-Bit register
        LDFAW, // Load a value by derefrenceing a 16-Bit register and loading that value into the specified 16-Bit register

        LDFAI,   // Load a value by derefrenceing a 16-Bit immediate and loading that byte into a 8-Bit register
        LDFAIW,  // Load a value by derefrenceing a 16-Bit immediate and loading thoes bytes into a 16-Bit register

        // Compare instructions
        CMP,     // Compare 2 8-Bit registers and sets the zero flag
        CMPW,    // Compare 2 16-Bit registers and sets the zero flag

        // Halt
        HLT,     // Send the halt command

        // Bitwise instructions
        // And instructions
        AND,
        ANDW,
        ANDI,
        ANDIW,

        // Or instruction
        OR,
        ORW,
        ORI,
        ORIW,

        // Invert
        INV,
        INVW,

        // XOR
        XOR,
        XORW,
        XORI,
        XORIW,
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

    enum class FlagRegister {
        CARRY_FLAG,
        ZERO_FLAG
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

    // Flags register
    std::uint8_t flagsRegister = 0;
public:
    // Stack functions
    void stackPushU8(std::uint8_t value);
    void stackPushU16(std::uint16_t value);
    std::uint8_t stackPopU8();
    std::uint16_t stackPopU16();

    std::uint8_t& getU8Register(U8Registers reg);
    std::uint16_t& getU16Register(U16Registers reg);

    void setFlag(FlagRegister flag, bool value);
    bool getFlag(FlagRegister flag) const;

    void setProgramMemory(Rom _prog);
    Rom& getProgramRom();

    bool step();
};
}