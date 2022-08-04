#include "program.hpp"

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