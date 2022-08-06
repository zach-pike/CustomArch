#pragma once
#include "lex.hpp"
#include "ext/types.hpp"
#include <variant>
#include <vector>

namespace asmb {
    // Expressions
    struct token_expr {
        token_type type;  
    };

    struct identifier_expr {
        token_literal name;
    };

    enum class register_type {
        a,
        b,
        c,
        d,
        xy,
        sp_reg,
        ip_reg,
        al,
        ah,
        bl,
        bh,
        cl,
        ch,
        dl,
        dh,
        x,
        y
    };

    struct register_expr {
        register_type type;
    };

    struct integer_expr {
        u16 value;
    };

    struct string_expr {
        token_literal value;
    };

    struct character_expr {
        char value;
    };

    struct paren_expr;
    struct bracket_expr;
    struct brace_expr;

    using expr = std::variant<
        token_expr,
        paren_expr,
        bracket_expr,
        brace_expr,
        identifier_expr,
        register_expr,
        integer_expr,
        string_expr,
        character_expr
    >;

    struct paren_expr {
        std::vector<expr> children;
    };

    struct bracket_expr {
        std::vector<expr> children;
    };

    struct brace_expr {
        std::vector<expr> children;
    };

    std::vector<expr> get_expressions_from_tokens(tokens tokens);
};