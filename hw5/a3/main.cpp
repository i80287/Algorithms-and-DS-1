#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "collisions_gen.hpp"

void write_to_file(const std::vector<std::string>& strs, std::string_view fname) {
    size_t total_size = 0;
    for (const auto& s : strs) {
        total_size += s.size();
    }
    total_size += strs.size();  // for '\n' chars
    std::string buffer;
    buffer.reserve(total_size);
    for (const auto& str : strs) {
        buffer += str;
        buffer.push_back('\n');
    }

    std::ofstream out(fname.data());
    if (!out.good() || !out.is_open()) {
        std::cerr << "Could not open file '" << fname << "' for writing\n";
        return;
    }
    out << buffer;
}

std::pair<const char*, uint32_t> parse_arguments(int argc,
                                                 const char* const argv[]) noexcept {
    const char* initial_string = "";
    uint32_t size              = collisions_gen::kDefaultSize;

    // argv[0] is an executable name
    if (argc >= 2) {
        initial_string = argv[1];
        if (argc >= 3) {
            char* s_end        = nullptr;
            long long arg_size = strtoll(argv[2], &s_end, 10);
            // If converted number fits in uint32_t
            //    and at least one char was read.
            if (uint32_t(arg_size) == arg_size && argv[2] < s_end) {
                size = uint32_t(arg_size);
            }
        }
    }

    return {initial_string, size};
}

int main(int argc, const char* const argv[]) {
    auto [initial_string, size] = parse_arguments(argc, argv);
    auto res = collisions_gen::generate_strings_with_same_hash(initial_string, size);
    write_to_file(res, "strings.txt");
    return 0;
}
