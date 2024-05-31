#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <valarray>
#include <vector>

using namespace std;

enum Engine : uint32_t {
    kFirst,
    kSecond,
    kThird,
    kFourth,
    kFifth,
};

constexpr bool f1() {
    constexpr Engine dp[5][5] = {
        {kFirst, kFirst, kFirst, kFirst, kFirst},  {kFourth, kFirst, kSecond, kFirst, kFirst},
        {kFirst, kThird, kThird, kFirst, kFirst},  {kFirst, kFourth, kThird, kFirst, kSecond},
        {kFirst, kFifth, kSecond, kFirst, kFirst},
    };

    for (uint32_t i = 1; i < 4; i++) {
        for (uint32_t j = 1; j < 4; j++) {
            uint32_t mask = 0;
            mask |= 1u << dp[i - 1][j];
            mask |= 1u << dp[i + 1][j];
            mask |= 1u << dp[i][j - 1];
            mask |= 1u << dp[i][j + 1];
            auto ans1 = countr_one(mask);

            int ans = 0;
            if ((mask & 0b1111) == 0b1111) {
                ans = kFifth;
            } else if ((mask & 0b111) == 0b111) {
                ans = kFourth;
            } else if ((mask & 0b11) == 0b11) {
                ans = kThird;
            } else if ((mask & 0b1) == 0b1) {
                ans = kSecond;
            } else {
                ans = kFirst;
            }
            if (ans != ans1) {
                return false;
            }
        }
    }

    return true;
}
static_assert(f1());

using int128_t = __int128_t;

int main() {
    constexpr uint32_t n          = 256;
    constexpr uint32_t m          = 256;
    constexpr array<int64_t, 5> p = {13, 21, 34, 55, 89};
    static constinit array<array<array<int64_t, kFifth + 1>, m>, n> dp{};

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            dp[i][j][kFirst] = n * m * p[kFirst];
            for (size_t e = kSecond; e <= kFifth; e++) {
                dp[i][j][e] = -1;
            }
        }
    }

    auto update = [&](size_t i, size_t j, Engine e) constexpr noexcept {
        for (size_t ijm1 = kFirst; ijm1 <= kFifth; ijm1++) {
            const int64_t dp_i_jm1 = j >= 1 ? dp[i][j - 1][ijm1] : 0;
            if (dp_i_jm1 == -1) {
                continue;
            }
            const uint32_t mask_i_jm1 = j >= 1 ? 1u << ijm1 : 0;
            for (size_t ijp1 = kFirst; ijp1 <= kFifth; ijp1++) {
                const int64_t dp_ijp1 = j + 1 < m ? dp[i][j + 1][ijp1] : 0;
                if (dp_ijp1 == -1) {
                    continue;
                }
                const uint32_t mask_ijp1 = j + 1 < n ? 1u << ijp1 : 0;
                for (size_t im1j = kFirst; im1j <= kFifth; im1j++) {
                    const int64_t dp_im1j = i >= 1 ? dp[i - 1][j][im1j] : 0;
                    if (dp_im1j == -1) {
                        continue;
                    }
                    const uint32_t mask_im1j = i >= 1 ? 1u << im1j : 0;
                    for (size_t ip1j = kFirst; ip1j <= kFifth; ip1j++) {
                        const int64_t dp_ip1j = i + 1 < n ? dp[i + 1][j][ip1j] : 0;
                        if (dp_ip1j == -1) {
                            continue;
                        }
                        const uint32_t mask_ip1j = i + 1 < n ? 1u << ip1j : 0;
                        const int64_t max_dp = max(max(dp_i_jm1, dp_ijp1), max(dp_im1j, dp_ip1j));

                        uint32_t mask = 0;
                        mask |= mask_i_jm1;
                        mask |= mask_ijp1;
                        mask |= mask_im1j;
                        mask |= mask_ip1j;
                        const Engine maxavailable = Engine(countr_one(mask));
                        if (e <= maxavailable) {
                            dp[i][j][e] = max(dp[i][j][e], max_dp + p[e]);
                        }
                    }
                }
            }
        }
    };

    for (size_t iter = 200; iter > 0; iter--) {
        for (size_t e = kSecond; e <= kFifth; e++) {
            for (size_t i = 0; i < n; i++) {
                for (size_t j = 0; j < m; j++) {
                    update(i, j, Engine(e));
                }
            }
        }
    }

    int64_t w = -1;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            for (size_t e = kFirst; e <= kFifth; e++) {
                w = max(w, dp[i][j][e]);
            }
        }
    }

    cout << int64_t(w);
}
