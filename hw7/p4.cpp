#include <array>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

using namespace std;

constexpr auto char_to_index(char c) noexcept {
    return uint8_t(c);
}

#if __cplusplus >= 202002L
constexpr
#endif
static auto create_distance_table(string_view p) noexcept {
    array<size_t, 256> dist{};
    dist.fill(p.size());
    for (size_t i = 0; i + 1 < p.size(); i++) {
        dist[char_to_index(p[i])] = p.size() - i - 1;
    }
    return dist;
}

#if __cplusplus >= 202002L
constexpr
#endif
std::vector<int> BMH_search(std::string_view pattern, std::string_view text) {
    auto distance_table = create_distance_table(pattern);
    std::vector<int> positions;

    const size_t M = pattern.size();
    const size_t N = text.size();
    for (size_t txtIndex = M - 1; txtIndex < N;) {
        const auto search_pos = txtIndex - (M - 1);
        if (text.substr(search_pos, M) == pattern) {
            positions.push_back(int(search_pos));
            txtIndex++;
            continue;
        }
        txtIndex += distance_table[char_to_index(text[txtIndex])];
    }

    return positions;
}

#if __cplusplus >= 202002L
static_assert(BMH_search("a", "a") == vector{0});
static_assert(BMH_search("a", "aa") == vector{0, 1});
static_assert(BMH_search("a", "aaa") == vector{0, 1, 2});
static_assert(BMH_search("aa", "aaa") == vector{0, 1});
static_assert(BMH_search("aa", "aaaa") == vector{0, 1, 2});
static_assert(BMH_search("abcdefghjioasdhjhjkahwdj", "abcdefghjioasdhjhjkahwdjabcdefghjioasdhjhjkahwdj") == vector{0, 24});
static_assert(BMH_search("abcdefghjioasdhjhjkahwdj", "abcdefghjioasdhqhjkahwdjabcdefghjioasghjhjkahwdj").empty());
static_assert(BMH_search("bc", "dcbbabcaababcccbcbba") == vector{5, 11, 15});
static_assert(BMH_search("dad", "dcbdbdcabbcdcddcacaa").empty());
static_assert(BMH_search("aaaaaaa", "axaaaaaaaaaaaaaa") == vector{2, 3, 4, 5, 6, 7, 8, 9});
static_assert(BMH_search("ndud", "bundudxtafndudndud") == vector{2, 10, 14});
static_assert(BMH_search("liqi", "emliqizlhlliqiliqi") == vector{2, 10, 14});
static_assert(BMH_search("telb", "wyllbyrsyy").empty());
#endif

int main() {
    std::string pattern, text;
    std::cin >> pattern;
    std::cin >> text;
    std::vector<int> positions = BMH_search(pattern, text);
    std::cout << positions.size() << '\n';
    for (size_t i = 0; i < positions.size(); ++i) {
        std::cout << positions[i] << '\n';
    }

    return 0;
}
