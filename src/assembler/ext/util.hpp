#pragma once
#include <cstdint>

namespace ext {
    template<std::size_t MAX, std::size_t I, typename F>
    constexpr void for_in_range_impl(const F& func) {
        if constexpr (I < MAX) {
            func.template operator()<I>();
            for_in_range_impl<MAX, I + 1>(func);
        }
    }

    template<std::size_t MAX, typename F>
    constexpr void for_in_range(const F& func) {
        for_in_range_impl<MAX, 0>(func);
    }

    template<typename T>
    struct remove_pointer_reference {
        using type = std::remove_pointer_t<std::remove_reference_t<std::remove_pointer_t<T>>>;
    };

    template<typename T>
    using remove_pointer_reference_t = typename remove_pointer_reference<T>::type;

    template<std::size_t N>
    struct string_literal {
        std::size_t size = 0;
        char data[N];

        constexpr string_literal(const char (&str)[N]) : size(N - 1) {
            std::copy_n(str, N, data);
        }
    };

    // std::string_view won't work here??
    template<string_literal COMP>
    bool string_compare(const char* other) {
        for (std::size_t i = 0; i < COMP.size; ++i) {
            if (COMP.data[i] != other[i]) {
                return false;
            }
        }
        return true;
    }
}