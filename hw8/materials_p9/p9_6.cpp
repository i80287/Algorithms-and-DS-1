#include <unistd.h>

#include <algorithm>
#include <any>
#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numbers>
#include <numeric>
#include <random>
#include <set>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std;
using u8   = uint8_t;
using u16  = uint16_t;
using u32  = uint32_t;
using u64  = uint64_t;
using u128 = __uint128_t;
using i8   = int8_t;
using i16  = int16_t;
using i32  = int32_t;
using i64  = int64_t;
using i128 = __int128_t;

inline constexpr size_t n        = 5;
inline constexpr size_t m        = 5;
inline constexpr size_t kMaxW    = 85;
inline constexpr array<i32, 5> p = {1, 2, 3, 4, 5};

// inline constexpr size_t n        = 30;
// inline constexpr size_t m        = 30;
// inline constexpr size_t kMaxW    = 3413;
// inline constexpr array<i32, 5> p = {1, 2, 3, 4, 5};

// inline constexpr size_t n        = 1;
// inline constexpr size_t m        = 1000;
// inline constexpr size_t kMaxW    = 2997;
// inline constexpr array<i32, 5> p = {1, 2, 4, 4, 4};

enum class Engine : u32 {
    kFirst,
    kSecond,
    kThird,
    kFourth,
    kFifth,
};
static constinit array<array<Engine, m>, n> dp{};

constexpr bool correct_engine(Engine e) noexcept {
    return u32(e) - u32(Engine::kFirst) <=
           u32(Engine::kFifth) - u32(Engine::kFirst);
}

static Engine get_max_engine(size_t i, size_t j) noexcept {
    assert(i < n && j < m);
    auto update_mask = [](u32& mask, const size_t i1,
                          const size_t j1) noexcept {
        if (i1 >= n || j1 >= m) [[unlikely]] {
            return;
        }
        const Engine d = dp[i1][j1];
        assert(correct_engine(d));
        const size_t idx = u32(d);
        mask |= 1u << idx;
    };

    u32 mask = 0;
    update_mask(mask, i - 1, j);
    update_mask(mask, i + 1, j);
    update_mask(mask, i, j - 1);
    update_mask(mask, i, j + 1);
    const Engine max_engine = Engine(std::countr_one(mask));
    assert(correct_engine(max_engine));
    return max_engine;
};

inline constexpr u64 kTotalIters = u64(5e5);
using flt = double;

static i64 solve(uint_fast32_t rndseed, const flt t_gamma,
                 const flt min_gamma) noexcept {
    assert(0 < min_gamma && min_gamma < t_gamma && t_gamma < 1);

    u32 x = 0;
    u32 y = 0;
    static_assert(int(Engine::kFirst) == 0);
    memset(&dp, 0, sizeof(dp));
    i64 ans    = n * m * p[u32(Engine::kFirst)];
    i64 maxans = ans;
    mt19937 rnd{rndseed};
    uniform_real_distribution<double> dist(0, 1);
    flt t_k = 1;
    // ranlux48 rnd{rndseed};

    // __attribute__((assume(t_gamma < 1)));
    // __attribute__((assume(0 < t_gamma)));
    // __attribute__((assume(min_gamma < 1)));
    // __attribute__((assume(0 < min_gamma)));
    // __attribute__((assume(min_gamma < t_gamma)));
    for (u64 iters = kTotalIters; iters > 0;
         t_k       = max(t_k * t_gamma, min_gamma), iters--) {
        u32 new_x = 0;
        u32 new_y = 0;
        do {
            // const uint_fast32_t word = rnd();
            // static_assert(sizeof(word) == 8);
            u32 shift = rnd() % 2 == 0 ? 1u : -1u;
            bool is_x = rnd() % 2 == 0;
            new_x     = is_x ? x + shift : x;
            new_y     = is_x ? y : y + shift;
        } while (new_x >= n || new_y >= m);

        const Engine e        = get_max_engine(new_x, new_y);
        const auto curr_power = p[u32(dp[new_x][new_y])];
        // __attribute__((assume(curr_power <= 5)));
        // __attribute__((assume(1 <= curr_power)));
        __builtin_assume(curr_power <= 5);
        __builtin_assume(1 <= curr_power);
        const auto new_power = p[u32(e)];
        // __attribute__((assume(new_power <= 5)));
        // __attribute__((assume(1 <= new_power)));
        __builtin_assume(new_power <= 5);
        __builtin_assume(1 <= new_power);
        const auto delta = new_power - curr_power;
        if (delta >= 0 || dist(rnd) <= std::exp(delta / t_k)) {
            x        = new_x;
            y        = new_y;
            dp[x][y] = e;
            ans += delta;
            maxans = max(maxans, ans);
        }
    }

    // printf("seed = %luul, | ans = %ld | maxans = %ld | temp = %.30e\n",
    // rndseed, ans, maxans, t_k);
    return maxans;
}

int main() {
    srand(u32(time(nullptr)));
    const auto initialseed = u32(rand());
    cout << setprecision(20) << "initialseed=" << initialseed << std::endl;
    mt19937 rnd{initialseed};

    i64 maxmaxans                = -1;
    uint_fast32_t maxmaxans_seed = 0;
    flt maxmaxans_gamma       = -1;
    flt maxmaxans_min_gamma   = -1;

    for (const flt t_gamma : {0.85L, 0.9L, 0.95L, 0.99L, 0.999L, 0.9999L, 0.99999L,
                           0.999999L, 0.9999999L, 0.99999999L, 0.999999999L,
                           0.9999999999L, 0.99999999999L, 0.999999999999L,
                           0.9999999999999L}) {
        for (const flt min_gamma :
             {1e-3L, 1e-4L, 1e-5L, 5e-6L, 1e-6L, 5e-6L, 1e-7L, 5e-8L, 4e-8L, 3e-8L,
              2e-8L, 1e-8L, 9e-9L, 8e-9L, 7e-9L, 6e-9L, 5e-9L, 1e-9L, 5e-10L}) {
            for (auto iter = 4096u; iter > 0; iter--) {
                const auto rndseed = rnd();
                auto maxans        = solve(rndseed, t_gamma, min_gamma);
                if (maxans > maxmaxans) {
                    maxmaxans           = maxans;
                    maxmaxans_seed      = rndseed;
                    maxmaxans_gamma     = t_gamma;
                    maxmaxans_min_gamma = min_gamma;
                }
            }
        }
    }

    cout << "| maxmaxans_seed = " << maxmaxans_seed << " | " << maxmaxans
         << " | " << maxmaxans_gamma << " | " << maxmaxans_min_gamma
         << " |\n";
}
