#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

namespace CPUEmulator {
class Rom {
private:
    std::uint8_t rom[0xffff];
public:
    Rom();
    ~Rom();

    void writeToFile(std::string filePath);
    void readFromFile(std::string fileLocation);

    std::uint8_t& operator[](int index);
};
}