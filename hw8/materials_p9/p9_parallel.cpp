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
#include <ranges>
#include <set>
#include <source_location>
#include <span>
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

inline constexpr u32 kTestNumber = 1;
inline constexpr size_t n        = 5;
inline constexpr size_t m        = 5;
inline constexpr size_t kMaxW    = 85;
inline constexpr array<i32, 5> p = {1, 2, 3, 4, 5};

// inline constexpr u32 kTestNumber = 2;
// inline constexpr size_t n        = 30;
// inline constexpr size_t m        = 30;
// inline constexpr size_t kMaxW    = 3413;
// inline constexpr array<i32, 5> p = {1, 2, 3, 4, 5};

// inline constexpr u32 kTestNumber = 6;
// inline constexpr size_t n        = 3;
// inline constexpr size_t m        = 3;
// inline constexpr size_t kMaxW    = 7633;
// inline constexpr array<i32, 5> p = {1, 9, 81, 729, 6561};

// inline constexpr u32 kTestNumber = 7;
// inline constexpr size_t n        = 4;
// inline constexpr size_t m        = 4;
// inline constexpr u32 kMaxW       = 149041;
// inline constexpr array<i32, 5> p = {1, 16, 256, 4096, 65536};

// inline constexpr u32 kTestNumber = 8;
// inline constexpr size_t n        = 8;
// inline constexpr size_t m        = 9;
// inline constexpr size_t kMaxW    = 231012;
// inline constexpr array<i32, 5> p = {1, 10, 100, 1000, 10000};

// inline constexpr u32 kTestNumber = 9;
// inline constexpr size_t n        = 167;
// inline constexpr size_t m        = 284;
// inline constexpr size_t kMaxW    = 54813600;
// inline constexpr array<i32, 5> p = {100, 250, 550, 1000, 2000};

// inline constexpr u32 kTestNumber = 10;
// inline constexpr size_t n        = 256;
// inline constexpr size_t m        = 256;
// inline constexpr size_t kMaxW    = 3732549;
// inline constexpr array<i32, 5> p = {13, 21, 34, 55, 89};

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

using flt                                     = double;
inline constexpr u32 kTotalIters              = 700;  // u64(5e5);
inline constexpr bool kUseDownEngineHeuristic = true;
inline constexpr u32 kItersPerCombination     = 64;
inline constexpr std::size_t kTopKBestResults = 12;
inline constexpr u32 kSeedDiscards            = 128;
#ifdef __INTELLISENSE__
#pragma diag_suppress 304
#pragma diag_suppress 605
#endif
using rnd_t = mt19937;

inline constexpr std::array kMinGammaValues = {
    // 1e-3, 1e-4, 1e-5, 5e-6, 1e-6,
    5e-6, 1e-7, 5e-8, 3e-8, 2e-8, 1e-8, 9e-9,  8e-9,  7e-9,
    6e-9, 5e-9, 4e-9, 3e-9, 2e-9, 1e-9, 5e-10, 1e-10, 5e-11,
};
inline constexpr std::array kFirstDownBoundaryValues = {
    0.99, 0.95, 0.9, 0.85, 0.8, 0.77, 0.75, 0.73, 0.7, 0.67, 0.6, 0.5,
    //  0.4,  0.3
    // , -1.0
};
inline constexpr std::array kSecondDownBoundaryValues = {
    0.55, 0.5, 0.4, 0.3, 0.2,
    // 0.1, 0.05, 0.001,
};
inline constexpr std::array kThirdDownBoundaryValues = {
    // 0.4,
    0.3,
    0.2,
    0.1,
    0.05,
};
inline constexpr std::array kEngDownTempBoundaryValues = {
    // 1e-5, 1e-6, 1e-7,
    1e-8, 1e-9, 1e-10, 1e-11, -1.0,
};

// #define USE_STATIC_STORAGE_FOR_DP
// #define USE_MAX_ANS_PER_ITER_METRICS

struct SolveResult {
    i64 max_ans;
    u32 max_ans_iters;
#ifdef USE_MAX_ANS_PER_ITER_METRICS
    u32 iters_2500_max_ans{};
    u32 iters_5000_max_ans{};
    u32 iters_20000_max_ans{};
    u32 iters_40000_max_ans{};
#endif
};

