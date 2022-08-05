#pragma once
#include <cstdlib>

namespace ext {
    template<std::size_t INTERVAL = 1024>
    class interval_allocator {
        std::size_t size = 0;
        public:
            bool reserve(std::size_t new_size) {
                if (new_size > size) {
                    while (new_size > size) {
                        size += INTERVAL;
                    }
                    return true;
                }
                return false;
            }

            template<typename T>
            T* alloc() {
                return (T*)std::malloc(size * sizeof(T));
            }

            void free(void* ptr) {
                std::free(ptr);
            }

            template<typename T>
            T* realloc(T* ptr) {
                return (T*)std::realloc(ptr, size * sizeof(T));
            }
    };
}