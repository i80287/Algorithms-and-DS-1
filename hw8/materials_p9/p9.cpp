#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <valarray>
#include <vector>

using namespace std;

enum Engine {
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

static int64_t parse(int argc, const char* const argv[]) {
    if (argc != 2) {
        puts("argc != 2");
        return -1;
    }
    char* end_ptr = nullptr;
    uint64_t res  = strtoul(argv[1], &end_ptr, 10);
    if (end_ptr == nullptr || *end_ptr != '\0' || static_cast<uint32_t>(res) != res) {
        puts("invalid argv[1]");
        return -1;
    }

    return static_cast<uint32_t>(res);
}

int main(int argc, const char* const argv[]) {
    const int64_t c_iters = parse(argc, argv);
    if (c_iters < 0) {
        return 1;
    }

    constexpr uint32_t n          = 256;
    constexpr uint32_t m          = 256;
    constexpr array<int64_t, 5> p = {13, 21, 34, 55, 89};
    struct cell_t {
        int64_t cost;
        Engine e;
    };
    static constinit array<array<cell_t, m>, n> dp{};
    memset(&dp, -1, sizeof(dp));
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            dp[i][j] = {n * m * p[kFirst], kFirst};
        }
    }

    auto update = [&](size_t i, size_t j) constexpr noexcept {
        const int64_t dp_i_jm1 = j >= 1 ? dp[i][j - 1].cost : 0;
        if (dp_i_jm1 == -1) {
            return;
        }
        assert(j == 0 || dp[i][j - 1].e != -1);
        const uint32_t mask_i_jm1 = j >= 1 ? 1u << dp[i][j - 1].e : 0;
        const int64_t dp_i_jp1    = j + 1 < n ? dp[i][j + 1].cost : 0;
        if (dp_i_jp1 == -1) {
            return;
        }
        assert(j + 1 == n || dp[i][j + 1].e != -1);
        const uint32_t mask_i_jp1 = j + 1 < n ? 1u << dp[i][j + 1].e : 0;
        const int64_t dp_im1_j    = i >= 1 ? dp[i - 1][j].cost : 0;
        if (dp_im1_j == -1) {
            return;
        }
        assert(i == 0 || dp[i - 1][j].e != -1);
        const uint32_t mask_im1_j = i >= 1 ? 1u << dp[i - 1][j].e : 0;
        const int64_t dp_ip1_j    = i + 1 < n ? dp[i + 1][j].cost : 0;
        if (dp_ip1_j == -1) {
            return;
        }
        assert(i + 1 == n || dp[i + 1][j].e != -1);
        const uint32_t mask_ip1_j = i + 1 < n ? 1u << dp[i + 1][j].e : 0;

        const int64_t max_dp = max(max(dp_i_jm1, dp_i_jp1), max(dp_im1_j, dp_ip1_j));
        uint32_t mask        = 0;
        mask |= mask_i_jm1;
        mask |= mask_i_jp1;
        mask |= mask_im1_j;
        mask |= mask_ip1_j;
        const Engine maxavailable = Engine(countr_one(mask));
        assert(kFirst <= maxavailable && maxavailable <= kFifth);
        if (maxavailable < kFirst || maxavailable > kFifth) {
            __builtin_unreachable();
        }
        if (dp[i][j].cost < max_dp + p[maxavailable]) {
            dp[i][j] = {.cost = max_dp + p[maxavailable], .e = maxavailable};
        }
    };

    for (auto iter = c_iters; iter > 0; iter--) {
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < m; j++) {
                update(i, j);
            }
        }
    }

    int64_t w = -1;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            for (size_t e = kFirst; e <= kFifth; e++) {
                w = max(w, dp[i][j].cost);
            }
        }
    }
    cout << w << '\n';
}
