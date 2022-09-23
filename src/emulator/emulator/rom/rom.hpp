#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace CPUEmulator {
class Rom {
private:
    std::uint8_t rom[0xffff];
public:
    Rom();
    ~Rom();

    void writeToFile(std::string filePath);
    void readFromFile(std::string fileLocation);

    void readFromBytes(const std::vector<std::uint8_t>& bytes);

    std::uint8_t& operator[](int index);
};
}