#include "ast.hpp"

using namespace asmb;

node asmb::get_tree_from_tokens(tokens tokens) {
    // // Use call state instead of state machine.
    // enum class builder_state {
    //     none,
    //     identifier,
    //     eof
    // };
    
    // builder_state state = builder_state::none;

    // std::size_t index = 0;

    // const auto use_none_state = [&]() {
    //     for (; index != tokens.size; ++index) {
    //         switch (tokens.types[index]) {
    //             default: break;
    //             case token_type::identifier:
    //                 state = builder_state::identifier;
    //                 return;
    //         }
    //     }
    // };

    // const auto use_identifier_state = [&]() {
    //     auto identifier_literal = tokens.literals[index];
    //     ++index;
    //     for (; index != tokens.size; ++index) {
    //     }
    // };

    // while (state != builder_state::eof) {
    //     switch (state) {
    //         case builder_state::none: use_none_state(); break;
    //         case builder_state::identifier: use_identifier_state(); break;
    //         default: break;
    //     }
    // }

    return { .children = {}, .val = root{} };
}