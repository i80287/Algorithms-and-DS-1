#include <unistd.h>

#include <algorithm>
#include <any>
#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <cinttypes>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <future>
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
#include <source_location>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

using std::min, std::max, std::uniform_int_distribution,
    std::uniform_real_distribution, std::setprecision;
using std::vector, std::array, std::cin, std::cout, std::conditional_t,
    std::mt19937, std::ranlux24, std::ranlux48;
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

// inline constexpr size_t n        = 5;
// inline constexpr size_t m        = 5;
// inline constexpr size_t kMaxW    = 85;
// inline constexpr array<i32, 5> p = {1, 2, 3, 4, 5};

// inline constexpr size_t n        = 30;
// inline constexpr size_t m        = 30;
// inline constexpr size_t kMaxW    = 3413;
// inline constexpr array<i32, 5> p = {1, 2, 3, 4, 5};

inline constexpr size_t n        = 4;
inline constexpr size_t m        = 4;
inline constexpr size_t kMaxW    = 149041;
inline constexpr array<i32, 5> p = {1, 16, 256, 4096, 65536};

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

static i64 solve(uint_fast32_t rndseed, const flt t_gamma,
                 const flt min_gamma) noexcept {
    assert(0 < min_gamma && min_gamma < t_gamma && t_gamma < 1);

    static_assert(int(Engine::kFirst) == 0);
    memset(&dp, 0, sizeof(dp));
    i64 ans    = n * m * p[u32(Engine::kFirst)];
    i64 maxans = ans;
    rnd_t rnd{rndseed};
    uniform_real_distribution<double> dist(0, 1);
    flt t_k = 1;

    for (u32 iters = kTotalIters; iters > 0;
         t_k       = max(t_k * t_gamma, min_gamma), iters--) {
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
        const auto delta = new_power - curr_power;
        const auto prob  = std::exp(delta / t_k);
        if (delta >= 0 || dist(rnd) <= prob) {
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
    rnd_t rnd{initialseed};

    printf("filename=%s,initialseed=%u,kUseMt19937=%u\n",
           std::source_location::current().file_name(), initialseed,
           kUseMt19937);

    i64 maxans                = -1;
    uint_fast32_t maxans_seed = 0;
    flt maxans_gamma          = -1;
    flt maxans_min_gamma      = -1;

    for (const flt t_gamma :
         {0.75, 0.8, 0.85, 0.9, 0.95, 0.99, 0.999, 0.9999, 0.99999,
          0.999999, 0.9999999, 0.99999999, 0.999999999, 0.9999999999,
          0.99999999999, 0.999999999999, 0.9999999999999}) {
        for (const flt min_gamma :
             {1e-3, 1e-4, 1e-5, 5e-6, 1e-6, 5e-6, 1e-7, 5e-8, 4e-8, 3e-8,
              2e-8, 1e-8, 9e-9, 8e-9, 7e-9, 6e-9, 5e-9, 1e-9, 5e-10}) {
            for (auto iter = 256u; iter > 0; iter--) {
                const auto rndseed = rnd();
                auto ans           = solve(rndseed, t_gamma, min_gamma);
                if (ans > maxans) [[unlikely]] {
                    maxans           = ans;
                    maxans_seed      = rndseed;
                    maxans_gamma     = t_gamma;
                    maxans_min_gamma = min_gamma;
                }
            }
        }
    }

    printf("| maxans_seed = %" PRIuFAST32 " | maxans = %" PRId64
           " | maxans_gamma = %.20lf | maxans_min_gamma = %.20e |\n",
           maxans_seed, maxans, maxans_gamma, maxans_min_gamma);
}
