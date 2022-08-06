#include "asmb_core.hpp"
#include "lex.hpp"
#include "expr.hpp"
#include <fmt/core.h>

using namespace asmb;

void print_exprs(std::span<const expr> exprs) {
    fmt::print("[\n");
    for (const auto& expr : exprs) {
        if (std::holds_alternative<paren_expr>(expr)) {
            fmt::print("Paren: ");
            const paren_expr& paren = std::get<paren_expr>(expr);
            print_exprs(paren.children);
        } else if (std::holds_alternative<token_expr>(expr)) {
            const token_expr& token = std::get<token_expr>(expr);
            fmt::print("Token: {}\n", (u32)token.type);
        } else {
            fmt::print("{}\n", expr.index());
        }
    }
    fmt::print("]\n");
}

std::vector<u8> asmb::get_bytes_from_source(const std::span<const char> source) {
    auto tokens = get_tokens_from_source(source);

    // for (std::size_t i = 0; i < tokens.size; i++) {
    //     fmt::print("{}, {}\n", (u32)tokens.types[i], std::string_view(tokens.literals[i].data, tokens.literals[i].size));
    // }

    auto exprs = get_expressions_from_tokens(std::move(tokens));

    print_exprs(exprs); 

    return std::vector<u8>(0xffff);
}