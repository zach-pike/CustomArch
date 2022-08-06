#pragma once
#include "expr.hpp"

namespace asmb {
    // Value types
    using integer_value = std::variant<identifier_expr, integer_expr, character_expr>;

    using direct_register_value = std::variant<identifier_expr, register_expr>;
    struct indirect_register_value {
        bool is_indirect;
        direct_register_value value;
    };

    using direct_value = std::variant<identifier_expr, integer_expr, character_expr, register_expr>;
    struct indirect_value {
        bool is_indirect;
        direct_value value;
    };

    using string_value = string_expr;

    //

    struct label_node;
    struct positioned_label_node;

    using string_node = string_value;

    using node = std::variant<
        label_node,
        positioned_label_node,
        string_node
    >;

    struct label_node {
        token_literal name;
        std::vector<node> children;
    };

    struct positioned_label_node {
        token_literal name;
        integer_value position;
        std::vector<node> children;
    };

    std::vector<node> get_tree_from_expressions(std::vector<expr> exprs);
};