#include "expr.hpp"
#include <fmt/core.h>

using namespace asmb;

std::vector<expr> asmb::get_expressions_from_tokens(tokens tokens) {
    std::vector<expr> exprs;

    std::size_t index = 0;

    const auto add_expr = [&tokens, &index](const auto& add_expr, token_type end_token_type, std::vector<expr>& exprs) -> void {
        for (;index < tokens.size; ++index) {
            if (tokens.types[index] == end_token_type) {
                return;
            }
            switch (tokens.types[index]) {
                default:
                    exprs.push_back(token_expr{tokens.types[index]});
                    break;
                case token_type::identifier:
                    exprs.push_back(identifier_expr{tokens.literals[index]});
                    break;
                case token_type::left_paren:
                    ++index;
                    add_expr(add_expr, token_type::right_paren, std::get<paren_expr>(exprs.emplace_back(paren_expr{})).children);
                    break;
                case token_type::a_reg:
                case token_type::b_reg:
                case token_type::c_reg:
                case token_type::d_reg:
                case token_type::al_reg:
                case token_type::ah_reg:
                case token_type::bl_reg:
                case token_type::bh_reg:
                case token_type::cl_reg:
                case token_type::ch_reg:
                case token_type::dl_reg:
                case token_type::dh_reg:
                case token_type::xy_reg:
                case token_type::x_reg:
                case token_type::y_reg:
                case token_type::sp_reg:
                case token_type::ip_reg:
                    exprs.push_back(register_expr{(register_type)((u32)tokens.types[index] - (u32)token_type::a_reg)});
                    break;
            }
        }
    };

    add_expr(add_expr, token_type::invalid, exprs);

    return exprs;
}