static SolveResult solve(const uint_fast32_t rndseed, const flt t_gamma, const flt min_gamma,
                         const flt first_down_boundary, const flt second_down_boundary,
                         const flt third_down_boundary, const flt eng_down_temp_boundary
#ifdef USE_STATIC_STORAGE_FOR_DP
                         ,
                         array<array<Engine, m>, n>& dp
#endif
                         ) noexcept {
    assert(0 < min_gamma && min_gamma < t_gamma && t_gamma < 1);

    static_assert(int(Engine::kFirst) == 0);

#ifdef USE_STATIC_STORAGE_FOR_DP
    dp.fill(std::array<Engine, m>{});
    // static_assert(sizeof(dp) > 8192);
#else
    array<array<Engine, m>, n> dp{};
    // static_assert(sizeof(dp) <= 8192);
#endif

    i64 ans          = n * m * p[u32(Engine::kFirst)];
    i64 max_ans      = ans;
    u32 max_ans_iter = 0;
    rnd_t rnd{rndseed};
    uniform_real_distribution<double> dist(0, 1);
    flt t_k = 1;
#ifdef USE_MAX_ANS_PER_ITER_METRICS
    u32 iters_2500_max_ans{};
    u32 iters_5000_max_ans{};
    u32 iters_20000_max_ans{};
    u32 iters_40000_max_ans{};
#endif

    for (u32 iters = 1; iters <= kTotalIters; t_k = max(t_k * t_gamma, min_gamma), iters++) {
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
        Engine max_engine = Engine(std::countr_one(mask));
        assert(correct_engine(max_engine));

        if constexpr (kUseDownEngineHeuristic) {
            if (max_engine > Engine::kFirst && t_k > eng_down_temp_boundary &&
                dist(rnd) <= first_down_boundary) {
                max_engine = Engine(u32(max_engine) - 1);
                if (max_engine > Engine::kFirst && dist(rnd) <= second_down_boundary) {
                    max_engine = Engine(u32(max_engine) - 1);
                    if (max_engine > Engine::kFirst && dist(rnd) <= third_down_boundary) {
                        max_engine = Engine(u32(max_engine) - 1);
                    }
                }
            }
        }

        if (dp[i][j] == max_engine) {
            continue;
        }

        const auto curr_power = p[u32(dp[i][j])];
        const auto new_power  = p[u32(max_engine)];
        const auto delta      = new_power - curr_power;
        if (delta >= 0 || dist(rnd) <= std::exp(delta / t_k)) {
            dp[i][j] = max_engine;
            ans += delta;
            if (ans > max_ans) {
                max_ans      = ans;
                max_ans_iter = iters;
            }
        }
#ifdef USE_MAX_ANS_PER_ITER_METRICS
        if (iters == 2500) [[unlikely]] {
            iters_2500_max_ans = u32(max_ans);
        }

        if (iters == 5000) [[unlikely]] {
            iters_5000_max_ans = u32(max_ans);
        }
        if (iters == 20000) [[unlikely]] {
            iters_20000_max_ans = u32(max_ans);
        }
        if (iters == 40000) [[unlikely]] {
            iters_40000_max_ans = u32(max_ans);
        }
#endif
    }

    return {
        .max_ans       = max_ans,
        .max_ans_iters = max_ans_iter,
#ifdef USE_MAX_ANS_PER_ITER_METRICS
        .iters_2500_max_ans  = iters_2500_max_ans,
        .iters_5000_max_ans  = iters_5000_max_ans,
        .iters_20000_max_ans = iters_20000_max_ans,
        .iters_40000_max_ans = iters_40000_max_ans,
#endif
    };
}

struct SearchResult final {
    i64 max_ans = -1;
    uint_fast32_t max_ans_seed{};
    flt max_ans_gamma{};
    flt max_ans_min_gamma{};
    u32 max_ans_iter{};
    flt max_ans_first_down_boundary{};
    flt max_ans_second_down_boundary{};
    flt max_ans_third_down_boundary{};
    flt eng_down_temp_boundary{};
#ifdef USE_MAX_ANS_PER_ITER_METRICS
    u32 iters_2500_max_ans{};
    u32 iters_5000_max_ans{};
    u32 iters_20000_max_ans{};
    u32 iters_40000_max_ans{};
#endif

