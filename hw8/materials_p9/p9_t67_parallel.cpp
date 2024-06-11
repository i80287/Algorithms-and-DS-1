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
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <future>
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

#include "config_macros.hpp"

using std::min, std::max, std::uniform_int_distribution, std::uniform_real_distribution,
    std::setprecision;
using std::size_t;
using std::uint32_t;
using std::uint64_t;
using std::vector, std::array, std::cin, std::cout, std::conditional_t, std::mt19937, std::ranlux24,
    std::ranlux48;
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

// Test 1
// inline constexpr size_t n        = 5;
// inline constexpr size_t m        = 5;
// inline constexpr size_t kMaxW    = 85;
// inline constexpr array<i32, 5> p = {1, 2, 3, 4, 5};

// Test 2
// inline constexpr size_t n        = 30;
// inline constexpr size_t m        = 30;
// inline constexpr size_t kMaxW    = 3413;
// inline constexpr array<i32, 5> p = {1, 2, 3, 4, 5};

// Test 6
// inline constexpr size_t n        = 3;
// inline constexpr size_t m        = 3;
// inline constexpr size_t kMaxW    = 7633;
// inline constexpr array<i32, 5> p = {1, 9, 81, 729, 6561};

// Test 7
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

constexpr bool correct_engine(Engine e) noexcept {
    return u32(e) - u32(Engine::kFirst) <= u32(Engine::kFifth) - u32(Engine::kFirst);
}

inline constexpr i32 kTotalIters = u64(5e5);
using flt                        = double;
#ifdef __INTELLISENSE__
#pragma diag_suppress 304
#pragma diag_suppress 605
#endif
using rnd_t = mt19937;

static i64 solve(uint_fast32_t rndseed, const flt t_gamma, const flt min_gamma
                 /*, array<array<Engine, m>, n>& dp*/) noexcept {
    assert(0 < min_gamma && min_gamma < t_gamma && t_gamma < 1);

    static_assert(int(Engine::kFirst) == 0);
    array<array<Engine, m>, n> dp{};
    static_assert(sizeof(dp) <= 512);
    // dp.fill(std::array<Engine, m>{});

    constexpr auto plocal(p);

    i64 ans    = n * m * plocal[u32(Engine::kFirst)];
    i64 maxans = ans;
    rnd_t rnd{rndseed};
    uniform_real_distribution<double> dist(0, 1);
    flt t_k = 1;

    for (u32 iters = kTotalIters; iters > 0; t_k = max(t_k * t_gamma, min_gamma), iters--) {
        const u32 i = rnd() % n;
        const u32 j = rnd() % m;

        u32 mask = 0;
        if (i > 0) {
            mask |= 1u << u32(dp[i - 1][j]);
        }
        if (i + 1 < n) {
            mask |= 1u << u32(dp[i + 1][j]);
        }
        if (j > 0) {
            mask |= 1u << u32(dp[i][j - 1]);
        }
        if (j + 1 < m) {
            mask |= 1u << u32(dp[i][j + 1]);
        }
        const Engine max_engine = Engine(std::countr_one(mask));
        assert(correct_engine(max_engine));

        const auto curr_power = plocal[u32(dp[i][j])];
        const auto new_power  = plocal[u32(max_engine)];
        const auto delta      = new_power - curr_power;
        if (delta >= 0 || dist(rnd) <= std::exp(delta / t_k)) {
            dp[i][j] = max_engine;
            ans += delta;
            maxans = max(maxans, ans);
        }
    }

    return maxans;
}

struct SearchResult final {
    i64 max_ans = -1;
    uint_fast32_t max_ans_seed{};
    flt max_ans_gamma{};
    flt max_ans_min_gamma{};

    constexpr std::weak_ordering operator<=>(const SearchResult& other) const noexcept {
        return max_ans <=> other.max_ans;
    }
};

static uint64_t get_seed_fallback() noexcept {
    fprintf(stderr, "Calling %s\n", std::source_location::current().function_name());
    return u32(rand()) ^ uint64_t(time(nullptr));
}

static uint64_t get_seed() noexcept {
    try {
        std::ifstream fin("/dev/urandom", std::ios::in | std::ios::binary);
        if (!fin || !fin.is_open()) {
            return get_seed_fallback();
        }
        uint64_t useed{};
        char* ptr = reinterpret_cast<char*>(&useed);
        fin.read(ptr, sizeof(useed));
        return fin ? useed : get_seed_fallback();
    } catch (const std::exception& ex) {
        fprintf(stderr, "Exception: %s\n", ex.what());
        return get_seed_fallback();
    } catch (...) {
        return get_seed_fallback();
    }
}

template <flt TGammaValue>
    requires(0 < TGammaValue && TGammaValue < 1)
