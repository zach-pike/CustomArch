#pragma once

#include <cstdint>
#include <stdexcept>

namespace CPUEmulator {
class Program {
private:
    std::uint8_t rom[0xffff];
public:
    std::uint8_t& operator[](int index);
};
}