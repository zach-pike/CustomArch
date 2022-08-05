#include "fs.hpp"
#include <sys/stat.h>

using namespace fs;

std::tuple<std::optional<std::vector<char>>, error_t> fs::read_string(const char* path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        return std::make_tuple(std::nullopt, errno );
    }
    const std::size_t size = st.st_size;
    std::vector<char> str(size);

    const auto file = std::fopen(path, "r");
    const auto bytes_read = std::fread(str.data(), 1, size, file);
    if (bytes_read != size) {
        return std::make_tuple(std::nullopt, errno );
    }
    std::fclose(file);

    return { str, 0 };
}

void fs::write_bytes(const char* path, const std::span<const u8> bytes) {
    auto file = fopen(path, "wb");
    fwrite(bytes.data(), 1, bytes.size(), file);
    fclose(file);
}