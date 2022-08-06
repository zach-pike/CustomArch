#include "asmb_core.hpp"
#include "lex.hpp"
#include "ast.hpp"
#include <fmt/core.h>

using namespace asmb;

void print_node(const node& node) {
    fmt::print("{}\n", node.val.index());
    fmt::print("Children (#{}): [\n", node.children.size());
    for (const auto& child : node.children) {
        print_node(child);
    }
    fmt::print("]\n");
}

std::vector<u8> asmb::get_bytes_from_source(const std::span<const char> source) {
    auto tokens = get_tokens_from_source(source);

    // for (std::size_t i = 0; i < tokens.size; i++) {
    //     fmt::print("{}, {}\n", (u32)tokens.types[i], std::string_view(tokens.literals[i].data, tokens.literals[i].size));
    // }

    auto root = get_tree_from_tokens(std::move(tokens));

    // print_node(root); 

    return std::vector<u8>(0xffff);
}