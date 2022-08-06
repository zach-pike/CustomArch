#include "expr.hpp"
#include "ext/util.hpp"

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
                case token_type::left_bracket:
                    ++index;
                    add_expr(add_expr, token_type::right_bracket, std::get<bracket_expr>(exprs.emplace_back(bracket_expr{})).children);
                    break;
                case token_type::left_brace:
                    ++index;
                    add_expr(add_expr, token_type::right_brace, std::get<brace_expr>(exprs.emplace_back(brace_expr{})).children);
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
                    exprs.push_back(register_expr{
                        (register_type)((u32)tokens.types[index] - (u32)token_type::a_reg)
                    });
                    break;
                case token_type::push_keyword:
                case token_type::pop_keyword:
                case token_type::if_keyword:
                case token_type::goto_keyword:
                case token_type::return_keyword:
                case token_type::using_keyword:
                    exprs.push_back(keyword_expr{
                        (keyword_type)((u32)tokens.types[index] - (u32)token_type::push_keyword)
                    });
                    break;
                case token_type::dec_integer:
                    exprs.push_back(integer_expr{
                        ext::get_dec_integer<u16>(std::string_view(tokens.literals[index].data, tokens.literals[index].size))
                    });
                    break;
                case token_type::hex_integer:
                    exprs.push_back(integer_expr{
                        ext::get_hex_integer<u16>(std::string_view(tokens.literals[index].data, tokens.literals[index].size))
                    });
                    break;
                case token_type::string:
                    exprs.push_back(string_expr{tokens.literals[index]});
                    break;
                case token_type::character:
                    exprs.push_back(character_expr{tokens.literals[index].data[0]});
                    break;
            }
        }
    };

    add_expr(add_expr, token_type::invalid, exprs);

    return exprs;
}