static SearchResult runner() noexcept {
    rnd_t rnd{uint_fast32_t(get_seed())};

    i64 max_ans                = -1;
    uint_fast32_t max_ans_seed = 0;
    flt max_ans_min_gamma      = -1;

    // static constinit array<array<Engine, m>, n> dp{}; static_assert(sizeof(dp) > 512);

    for (const flt min_gamma : {1e-3, 1e-4, 1e-5, 5e-6, 1e-6, 5e-6, 1e-7, 5e-8, 4e-8, 3e-8, 2e-8,
                                1e-8, 9e-9, 8e-9, 7e-9, 6e-9, 5e-9, 1e-9, 5e-10}) {
        for (auto iter = 512u; iter > 0; iter--) {
            const auto rndseed = rnd();
            auto ans           = solve(rndseed, TGammaValue, min_gamma /*, dp*/);
            if (ans > max_ans) [[unlikely]] {
                max_ans           = ans;
                max_ans_seed      = rndseed;
                max_ans_min_gamma = min_gamma;
            }
        }
    }

    return SearchResult{
        .max_ans           = max_ans,
        .max_ans_seed      = max_ans_seed,
        .max_ans_gamma     = TGammaValue,
        .max_ans_min_gamma = max_ans_min_gamma,
    };
}

inline constexpr std::size_t kPackSize = 12;
using ThreadHandleContainer            = std::array<std::future<SearchResult>, kPackSize>;

// template <flt TGammaValue, flt... TGammaValues>
// static void start_runners(ThreadHandleContainer& thread_handles) {
//     thread_handles.emplace_back(std::async(std::launch::async, &runner<TGammaValue>));
//     if constexpr (sizeof...(TGammaValues) > 0) {
//         start_runners<TGammaValues...>(thread_handles);
//     }
// }

template <std::size_t I, std::size_t Offset, std::array TGammaValues>
static std::size_t start_runners(ThreadHandleContainer& thread_handles) {
    if constexpr (I < kPackSize && Offset + I < std::size(TGammaValues)) {
        thread_handles[I] = std::async(std::launch::async, &runner<TGammaValues[Offset + I]>);
        return start_runners<I + 1, Offset, TGammaValues>(thread_handles);
    } else {
        return I;
    }
}

template <std::size_t Offset, std::array TGammaValues>
static SearchResult gsolve_impl() {
    SearchResult best_result{};

    {
        ThreadHandleContainer thread_handles{};
        static_assert(std::size(thread_handles) == kPackSize);
        // start_runners<TGammaValues...>(thread_handles);
        const std::size_t cnt = start_runners<0, Offset, TGammaValues>(thread_handles);
        printf("Starting %zu threads\n", cnt);

        for (std::future<SearchResult>& handle_future : thread_handles) {
            if (!handle_future.valid()) {
                if (cnt == kPackSize) {
                    fputs("Invalid future\n", stderr);
                }
                continue;
            }
            best_result = max(best_result, handle_future.get());
        }
    }

    constexpr std::size_t NextOffset = Offset + kPackSize;
    if constexpr (NextOffset < std::size(TGammaValues)) {
        best_result = max(best_result, gsolve_impl<NextOffset, TGammaValues>());
    }

    return best_result;
}

template <std::array TGammaValues>
static SearchResult gsolve() {
    printf("Total: %zu gamma values\n", std::size(TGammaValues));
    return gsolve_impl<0, TGammaValues>();
}

int main() {
    srand(u32(time(nullptr)));
    printf("filename=%s,kPackSize=%zu\n", std::source_location::current().file_name(), kPackSize);

    SearchResult best_result = gsolve<std::array{0.6,
                                                 0.61,
                                                 0.63,
                                                 0.65,
                                                 0.67,
                                                 0.69,
                                                 0.7,
                                                 0.73,
                                                 0.74,
                                                 0.75,
                                                 0.76,
                                                 0.77,
                                                 0.79,
                                                 0.8,
                                                 0.82,
                                                 0.83,
                                                 0.85,
                                                 0.88,
                                                 0.9,
                                                 0.92,
                                                 0.95,
                                                 0.97,
                                                 0.99,
                                                 0.999,
                                                 0.9999,
                                                 0.99999,
                                                 0.999999,
                                                 0.9999999,
                                                 0.99999999,
                                                 0.999999999,
                                                 0.9999999999,
                                                 0.99999999999,
                                                 0.999999999999,
                                                 0.9999999999999,
                                                 0.99999999999999,
                                                 0.999999999999999}>();

    printf("| maxans_seed = %" PRIuFAST32 " | maxans = %" PRId64
           " | maxans_gamma = %.20lf | maxans_min_gamma = %.20e |\n",
           best_result.max_ans_seed, best_result.max_ans, best_result.max_ans_gamma,
           best_result.max_ans_min_gamma);
}