    constexpr bool operator==(const SearchResult& other) const noexcept {
        return max_ans == other.max_ans;
    }
    constexpr std::weak_ordering operator<=>(const SearchResult& other) const noexcept {
        if (max_ans != other.max_ans) {
            return max_ans <=> other.max_ans;
        }
        return other.max_ans_iter <=> max_ans_iter;
    }
};

ATTRIBUTE_COLD
static uint64_t get_seed_fallback() noexcept {
    fprintf(stderr, "Calling %s\n", std::source_location::current().function_name());
    return u32(rand()) ^ uint64_t(time(nullptr));
}

static uint64_t get_seed() noexcept {
    try {
        std::ifstream fin("/dev/urandom", std::ios::in | std::ios::binary);
        if (!fin.is_open() || !fin) [[unlikely]] {
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

template <std::size_t N>
static std::array<SearchResult, kTopKBestResults> runner(
    const std::array<flt, N> gamma_values) noexcept {
    rnd_t rnd{uint_fast32_t(get_seed())};

    i64 max_ans = -1;
    std::array<SearchResult, kTopKBestResults> best_results{};
    std::size_t best_results_size = 0;

#ifdef USE_STATIC_STORAGE_FOR_DP
    static constinit thread_local array<array<Engine, m>, n> dp{};
    static_assert(sizeof(dp) > 8192);
#endif

    for (uint32_t iter = kItersPerCombination; iter > 0; iter--) {
        for (const flt gamma : gamma_values) {
            for (const flt min_gamma : kMinGammaValues) {
                for (const flt first_down_boundary : kFirstDownBoundaryValues) {
                    for (const flt second_down_boundary : kSecondDownBoundaryValues) {
                        for (const flt third_down_boundary : kThirdDownBoundaryValues) {
                            for (const flt eng_down_temp_boundary : kEngDownTempBoundaryValues) {
                                const auto rndseed = rnd();
                                const auto [ans, ans_iter
#ifdef USE_MAX_ANS_PER_ITER_METRICS
                                            ,
                                            iters_20000_max_ans, iters_40000_max_ans,
                                            iters_2500_max_ans, iters_5000_max_ans
#endif
                                ] = solve(rndseed, gamma, min_gamma, first_down_boundary,
                                          second_down_boundary, third_down_boundary,
                                          eng_down_temp_boundary
#ifdef USE_STATIC_STORAGE_FOR_DP
                                          ,
                                          dp
#endif
                                );
                                if (ans * 10 < max_ans * 9) {
                                    continue;
                                }

                                if (ans > max_ans) {
                                    best_results_size = 0;
                                } else if (best_results_size >= best_results.size()) {
                                    continue;
                                }

                                max_ans                           = std::max(max_ans, ans);
                                best_results[best_results_size++] = SearchResult{
                                    .max_ans                      = ans,
                                    .max_ans_seed                 = rndseed,
                                    .max_ans_gamma                = gamma,
                                    .max_ans_min_gamma            = min_gamma,
                                    .max_ans_iter                 = ans_iter,
                                    .max_ans_first_down_boundary  = first_down_boundary,
                                    .max_ans_second_down_boundary = second_down_boundary,
                                    .max_ans_third_down_boundary  = third_down_boundary,
                                    .eng_down_temp_boundary       = eng_down_temp_boundary,
#ifdef USE_MAX_ANS_PER_ITER_METRICS
                                    .iters_2500_max_ans  = iters_2500_max_ans,
                                    .iters_5000_max_ans  = iters_5000_max_ans,
                                    .iters_20000_max_ans = iters_20000_max_ans,
                                    .iters_40000_max_ans = iters_40000_max_ans,
#endif
                                };
                            }
                        }
                    }
                }
            }
        }

        for (u32 i = kSeedDiscards; i > 0; i--) {
            rnd();
        }
    }
    return best_results;
}

inline constexpr std::size_t kPackSize = 10;

template <std::size_t N>
static std::vector<SearchResult> gsolve(const std::array<flt, N>& gamma_values) {
    using TFuture = std::future<std::array<SearchResult, kTopKBestResults>>;
    std::array<TFuture, kPackSize> thread_handles{};
    constexpr std::size_t kValsPerThread = (N + kPackSize - 1) / kPackSize;

    std::size_t started_threads_count = 0;
    for (std::size_t i = 0; TFuture & handle_future : thread_handles) {
        if (i >= gamma_values.size()) {
            break;
        }

        std::array<flt, kValsPerThread> thread_gamma_values;
        thread_gamma_values.fill(gamma_values[i]);

        const auto copy_begin = gamma_values.begin() + i;
        i                     = std::min(i + kValsPerThread, gamma_values.size());
        const auto copy_end   = gamma_values.begin() + i;
        std::copy(copy_begin, copy_end, thread_gamma_values.begin());

        handle_future =
            std::async(std::launch::async, &runner<kValsPerThread>, thread_gamma_values);
        started_threads_count++;
    }

    printf(
        "    gamma_values_size = %zu\n"
        "    started_threads = %zu\n",
        std::size(gamma_values), started_threads_count);

    std::vector<SearchResult> best_results;
    constexpr std::size_t kMaxEntries = 8;
    best_results.reserve(std::max(kMaxEntries, started_threads_count * kTopKBestResults));
    i64 best_ans = 0;

    for (u32 thread_num = 0; TFuture & handle_future : thread_handles) {
        thread_num++;
        if (!handle_future.valid()) {
            if (started_threads_count == kPackSize) {
                fprintf(stderr, "WARNING: invalid future from thread %u\n", thread_num);
            }
            continue;
        }
        auto results_arr           = handle_future.get();
        const auto thread_best_ans = std::ranges::max_element(results_arr)->max_ans;
        if (thread_best_ans < i64(i32(best_ans) * 0.9)) {
            fprintf(stderr, "INFO: thread %u has result %" PRId64 " < %" PRId64 "\n", thread_num,
                    thread_best_ans, i64(i32(best_ans) * 0.9));
            continue;
        }

        best_ans = max(best_ans, thread_best_ans);
        const auto end_iter =
            std::find_if(results_arr.begin(), results_arr.end(),
                         [](const SearchResult& sr) constexpr noexcept { return sr.max_ans < 0; });
        std::move(results_arr.begin(), end_iter, std::back_inserter(best_results));
        std::partial_sort(
            best_results.begin(),
            best_results.begin() + std::ptrdiff_t(std::min(best_results.size(), kMaxEntries)),
            best_results.end(),
            [](const SearchResult& sr1, const SearchResult& sr2) constexpr noexcept {
                return sr2 < sr1;
            });
        if (best_results.size() > kMaxEntries) {
            best_results.resize(kMaxEntries);
        }
    }

    return best_results;
}

#include <sys/resource.h>

static int setup() noexcept {
    {
        auto s = "res" + std::to_string(kTestNumber) + ".lua";
        if (freopen(s.c_str(), "a+", stdout) == nullptr) {
            perror("freopen");
            return EXIT_FAILURE;
        }
    }
    // if (setpriority(PRIO_PROCESS, 0, -NZERO)) {
    //     perror("setpriority");
    //     return EXIT_FAILURE;
    // }
    rlimit rl{};
    if (getrlimit(RLIMIT_STACK, &rl)) {
        perror("getrlimit");
        return EXIT_FAILURE;
    }
    rl.rlim_cur = std::max(rl.rlim_cur, rlim_t(128) << 20);
    if (setrlimit(RLIMIT_STACK, &rl)) {
        perror("setrlimit");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main() {
    if (int ret = setup(); ret) {
        return ret;
    }

    srand(u32(time(nullptr)));
    time_t start = time(nullptr);
    assert(start != time_t(-1));
    {
        auto tostr = [](const auto& arr) {
            std::string s;
            s.reserve(100);
            s += '[';
            for (double f : arr) {
                s += __gnu_cxx::__to_xstring<std::string>(
                    &std::vsnprintf, std::numeric_limits<double>::max_exponent10 + 20,
                    std::abs(f) > 1e-6 ? "%lf" : "%e", f);
                s += ',';
            }
            s += ']';
            return s;
        };
        auto mgv   = tostr(kMinGammaValues);
        auto fdbv  = tostr(kFirstDownBoundaryValues);
        auto sdbv  = tostr(kSecondDownBoundaryValues);
        auto tdbv  = tostr(kThirdDownBoundaryValues);
        auto edtbv = tostr(kEngDownTempBoundaryValues);
        printf(
            "{\n"
            "    start_time = %s"
            "    filename = \"%s\"\n"
            "    kTotalIters = %u\n"
            "    kPackSize = %zu\n"
            "    kItersPerCombination = %u\n"
            "    kUseDownEngineHeuristic = %u\n"
            "    kTopKBestResults = %zu\n"
            "    kMinGammaValues = %s\n"
            "    kFirstDownBoundaryValues = %s\n"
            "    kSecondDownBoundaryValues = %s\n"
            "    kThirdDownBoundaryValues = %s\n"
            "    kEngDownTempBoundaryValues = %s\n"
            "    solve(...) calls per gamma value = %zu\n", asctime(localtime(&start)),
            std::source_location::current().file_name(), kTotalIters, kPackSize,
            kItersPerCombination, kUseDownEngineHeuristic, kTopKBestResults, mgv.c_str(),
            fdbv.c_str(), sdbv.c_str(), tdbv.c_str(), edtbv.c_str(),
            kItersPerCombination * kMinGammaValues.size() * kFirstDownBoundaryValues.size() *
                kSecondDownBoundaryValues.size() * kThirdDownBoundaryValues.size() *
                kEngDownTempBoundaryValues.size());
    }

    constexpr std::array kGammaValues = {
        0.15,
        0.16,
        0.2,
        0.21,
        0.22,
        0.23,
        0.24,
        0.25,
        0.26,
        0.27,
        0.28,
        0.29,
        0.3,
        0.31,
        0.32,
        0.33,
        0.34,
        0.35,
        0.36,
        0.37,
        0.38,
        0.39,
        0.4,
        0.41,
        0.42,
        0.43,
        0.44,
        0.45,
        0.46,
        0.47,
        0.48,
        0.49,

        0.5,
        0.51,
        0.52,
        0.53,
        0.54,
        0.56,
        0.57,
        0.59,
        0.6,
        0.61,
        0.63,
        0.65,
        0.67,
        0.68,
        0.69,
        0.7,
        0.71,
        0.72,
        0.73,
        0.74,
        0.75,
        0.76,
        0.77,
        0.79,
        0.8,
        0.81,
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
        0.999999999999999,
    };

    auto best_results = gsolve(kGammaValues);
    i64 max_ans       = std::ranges::max_element(best_results)->max_ans;
    auto sc           = flt(max_ans) / kMaxW;
    time_t end        = time(nullptr);
    assert(end != time_t(-1));
    u64 duration_in_seconds = u64(end - start);

    // clang-format off
    printf("    max_ans = %" PRId64 "\n"
           "    potential_score = %.20lf\n"
           "    duration_in_seconds = %" PRIu64 "\n\n",
           max_ans, sc * sc, duration_in_seconds);

    for (u32 sol_no = 0; const SearchResult& sr : best_results) {
        printf("    solution number = %" PRIu32 ":\n"
               "        max_ans = %" PRId64 "\n"
               "        max_ans_seed = %" PRIuFAST32 "\n"
               "        max_ans_gamma = %.20lf\n"
               "        max_ans_min_gamma = %.20e\n"
               "        max_ans_first_down_boundary = %lf\n"
               "        max_ans_second_down_boundary = %lf\n"
               "        max_ans_third_down_boundary = %lf\n"
               "        eng_down_temp_boundary = %lf\n"
               "        max_ans_iter = %u\n"
#ifdef USE_MAX_ANS_PER_ITER_METRICS
               "        iters_2500_max_ans = %u\n"
               "        iters_5000_max_ans = %u\n"
               "        iters_20000_max_ans = %u\n"
               "        iters_40000_max_ans = %u\n"
#endif
               ,
               sol_no, sr.max_ans, sr.max_ans_seed,
               sr.max_ans_gamma, sr.max_ans_min_gamma,
               sr.max_ans_first_down_boundary,
               sr.max_ans_second_down_boundary,
               sr.max_ans_third_down_boundary,
               sr.eng_down_temp_boundary,
               sr.max_ans_iter
#ifdef USE_MAX_ANS_PER_ITER_METRICS
               ,
               sr.iters_2500_max_ans,
               sr.iters_5000_max_ans,
               sr.iters_20000_max_ans,
               sr.iters_40000_max_ans
#endif
               );
        sol_no++;
    }
    printf("}\n");
    // clang-format on
}