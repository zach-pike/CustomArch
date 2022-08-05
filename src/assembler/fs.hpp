#pragma once
#include <optional>
#include <span>
#include <tuple>
#include <string>
#include <vector>
#include "ext/types.hpp"

namespace fs {
    std::tuple<std::optional<std::vector<char>>, error_t> read_string(const char* path);

    void write_bytes(const char* path, const std::span<const u8> bytes);
};