#pragma once
#include <string_view>
#include <vector>
#include <span>
#include "../ext/types.hpp"

namespace asmb {
    std::vector<u8> get_bytes_from_source(const std::span<const char> source);
};