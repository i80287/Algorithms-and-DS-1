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
#include "math_functions.hpp"

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

inline constexpr i32 kTotalIters  = u64(5e5);
using flt                         = double;
inline constexpr bool kUseMt19937 = true;
using rnd_t = conditional_t<kUseMt19937, mt19937, ranlux48>;

static i64 solve(uint_fast32_t rndseed) noexcept {
    static_assert(int(Engine::kFirst) == 0);
    memset(&dp, 0, sizeof(dp));
    i64 ans    = n * m * p[u32(Engine::kFirst)];
    i64 maxans = ans;
    rnd_t rnd{rndseed};
    uniform_real_distribution<double> dist(0, 1);

    for (i32 iters = 1; iters <= kTotalIters; iters++) {
        const u32 new_x       = rnd() % n;
        const u32 new_y       = rnd() % m;
        const Engine e        = get_max_engine(new_x, new_y);
        const auto curr_power = p[u32(dp[new_x][new_y])];
        #if defined(__GNUC__) && !defined(__clang__)
        __attribute__((assume(curr_power <= 5)));
        __attribute__((assume(1 <= curr_power)));
#elif defined(__clang__)
        __builtin_assume(curr_power <= 5);
        __builtin_assume(1 <= curr_power);
#endif
        const auto new_power = p[u32(e)];
#if defined(__GNUC__) && !defined(__clang__)
        __attribute__((assume(new_power <= 5)));
        __attribute__((assume(1 <= new_power)));
#elif defined(__clang__)
        __builtin_assume(new_power <= 5);
        __builtin_assume(1 <= new_power);
#endif
        const auto delta = i64(new_power - curr_power);
        if (delta >= 0 || dist(rnd) <= math_functions::bin_pow(flt(iters), delta)) {
            dp[new_x][new_y] = e;
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
    cout << setprecision(20) << "initialseed=" << initialseed
         << ",kUseMt19937=" << kUseMt19937 << std::endl;
    rnd_t rnd{initialseed};

    i64 maxmaxans                = -1;
    uint_fast32_t maxmaxans_seed = 0;

    for (auto iter = 4096u; iter > 0; iter--) {
        const auto rndseed = rnd();
        auto maxans        = solve(rndseed);
        if (maxans > maxmaxans) [[unlikely]] {
            maxmaxans           = maxans;
            maxmaxans_seed      = rndseed;
        }
    }

    cout << "| maxmaxans_seed = " << maxmaxans_seed << " | " << maxmaxans
         << " |\n";
}
