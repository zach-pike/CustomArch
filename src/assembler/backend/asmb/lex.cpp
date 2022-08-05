#include "lex.hpp"
#include "ext/types.hpp"
#include "ext/util.hpp"
#include <fmt/core.h>

using namespace asmb;

token_types::token_types(std::size_t size) : types((token_type*)std::malloc(size * sizeof(token_type))) {}
token_types::~token_types() {
    if (types != nullptr) {
        std::free(types);
    }
}

void lexer_states_and_token_literals::append(lexer_state state, const token_literal& literal) {
    std::size_t new_size = size + 1;

    bool should_reallocate = allocator.reserve(new_size);
    if (should_reallocate) {
        if (size > 0) {
            states = allocator.realloc(states);
            literals = allocator.realloc(literals);
        } else {
            states = allocator.alloc<lexer_state>();
            literals = allocator.alloc<token_literal>();
        }
    }

    states[size] = state;
    literals[size] = literal;

    size = new_size;
}

lexer_states_and_token_literals::~lexer_states_and_token_literals() {
    if (states != nullptr) {
        allocator.free(states);
    }
    if (literals != nullptr) {
        allocator.free(literals);
    }
}

tokens::tokens(lexer_states_and_token_literals&& states_and_literals, token_types&& types) : size(states_and_literals.size), types(types.types), literals(states_and_literals.literals) {
    states_and_literals.size = 0;
    states_and_literals.literals = nullptr;
    types.types = nullptr;
}

tokens::~tokens() {
    if (types != nullptr) {
        std::free(types);
    }
    if (literals != nullptr) {
        std::free(literals);
    }
}

#define CASE_NEWLINE \
case 0xD: \
case '\n'

#define CASE_WHITESPACE \
case 0xD: \
case ' ': \
case '\n': \
case '\t'

#define CASE_SYMBOL \
case ';': \
case ':': \
case '(': \
case ')': \
case '{': \
case '}': \
case '[': \
case ']': \
case '<': \
case '>': \
case '.': \
case ',': \
case '=': \
case '+': \
case '-': \
case '*': \
case '/': \
case '%': \
case '&': \
case '|': \
case '!': \
case '^'

#define CASE_DEC_DIGIT \
case '0': \
case '1': \
case '2': \
case '3': \
case '4': \
case '5': \
case '6': \
case '7': \
case '8': \
case '9'

#define CASE_HEX_DIGIT \
CASE_DEC_DIGIT: \
case 'A': \
case 'B': \
case 'C': \
case 'D': \
case 'E': \
case 'F': \
case 'a': \
case 'b': \
case 'c': \
case 'd': \
case 'e': \
case 'f'

