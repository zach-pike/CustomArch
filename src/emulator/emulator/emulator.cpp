#include "emulator.hpp"

#include <iostream>
#include <stdexcept>

using CPUEmulator::Emulator;
using CPUEmulator::Rom;

std::uint8_t& Emulator::getU8Register(U8Registers reg) {
    switch (reg) {
        case U8Registers::AL: {
            return *(std::uint8_t*)&AReg;
        } break;
        case U8Registers::AH: {
            return *(std::uint8_t*)(&AReg+1);
        } break;
        case U8Registers::BL: {
            return *(std::uint8_t*)&BReg;
        } break;
        case U8Registers::BH: {
            return *(std::uint8_t*)(&BReg+1);
        } break;
        case U8Registers::CL: {
            return *(std::uint8_t*)&CReg;
        } break;
        case U8Registers::CH: {
            return *(std::uint8_t*)(&CReg+1);
        } break;
        case U8Registers::DL: {
            return *(std::uint8_t*)&DReg;
        } break;
        case U8Registers::DH: {
            return *(std::uint8_t*)(&DReg+1);
        } break;
        case U8Registers::X: {
            return XReg;
        } break;
        case U8Registers::Y: {
            return YReg;
        } break;
        default: {
            std::cerr << (int)reg << std::endl;
            throw std::runtime_error("Could not identify U8 register");
        } break;
    }
}

std::uint16_t& Emulator::getU16Register(U16Registers reg) {
    switch (reg) {
        case U16Registers::A: {
            return AReg;
        } break;
        case U16Registers::B: {
            return BReg;
        } break;
        case U16Registers::C: {
            return CReg;
        } break;
        case U16Registers::D: {
            return DReg;
        } break;
        case U16Registers::SP: {
            return stackPointer;
        } break;
        case U16Registers::IP: {
            return instructionPointer;
        } break;
        default: {
            std::cerr << (int)reg << std::endl;
            throw std::runtime_error("Could not identify U16 register");
        } break;
    }
}

void Emulator::stackPushU8(std::uint8_t value) {
    prog[stackPointer] = value;
    stackPointer -= 1;
}

void Emulator::stackPushU16(std::uint16_t value) {
    prog[stackPointer] = (value >> 8) & 0xff;
    prog[stackPointer - 1] = value & 0xff;
    stackPointer -= 2;
}

std::uint8_t Emulator::stackPopU8() {
    stackPointer += 1;
    return prog[stackPointer];
}

std::uint16_t Emulator::stackPopU16() {
    stackPointer += 2;
    std::uint16_t lobyte = prog[stackPointer - 1];
    std::uint16_t hibyte = prog[stackPointer];

    return (hibyte << 8) | lobyte;
}

void Emulator::setProgramMemory(Rom _prog) {
    prog = _prog;
}

Rom& Emulator::getProgramRom() {
    return prog;
}

void Emulator::setFlag(FlagRegister flag, bool value) {
    if (value) {
        flagsRegister |= 0b1 << (int)flag;
    } else {
        flagsRegister &= ~(0b1 << (int)flag);
    }
}

bool Emulator::getFlag(FlagRegister flag) const {
    return (flagsRegister >> (int)flag) & 0x1;
}

std::uint8_t Emulator::fullAdd(std::uint8_t v1, std::uint8_t v2) {
    std::uint8_t carry = getFlag(FlagRegister::CARRY_FLAG);

    using b = std::uint8_t;
        
    b out = 0;

    for (int i=0; i<8; i++) {
        b in1 = (v1 >> i) & 0b1;
        b in2 = (v2 >> i) & 0b1;

        b sum1   = in1 ^ in2;
        b carry1 = in1 & in2;

        b sum2 = sum1 ^ carry;
        b carry2 = sum1 & carry;

        carry = carry1 | carry2;

        if (sum2) out |= (0b1 << i);
    }

    setFlag(FlagRegister::CARRY_FLAG, carry);
    return out;
}

