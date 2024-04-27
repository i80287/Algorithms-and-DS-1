#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

using u32       = uint32_t;
constexpr u32 N = 4001;
static array<array<u32, N>, N> dp{};

static constexpr u32 solve(const string_view s1, const string_view s2) noexcept {
    for (size_t i = 0; i <= s1.size(); i++) {
        dp[i][0] = u32(i);
    }
    for (size_t j = 0; j <= s2.size(); j++) {
        dp[0][j] = u32(j);
    }
    for (size_t i = 1; i <= s1.size(); i++) {
        for (size_t j = 1; j <= s2.size(); j++) {
            dp[i][j] = min(
                dp[i - 1][j - 1] + (s1[i - 1] != s2[j - 1]),
                min(
                    dp[i - 1][j] + 1,
                    dp[i][j - 1] + 1
            ));
        }
    }

    return dp[s1.size()][s2.size()];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    uint32_t n = 1;
    cin >> n;
    while (cin.get() != '\n') {
    }
    while (n--) {
        string s1, s2;
        getline(cin, s1);
        getline(cin, s2);
        cout << solve(s1, s2) << ' ';
    }
}
