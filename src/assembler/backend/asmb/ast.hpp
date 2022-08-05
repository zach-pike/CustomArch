#pragma once
#include "lex.hpp"
#include "ext/types.hpp"
#include <variant>
#include <vector>

namespace asmb {
    // Value and indirect value types

    struct label_identifier {
        token_literal name;
    };

    using value = std::variant<label_identifier>;
    struct indirect_value {
        bool is_indirect;
        value val;
    };

    // Node types

    struct label {
        token_literal name;
    };

    struct positioned_label {
        token_literal name;
        u16 addr;
    };

    struct push_instruction {
        value val;
    };

    struct pop_instruction {};

    struct pop_value_instruction {
        value val;
    };

    struct move_instruction {
        indirect_value into;
        indirect_value from;
    };

    struct call_instruction {
        value val;
    };

    struct increment_instruction {
        indirect_value val;
    };

    struct add_instruction {
        indirect_value into;
        indirect_value from;
    };

    struct sub_instruction {
        indirect_value into;
        indirect_value from;
    };

    struct mul_instruction {
        indirect_value into;
        indirect_value from;
    };

    struct div_instruction {
        indirect_value into;
        indirect_value from;
    };

    struct less_than_instruction {
        value into;
        value from;
    };

    struct greater_than_instruction {
        value into;
        value from;
    };

    struct less_than_or_equal_instruction {
        value into;
        value from;
    };

    struct greater_than_or_equal_instruction {
        value into;
        value from;
    };

    struct using_keyword {
        token_literal alias;
        value val;
    };

    struct root {};

    struct node {
        std::vector<node> children;
        std::variant<
            root,
            label,
            positioned_label,
            push_instruction,
            pop_instruction,
            pop_value_instruction
        > val;
    };

    node get_tree_from_tokens(tokens tokens);
};