bool Emulator::step() {
    // Load the instruction
    Instructions instruction = static_cast<Instructions>(prog[instructionPointer]);

    // Load the 2 arguments
    std::uint16_t arg0 = (prog[instructionPointer + 1] << 8) | prog[instructionPointer + 2];
    std::uint16_t arg1 = (prog[instructionPointer + 3] << 8) | prog[instructionPointer + 4];

    bool incrementInstructionPointer = true;
    bool returnValue = true;

    switch (instruction) {
        case Instructions::ADD: {
            std::uint8_t v1 = getU8Register((U8Registers)arg0);
            std::uint8_t v2 = getU8Register((U8Registers)arg1);

            std::uint8_t res = fullAdd(v1, v2);
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
            getU16Register(U16Registers::A) = res;
        } break;
        case Instructions::ADDW: {
            std::uint16_t v1 = getU16Register((U16Registers)arg0);
            std::uint16_t v2 = getU16Register((U16Registers)arg1);

            std::uint8_t lores = fullAdd(v1 & 0xff, v2 & 0xff);
            std::uint8_t hires = fullAdd((v1 >> 8) & 0xff, (v2 >> 8) & 0xff);

            std::uint16_t res = (hires << 8) | lores;

            setFlag(FlagRegister::ZERO_FLAG, res == 0);
            getU16Register(U16Registers::A) = res;
        } break;
        case Instructions::ADDI: {
            std::uint8_t v1 = getU8Register((U8Registers)arg0);
            std::uint8_t v2 = arg1;

            std::uint8_t res = fullAdd(v1, v2);
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
            getU16Register(U16Registers::A) = res;
        } break;
        case Instructions::ADDIW: {
            std::uint16_t v1 = getU16Register((U16Registers)arg0);
            std::uint16_t v2 = arg1;

            std::uint8_t lores = fullAdd(v1 & 0xff, v2 & 0xff);
            std::uint8_t hires = fullAdd((v1 >> 8) & 0xff, (v2 >> 8) & 0xff);

            std::uint16_t res = (hires << 8) | lores;

            setFlag(FlagRegister::ZERO_FLAG, res == 0);
            getU16Register(U16Registers::A) = res;
        } break;

        case Instructions::JMP: {
            incrementInstructionPointer = false;
            instructionPointer = getU16Register((U16Registers)arg0);
        } break;

        case Instructions::JMPI: {
            incrementInstructionPointer = false;
            instructionPointer = arg0;
        } break;

        case Instructions::JEQ: {
            if (getFlag(FlagRegister::ZERO_FLAG) == true) {
                incrementInstructionPointer = false;
                instructionPointer = getU16Register((U16Registers)arg0);
                setFlag(FlagRegister::ZERO_FLAG, false);
            }
        } break;
        case Instructions::JEQI: {
            if (getFlag(FlagRegister::ZERO_FLAG) == true) {
                incrementInstructionPointer = false;
                instructionPointer = arg0;
                setFlag(FlagRegister::ZERO_FLAG, false);
            }
        } break;

        case Instructions::JNE: {
            if (getFlag(FlagRegister::ZERO_FLAG) == false) {
                incrementInstructionPointer = false;
                instructionPointer = getU16Register((U16Registers)arg0);
            }
        } break;
        case Instructions::JNEI: {
            if (getFlag(FlagRegister::ZERO_FLAG) == false) {
                incrementInstructionPointer = false;
                instructionPointer = arg0;
            }
        } break;

        case Instructions::JLT: {
            if (getFlag(FlagRegister::LESS_THAN_FLAG) == true) {
                incrementInstructionPointer = false;
                instructionPointer = getU16Register((U16Registers)arg0);
                setFlag(FlagRegister::LESS_THAN_FLAG, false);
            }
        } break;
        case Instructions::JLTI: {
            if (getFlag(FlagRegister::LESS_THAN_FLAG) == true) {
                incrementInstructionPointer = false;
                instructionPointer = arg0;
                setFlag(FlagRegister::LESS_THAN_FLAG, false);
            }
        } break;

        case Instructions::JGT: {
            if (getFlag(FlagRegister::LESS_THAN_FLAG) == false) {
                incrementInstructionPointer = false;
                instructionPointer = getU16Register((U16Registers)arg0);
            }
        } break;
        case Instructions::JGTI: {
            if (getFlag(FlagRegister::LESS_THAN_FLAG) == false) {
                incrementInstructionPointer = false;
                instructionPointer = arg0;
            }
        } break;

        case Instructions::PUSH: {
            U8Registers reg = (U8Registers)arg0;
            std::uint8_t val = getU8Register(reg);

            stackPushU8(val);
        } break;
        case Instructions::PUSHW: {
            U16Registers reg = (U16Registers)arg0;
            std::uint16_t val = getU16Register(reg);

            stackPushU16(val);
        } break;
        case Instructions::POP: {
            U8Registers reg = (U8Registers)arg0;
            getU8Register(reg) = stackPopU8();
        } break;
        case Instructions::POPW: {
            U16Registers reg = (U16Registers)arg0;
            getU16Register(reg) = stackPopU16();
        } break;

        case Instructions::MOV: {
            U8Registers reg1 = (U8Registers)arg0;
            U8Registers reg2 = (U8Registers)arg1;

            getU8Register(reg1) = getU8Register(reg2);
        } break;
        case Instructions::MOVW: {
            U16Registers reg1 = (U16Registers)arg0;
            U16Registers reg2 = (U16Registers)arg1;

            getU16Register(reg1) = getU16Register(reg2);
        } break;
        
        case Instructions::LDI: {
            U8Registers reg = (U8Registers)arg0;
            getU8Register(reg) = (std::uint8_t)arg1;
        } break;
        case Instructions::LDIW: {
            U16Registers reg = (U16Registers)arg0;
            getU16Register(reg) = arg1;
        } break;

        case Instructions::LDFA: {
            std::uint16_t address = getU16Register((U16Registers)arg1);
            getU8Register((U8Registers)arg0) = prog[address];
        } break;
        case Instructions::LDFAW: {
            std::uint16_t address = getU16Register((U16Registers)arg1);
            getU16Register((U16Registers)arg0) = (prog[address] << 8) | prog[address + 1];
        } break;
        
        case Instructions::LDFAI: {
            getU8Register((U8Registers)arg0) = prog[arg1];
        } break;
        case Instructions::LDFAIW: {
            getU16Register((U16Registers)arg0) = (prog[arg0] << 8) | prog[arg0 + 1];
        } break;

        case Instructions::STA: {
            std::uint16_t address = getU16Register((U16Registers)arg0);
            prog[address] = getU8Register((U8Registers)arg1);
        } break;
        case Instructions::STAW: {
            std::uint16_t address = getU16Register((U16Registers)arg0);
            std::uint16_t value = getU16Register((U16Registers)arg1);
            prog[address] = (value >> 8) & 0xff;
            prog[address + 1] = value & 0xff;
        } break;
        case Instructions::STAI: {
            std::uint16_t address = getU16Register((U16Registers)arg0);
            std::uint8_t  value   = arg1;

            prog[address] = value;
        } break;
        case Instructions::STAIW: {
            std::uint16_t address = getU16Register((U16Registers)arg0);
            std::uint16_t value   = arg1;

            prog[address] = (value >> 8) & 0xff;
            prog[address + 1] = value & 0xff;
        } break;

        case Instructions::CMP: {
            std::uint8_t reg1 = getU8Register((U8Registers)arg0);
            std::uint8_t reg2 = getU8Register((U8Registers)arg1);

            setFlag(FlagRegister::ZERO_FLAG, (reg1 - reg2) == 0);
            setFlag(FlagRegister::LESS_THAN_FLAG, reg1 < reg2);
        } break;
        case Instructions::CMPW: {
            std::uint16_t reg1 = getU16Register((U16Registers)arg0);
            std::uint16_t reg2 = getU16Register((U16Registers)arg1);

            setFlag(FlagRegister::ZERO_FLAG, (reg1 - reg2) == 0);
            setFlag(FlagRegister::LESS_THAN_FLAG, reg1 < reg2);
        } break;
        case Instructions::CMPI: {
            std::uint8_t reg1 = getU8Register((U8Registers)arg0);
            std::uint8_t reg2 = arg1;

            setFlag(FlagRegister::ZERO_FLAG, (reg1 - reg2) == 0);
            setFlag(FlagRegister::LESS_THAN_FLAG, reg1 < reg2);
        } break;
        case Instructions::CMPIW: {
            std::uint16_t reg1 = getU16Register((U16Registers)arg0);
            std::uint16_t reg2 = arg1;

            setFlag(FlagRegister::ZERO_FLAG, (reg1 - reg2) == 0);
            setFlag(FlagRegister::LESS_THAN_FLAG, reg1 < reg2);
        } break;

        // Halt emulator
        case Instructions::HLT: returnValue = false; break;

        // Bitwise
        // AND
        case Instructions::AND: {
            std::uint8_t val1 = getU8Register((U8Registers)arg0);
            std::uint8_t val2 = getU8Register((U8Registers)arg1);

            std::uint8_t res = val1 & val2;

            getU16Register(U16Registers::A) = res;
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
        } break;
        case Instructions::ANDW: {
            std::uint16_t val1 = getU16Register((U16Registers)arg0);
            std::uint16_t val2 = getU16Register((U16Registers)arg1);

            std::uint16_t res = val1 & val2;

            getU16Register(U16Registers::A) = res;
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
        } break;
        case Instructions::ANDI: {
            std::uint8_t val1 = getU8Register((U8Registers)arg0);
            std::uint8_t val2 = (uint8_t)arg1;

            std::uint8_t res = val1 & val2;

            getU16Register(U16Registers::A) = res;
            setFlag(FlagRegister::ZERO_FLAG, res == 0);

        } break;
        case Instructions::ANDIW: {
            std::uint16_t val1 = getU16Register((U16Registers)arg0);
            std::uint16_t val2 = arg1;

            std::uint16_t res = val1 & val2;

            getU16Register(U16Registers::A) = res;
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
        } break;
        
        // OR
        case Instructions::OR: {
            std::uint8_t val1 = getU8Register((U8Registers)arg0);
            std::uint8_t val2 = getU8Register((U8Registers)arg1);

            std::uint8_t res = val1 | val2;

            getU16Register(U16Registers::A) = res;
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
        } break;
        case Instructions::ORW: {
            std::uint16_t val1 = getU16Register((U16Registers)arg0);
            std::uint16_t val2 = getU16Register((U16Registers)arg1);

            std::uint16_t res = val1 | val2;

            getU16Register(U16Registers::A) = res;
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
        } break;
        case Instructions::ORI: {
            std::uint8_t val1 = getU8Register((U8Registers)arg0);
            std::uint8_t val2 = arg1;

            std::uint8_t res = val1 | val2;
            
            getU16Register(U16Registers::A) = res;
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
        } break;
        case Instructions::ORIW: {
            std::uint16_t val1 = getU16Register((U16Registers)arg0);
            std::uint16_t val2 = getU16Register((U16Registers)arg1);

            std::uint16_t res = val1 | val2;

            getU16Register(U16Registers::A) = res;
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
        } break;

        // Invert
        case Instructions::INV: {
            std::uint8_t val1 = getU8Register((U8Registers)arg0);

            std::uint8_t res = ~val1;

            getU16Register(U16Registers::A) = res;
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
        } break;
        case Instructions::INVW: {
            std::uint16_t val1 = getU16Register((U16Registers)arg0);

            std::uint16_t res = ~val1;

            getU16Register(U16Registers::A) = res;
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
        } break;

        // XOR
        case Instructions::XOR: {
            std::uint8_t val1 = getU8Register((U8Registers)arg0);
            std::uint8_t val2 = getU8Register((U8Registers)arg1);

            std::uint8_t res = val1 ^ val2;

            getU16Register(U16Registers::A) = res;
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
        } break;
        case Instructions::XORW: {
            std::uint16_t val1 = getU16Register((U16Registers)arg0);
            std::uint16_t val2 = getU16Register((U16Registers)arg1);

            std::uint16_t res = val1 ^ val2;

            getU16Register(U16Registers::A) = res;
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
        } break;
        case Instructions::XORI: {
            std::uint8_t val1 = getU8Register((U8Registers)arg0);
            std::uint8_t val2 = arg1;

            std::uint8_t res = val1 ^ val2;

            getU16Register(U16Registers::A) = res;
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
        } break;
        case Instructions::XORIW: {
            std::uint16_t val1 = getU16Register((U16Registers)arg0);
            std::uint16_t val2 = arg1;

            std::uint16_t res = val1 ^ val2;

            getU16Register(U16Registers::A) = res;
            setFlag(FlagRegister::ZERO_FLAG, res == 0);
        } break;
        case Instructions::REGADD: {
            U8Registers reg = (U8Registers)arg0;

            getU8Register(reg) += (std::uint8_t)arg1;
        } break;
        case Instructions::REGADDW: {
            U16Registers reg = (U16Registers)arg0;

            getU16Register(reg) += arg1;
        } break;

        case Instructions::INY: {
          getU8Register(U8Registers::Y)++;
        } break;
        case Instructions::INX: {
          getU8Register(U8Registers::X)++;
        } break;
    }

    // Every instruction is 5 bytes wide
    if (incrementInstructionPointer) instructionPointer += 5;

    return returnValue;
}