tokens asmb::get_tokens_from_source(const std::span<const char> source) {
    if (source.size() == 0) {
        return {};
    }

    lexer_states_and_token_literals states_and_literals;

    lexer_state state = lexer_state::none;

    auto it = source.begin();
    auto literal_begin = it;

    const auto reset_literal = [&]() {
        literal_begin = it;
    };

    const auto tokenize = [&]() {
        if (literal_begin != it && it < source.end()) {
            states_and_literals.append(state, { &(*literal_begin), (std::size_t)(it - literal_begin) });
        }
        reset_literal();
    };

    const auto use_none_state = [&]() {
        switch (*it) {
            default: state = lexer_state::word; return;
            CASE_SYMBOL: state = lexer_state::symbol; return;
            CASE_WHITESPACE: state = lexer_state::whitespace; return;
            CASE_DEC_DIGIT: state = lexer_state::dec_integer; return;
            case '\'': state = lexer_state::character; return;
            case '"': state = lexer_state::string; return;
        }
    };

    const auto use_word_state = [&]() {
        for (;it != source.end(); ++it) {
            switch (*it) {
                CASE_SYMBOL:
                CASE_WHITESPACE:
                CASE_DEC_DIGIT:
                case '\'':
                case '"':
                    tokenize();
                    state = lexer_state::none;
                    return;
                default: break;
            }
        }
        tokenize();
        state = lexer_state::eof;
    };

    const auto use_symbol_state = [&]() {
        for (;it != source.end(); ++it) {
            switch (*it) {
                CASE_SYMBOL:
                    switch (*it) {
                        default:
                            tokenize();
                            break;
                        case '+':
                        case '-':
                        case '=': break;
                        case '/':
                            state = lexer_state::slash_or_comment;
                            return;
                    }
                    break;
                default:
                    tokenize();
                    state = lexer_state::none;
                    return;
            }
        }
        tokenize();
        state = lexer_state::eof;
    };

    const auto use_slash_or_comment_state = [&]() {
        ++it;
        for (;it != source.end(); ++it) {
            switch (*it) {
                default:
                    tokenize();
                    state = lexer_state::none;
                    return;
                case '/':
                    state = lexer_state::comment;
                    return;
                case '=': break;
            }
        }
        tokenize();
        state = lexer_state::eof;
    };

    const auto use_comment_state = [&]() {
        ++it;
        for (;it != source.end(); ++it) {
            switch (*it) {
                CASE_NEWLINE:
                    state = lexer_state::whitespace;
                    return;
                default: break;
            }
        }
        state = lexer_state::eof;
    };

    const auto use_whitespace_state = [&]() {
        ++it;
        for (;it != source.end(); ++it) {
            switch (*it) {
                default:
                    reset_literal();
                    state = lexer_state::none;
                    return;
                CASE_WHITESPACE: break;
            }
        }
        state = lexer_state::eof;
    };

    const auto use_dec_integer_state = [&]() {
        for (;it != source.end(); ++it) {
            switch (*it) {
                default:
                    tokenize();
                    state = lexer_state::none;
                    return;
                case 'x':
                    state = lexer_state::hex_integer;
                    return;
                CASE_DEC_DIGIT: break;
            }
        }
        tokenize();
        state = lexer_state::eof;
    };

    const auto use_hex_integer_state = [&]() {
        ++it;
        for (;it != source.end(); ++it) {
            switch (*it) {
                default:
                    tokenize();
                    state = lexer_state::none;
                    return;
                CASE_HEX_DIGIT: break;
            }
        }
        tokenize();
        state = lexer_state::eof;
    };

    const auto use_character_state = [&]() {
        ++it;
        reset_literal();
        for (;it != source.end(); ++it) {
            switch (*it) {
                case '\'':
                    tokenize();
                    state = lexer_state::string_or_character_end;
                    return;
            }
        }
        tokenize();
        state = lexer_state::eof;
    };

    const auto use_string_state = [&]() {
        ++it;
        reset_literal();
        for (;it != source.end(); ++it) {
            switch (*it) {
                case '"':
                    tokenize();
                    state = lexer_state::string_or_character_end;
                    return;
            }
        }
        tokenize();
        state = lexer_state::eof;
    };

    const auto use_string_or_character_end_state = [&]() {
        ++it;
        for (;it != source.end(); ++it) {
            reset_literal();
            state = lexer_state::none;
            return;
        }
        state = lexer_state::eof;
    };

    while (state != lexer_state::eof) {
        switch (state) {
            case lexer_state::none: use_none_state(); break;
            case lexer_state::word: use_word_state(); break;
            case lexer_state::symbol: use_symbol_state(); break;
            case lexer_state::slash_or_comment: use_slash_or_comment_state(); break;
            case lexer_state::comment: use_comment_state(); break;
            case lexer_state::whitespace: use_whitespace_state(); break;
            case lexer_state::dec_integer: use_dec_integer_state(); break;
            case lexer_state::hex_integer: use_hex_integer_state(); break;
            case lexer_state::character: use_character_state(); break;
            case lexer_state::string: use_string_state(); break;
            case lexer_state::string_or_character_end: use_string_or_character_end_state(); break;
            default: break;
        }
    }

    token_types types(states_and_literals.size);
    const auto get_literal_type = [](lexer_state state, const token_literal& literal) {
        // Literal is guaranteed to at least have a length of 1 so literal.data[0] is valid.
        switch (state) {
            case lexer_state::word:
                switch (literal.size) {
                    default: break;
                    case 1: switch (literal.data[0]) {
                        case 'A': return token_type::a_reg;
                        case 'B': return token_type::b_reg;
                        case 'C': return token_type::c_reg;
                        case 'D': return token_type::d_reg;
                        case 'X': return token_type::x_reg;
                        case 'Y': return token_type::y_reg;
                        default: break;
                    } break;
                    case 2: switch (literal.data[1]) {
                        case 'L': switch (literal.data[0]) {
                            case 'A': return token_type::al_reg;
                            case 'B': return token_type::bl_reg;
                            case 'C': return token_type::cl_reg;
                            case 'D': return token_type::dl_reg;
                            default: break;
                        } break;
                        case 'H': switch (literal.data[0]) {
                            case 'A': return token_type::ah_reg;
                            case 'B': return token_type::bh_reg;
                            case 'C': return token_type::ch_reg;
                            case 'D': return token_type::dh_reg;
                            default: break;
                        } break;
                        case 'P': switch (literal.data[0]) {
                            case 'S': return token_type::sp_reg;
                            case 'I': return token_type::ip_reg;
                        } break;
                        case 'Y': switch (literal.data[0]) {
                            case 'X': return token_type::xy_reg;
                            default: break;
                        } break;
                        case 'f': switch (literal.data[0]) {
                            case 'i': return token_type::if_keyword;
                            default: break;
                        } break;
                        default: break;
                    } break;
                    case 3:
                        if (ext::string_compare<"pop">(literal.data)) {
                            return token_type::pop_keyword;
                        }
                        break;
                    case 4:
                        if (ext::string_compare<"goto">(literal.data)) {
                            return token_type::goto_keyword;
                        } else if (ext::string_compare<"push">(literal.data)) {
                            return token_type::push_keyword;
                        }
                        break;
                    case 5:
                        if (ext::string_compare<"return">(literal.data)) {
                            return token_type::return_keyword;
                        } else if (ext::string_compare<"using">(literal.data)) {
                            return token_type::using_keyword;
                        }
                        break;
                }
                return token_type::identifier;
            case lexer_state::symbol:
                switch (literal.size) {
                    default: break;
                    case 1: switch (literal.data[0]) {
                        case ';': return token_type::semicolon;
                        case '(': return token_type::left_paren;
                        case ')': return token_type::right_paren;
                        case '[': return token_type::left_bracket;
                        case ']': return token_type::right_bracket;
                        case '{': return token_type::left_brace;
                        case '}': return token_type::right_brace;
                        case '<': return token_type::left_angle_bracket;
                        case '>': return token_type::right_angle_bracket;
                        case '=': return token_type::equals;
                        case ':': return token_type::colon;
                        case ',': return token_type::comma;
                        case '.': return token_type::period;
                        default: break;
                    } break;
                    case 2: switch (literal.data[1]) {
                        case '+': switch (literal.data[0]) {
                            case '+': return token_type::plus_plus;
                            default: break;
                        } break;
                        case '-': switch (literal.data[0]) {
                            case '-': return token_type::minus_minus;
                            default: break;
                        } break;
                        case '=': switch (literal.data[0]) {
                            case '<': return token_type::left_angle_bracket_equals;
                            case '>': return token_type::right_angle_bracket_equals;
                            case '+': return token_type::plus_equals;
                            case '-': return token_type::minus_equals;
                            case '*': return token_type::star_equals;
                            case '/': return token_type::slash_equals;
                            case '%': return token_type::percent_equals;
                            case '&': return token_type::ampersand_equals;
                            case '|': return token_type::pipe_equals;
                            case '^': return token_type::caret_equals;
                            default: break;
                        } break;
                    } break;
                }
                return token_type::invalid;
            case lexer_state::dec_integer:
            case lexer_state::hex_integer: return token_type::integer;
            case lexer_state::character: return token_type::character;
            case lexer_state::string: return token_type::string;
            default: return token_type::invalid;
        }
    };

    for (std::size_t i = 0; i < states_and_literals.size; ++i) {
        types.types[i] = get_literal_type(
            states_and_literals.states[i],
            states_and_literals.literals[i]
        );
    }

    return { std::move(states_and_literals), std::move(types) };
}