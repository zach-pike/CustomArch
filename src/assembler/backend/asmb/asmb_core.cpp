#include "asmb_core.hpp"
#include "lex.hpp"
#include <fmt/core.h>

using namespace asmb;

std::vector<u8> asmb::get_bytes_from_source(const std::span<const char> source) {
    auto tokens = get_tokens_from_source(source);

    for (std::size_t i = 0; i < tokens.size; i++) {
        fmt::print("{}, {}\n", (u32)tokens.types[i], std::string_view(tokens.literals[i].data, tokens.literals[i].size));
    }

    return std::vector<u8>(0xffff);
}