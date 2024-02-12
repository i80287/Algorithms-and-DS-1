#include <fstream>
#include <vector>
#include <iostream>

#include "collisions_gen.hpp"

void write_to_file(const std::vector<std::string>& strs,
                   std::string_view fname) {
    size_t total_size = 0;
    for (const auto& s : strs) {
        total_size += s.size();
    }
    total_size += strs.size();  // for '\n' chars
    std::string buffer;
    buffer.reserve(total_size);
    for (const auto& str : strs) {
        buffer.append(str).push_back('\n');
    }

    std::ofstream out(fname.data());
    if (out.fail()) {
        std::cerr << "Could not open file '" << fname << "' for writing\n";
        return;
    }
    out << buffer;
}

int main() {
    auto res = collisions_gen::generate_strings_with_same_hash("abcd", 2000);
    write_to_file(res, "strings.txt");
    return 0;
}
