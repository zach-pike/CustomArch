#include <fmt/core.h>
#include <span>
#include "fs.hpp"
#include "asmb/asmb_core.hpp"

int main(int argc, const char** argv) {
    const std::span<const char*> args(argv, argc);

    if (args.size() < 1) {
        fmt::print("No first argument supplied\n");
        return 1;
    }

    if (args.size() < 3) {
        fmt::print("Usage: {} <output binary> <source files>\n", args[0]);
        return 1;
    }

    const auto output_file_path = args[1];
    const std::span<const char*> source_file_paths(args.data() + 2, args.size() - 2);

    for (const auto path : source_file_paths) {
        // It is faster to read the entire file into memory at once than to read it line by line.
        // This is because a line by line load will increase the amount of heap allocations and cause more cache misses.
        // We are allocating this memory upfront to avoid a heap fragmentation issue.
        const auto [ source, error_code ] = fs::read_string(path);
        if (source.has_value()) {
            const auto bytes = asmb::get_bytes_from_source(*source);
            fs::write_bytes(output_file_path, bytes);
        } else {
            fmt::print("{}: {}, skipping.\n", strerror(error_code), path);
        }
    }

    return 0;
}