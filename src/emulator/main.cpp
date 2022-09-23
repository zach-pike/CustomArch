#include "emulator/emulator.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>

#include <type_traits>

// #include "assembler/asmb/asmb_core.hpp"

std::vector<uint8_t> readFile(const char* filename) {
    // open the file:
    std::streampos fileSize;
    std::ifstream file(filename, std::ios::binary);

    // get its size:
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data:
    std::vector<uint8_t> fileData(fileSize);
    file.read((char*) &fileData[0], fileSize);
    return fileData;
}


int main() {
    CPUEmulator::Emulator emulator;
    CPUEmulator::Rom prog;

    auto bytes = readFile("/home/runner/MyArch/Compiler/prgm.bin");
    prog.readFromBytes(bytes);
  
    emulator.setProgramMemory(prog);

    // while(emulator.step()) {
    //   int A = emulator.getU16Register(CPUEmulator::Emulator::U16Registers::A);
    //   int B = emulator.getU16Register(CPUEmulator::Emulator::U16Registers::B);
    //   int C = emulator.getU16Register(CPUEmulator::Emulator::U16Registers::C);

    //   int X = emulator.getU8Register(CPUEmulator::Emulator::U8Registers::X);
    //   int Y = emulator.getU8Register(CPUEmulator::Emulator::U8Registers::Y);

    //   int IP = emulator.getU16Register(CPUEmulator::Emulator::U16Registers::IP);
    //   int SP = emulator.getU16Register(CPUEmulator::Emulator::U16Registers::SP);
    

    //   std::cout 
    //     << "A: " << A << "\n"
    //     << "B: " << B << "\n"
    //     << "C: " << C << "\n"
    //     << "X: " << X << "\n"
    //     << "Y: " << Y << "\n"
    //     << "IP: " << IP - 5 << "\n"
    //     << "SP: " << SP << "\n\n"; 

    //   std::cin.get();
    // }

  while(emulator.step());

        int A = emulator.getU16Register(CPUEmulator::Emulator::U16Registers::A);
      int B = emulator.getU16Register(CPUEmulator::Emulator::U16Registers::B);
      int C = emulator.getU16Register(CPUEmulator::Emulator::U16Registers::C);

      int X = emulator.getU8Register(CPUEmulator::Emulator::U8Registers::X);
      int Y = emulator.getU8Register(CPUEmulator::Emulator::U8Registers::Y);

    

      std::cout 
        << "A: " << A << "\n"
        << "B: " << B << "\n"
        << "C: " << C << "\n"
        << "X: " << X << "\n"
        << "Y: " << Y << "\n";
}