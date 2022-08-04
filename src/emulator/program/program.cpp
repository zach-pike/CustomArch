#include "program.hpp"

using CPUEmulator::Program;

std::uint8_t& Program::operator[](int index) {
    if (index < 0 || index > 0xffff) throw std::runtime_error("Index out of bounds!");
    return rom[index];
}