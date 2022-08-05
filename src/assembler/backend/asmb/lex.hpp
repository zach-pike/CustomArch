#pragma once
#include "ext/interval_allocator.hpp"
#include <span>
#include <vector>

namespace asmb {
    enum class token_type {
        invalid,
        // Words
        // Keywords
        if_keyword,
        goto_keyword,
        using_keyword,
        return_keyword,
        push_keyword,
        pop_keyword,
        // Registers
        a_reg,
        b_reg,
        c_reg,
        d_reg,
        sp_reg,
        ip_reg,
        al_reg,
        ah_reg,
        bl_reg,
        bh_reg,
        cl_reg,
        ch_reg,
        dl_reg,
        dh_reg,
        xy_reg,
        x_reg,
        y_reg,
        //
        identifier,
        //
        // Symbols
        semicolon,
        left_paren,
        right_paren,
        left_bracket,
        right_bracket,
        left_brace,
        right_brace,
        left_angle_bracket,
        right_angle_bracket,
        left_angle_bracket_equals,
        right_angle_bracket_equals,
        equals,
        colon,
        comma,
        period,
        // plus,
        // minus,
        // star,
        // slash,
        // percent,
        // ampersand,
        // pipe,
        // exclamation,
        // caret,
        plus_plus,
        minus_minus,
        plus_equals,
        minus_equals,
        star_equals,
        slash_equals,
        percent_equals,
        ampersand_equals,
        pipe_equals,
        exclamation_equals,
        caret_equals,
        //
        integer,
        character,
        string
    };

    enum class lexer_state {
        none,
        whitespace,
        word,
        symbol,
        dec_integer,
        hex_integer,
        slash_or_comment,
        comment,
        character,
        string,
        string_or_character_end,
        eof
    };

    struct token_types {
        token_type* types;

        token_types(std::size_t size);
        ~token_types();
    };
    struct token_literal {
        const char* data;
        std::size_t size = 0;
    };

    struct lexer_states_and_token_literals {
        std::size_t size = 0;
        ext::interval_allocator<1024> allocator;
        lexer_state* states = nullptr;
        token_literal* literals = nullptr;

        void append(lexer_state state, const token_literal& literal);
        ~lexer_states_and_token_literals();
    };
    
    struct tokens {
        std::size_t size = 0;
        token_type* types = nullptr;
        token_literal* literals = nullptr;

        tokens() = default;
        tokens(lexer_states_and_token_literals&& states_and_literals, token_types&& types);
        ~tokens();
    };

    tokens get_tokens_from_source(const std::span<const char> source);
};