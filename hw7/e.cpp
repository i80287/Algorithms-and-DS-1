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

[[maybe_unused, noreturn]] static void reps1s2(const string_view s1, const string_view s2) {
    string m;
    m.reserve(s1.size() + s2.size() + 100);
    m += "s1.size() = ";
    m += to_string(s1.size());
    m += "; s2.size() = ";
    m += to_string(s2.size());
    m += "; s1 = '";
    m += s1;
    m += "'; s2 = '";
    m += s2;
    m += '\'';
    cerr << m << endl;
    throw runtime_error(m);
}

[[maybe_unused, noreturn]] static void repc(int c) {
    cerr << "char = '" << char(c) << "'; int value = " << c << endl;
    throw runtime_error("");
}

static u32 solve(const string_view s1, const string_view s2) noexcept {
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
            assert((dp[i][j] <= max(i, j)));
        }
    }

    return dp[s1.size()][s2.size()];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    uint32_t n = 1;
    cin >> n;
    for (bool cont = true; cont;) {
        int c = cin.peek();
        switch(c) {
        case '\n':
            cont = false;
        case '\r':
            cin.get();
            break;
        default:
            repc(c);
        }
    }

    [[maybe_unused]] const uint32_t orig_n = n;
    while (n--) {
        string s1, s2;
        getline(cin, s1);
        getline(cin, s2);
        // if (14 < min(s1.size(), s2.size()) && max(s1.size(), s2.size()) < 500) {
        //     reps1s2(s1, s2);
        // }
        cout << solve(s1, s2) << ' ';
    }
}
