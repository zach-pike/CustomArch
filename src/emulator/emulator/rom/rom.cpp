#include "rom.hpp"

using CPUEmulator::Rom;


Rom::Rom() {}
Rom::~Rom() {}

void Rom::writeToFile(std::string fp) {

}

void Rom::readFromFile(std::string fp) {
    
}

std::uint8_t& Rom::operator[](int index) {
    if (index < 0 || index > 0xffff) throw std::runtime_error("Index out of bounds!");
    return rom[index];
}

void Rom::readFromBytes(const std::vector<std::uint8_t>& bytes) {
    if (bytes.size() != 0x10000) throw std::runtime_error("Array is incorrect size!");
    int index = 0;
    for (const auto& b : bytes) rom[index ++] = b;
}