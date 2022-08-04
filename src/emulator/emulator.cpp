#include "emulator.hpp"

#include <iostream>

using CPUEmulator::Emulator;
using CPUEmulator::Program;

std::uint8_t& Emulator::getU8Register(ByteRegisters reg) {
    switch (reg) {
        case ByteRegisters::AL: {
            return *(std::uint8_t*)&AReg;
        } break;
        case ByteRegisters::AH: {
            return *(std::uint8_t*)(&AReg+1);
        } break;
        case ByteRegisters::BL: {
            return *(std::uint8_t*)&BReg;
        } break;
        case ByteRegisters::BH: {
            return *(std::uint8_t*)(&BReg+1);
        } break;
        case ByteRegisters::CL: {
            return *(std::uint8_t*)&CReg;
        } break;
        case ByteRegisters::CH: {
            return *(std::uint8_t*)(&CReg+1);
        } break;
        case ByteRegisters::DL: {
            return *(std::uint8_t*)&DReg;
        } break;
        case ByteRegisters::DH: {
            return *(std::uint8_t*)(&DReg+1);
        } break;
        case ByteRegisters::X: {
            return XReg;
        } break;
        case ByteRegisters::Y: {
            return YReg;
        } break;
    }
}

std::uint16_t& Emulator::getU16Register(WordRegisters reg) {
    switch (reg) {
        case WordRegisters::A: {
            return AReg;
        } break;
        case WordRegisters::B: {
            return BReg;
        } break;
        case WordRegisters::C: {
            return CReg;
        } break;
        case WordRegisters::D: {
            return DReg;
        } break;
        case WordRegisters::SP: {
            return stackPointer;
        } break;
        case WordRegisters::IP: {
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

void Emulator::setProgramMemory(Program _prog) {
    prog = _prog;
}

Program& Emulator::getProgramRom() {
    return prog;
}

bool Emulator::step() {
    // Load the instruction
    Instructions instruction = static_cast<Instructions>(prog[instructionPointer]);

    // Load the 2 arguments
    std::uint16_t arg0 = (prog[instructionPointer + 1] << 8) | prog[instructionPointer + 2];
    std::uint16_t arg1 = (prog[instructionPointer + 3] << 8) | prog[instructionPointer + 4];

    switch (instruction) {
        case Instructions::LDIB: {
            ByteRegisters reg = (ByteRegisters)arg0;
            getU8Register(reg) = (std::uint8_t)arg1;
        } break;
        case Instructions::LDIW: {
            WordRegisters reg = (WordRegisters)arg0;
            getU16Register(reg) = arg1;
        } break;
    }



    // Every instruction is 5 bytes wide
    instructionPointer += 5;

    return true;
}