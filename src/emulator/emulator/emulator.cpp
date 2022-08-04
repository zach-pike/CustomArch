#include "emulator.hpp"

#include <iostream>

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

bool Emulator::step() {
    // Load the instruction
    Instructions instruction = static_cast<Instructions>(prog[instructionPointer]);

    // Load the 2 arguments
    std::uint16_t arg0 = (prog[instructionPointer + 1] << 8) | prog[instructionPointer + 2];
    std::uint16_t arg1 = (prog[instructionPointer + 3] << 8) | prog[instructionPointer + 4];

    bool incrementInstructionPointer = true;

    switch (instruction) {
        case Instructions::LDIB: {
            U8Registers reg = (U8Registers)arg0;
            getU8Register(reg) = (std::uint8_t)arg1;
        } break;
        case Instructions::LDIW: {
            U16Registers reg = (U16Registers)arg0;
            getU16Register(reg) = arg1;
        } break;
        case Instructions::JMPI: {
            incrementInstructionPointer = false;
            instructionPointer = arg0;
        } break;
        case Instructions::JMP: {
            incrementInstructionPointer = false;
            instructionPointer = getU16Register((U16Registers)arg0);
        } break;
        case Instructions::ADDB: break;
        case Instructions::ADDW: break;
    }



    // Every instruction is 5 bytes wide
    if (incrementInstructionPointer) instructionPointer += 5;

    return true;
}