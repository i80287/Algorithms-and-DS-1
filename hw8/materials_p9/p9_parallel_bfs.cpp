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

// Test 1
inline constexpr size_t n        = 5;
inline constexpr size_t m        = 5;
inline constexpr u32 kMaxW       = 85;
inline constexpr array<i32, 5> p = {1, 2, 3, 4, 5};

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
// inline constexpr size_t n        = 4;
// inline constexpr size_t m        = 4;
// inline constexpr u32 kMaxW       = 149041;
// inline constexpr array<i32, 5> p = {1, 16, 256, 4096, 65536};

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
inline constexpr i32 kTotalIters              = u64(100);  // u64(5e5);
inline constexpr std::size_t kMaxQueueSize    = 1e5;
inline constexpr bool kUseDownEngineHeuristic = true;
inline constexpr flt kSecondDownBoundary      = 0.2;
inline constexpr flt kThirdDownBoundary       = 0.1;
inline constexpr std::size_t kTopKBestResults = 10;
#ifdef __INTELLISENSE__
#pragma diag_suppress 304
#pragma diag_suppress 605
#endif
using rnd_t = mt19937;

static std::pair<i64, u64> solve(uint_fast32_t rndseed, const flt t_gamma, const flt min_gamma,
                                 const flt first_down_boundary) noexcept {
    assert(0 < min_gamma && min_gamma < t_gamma && t_gamma < 1);

    constexpr auto plocal(p);

    i64 max_ans      = -1;
    u64 max_ans_iter = 0;
    rnd_t rnd{rndseed};
    uniform_real_distribution<double> dist(0, 1);

    struct State final {
        array<array<Engine, m>, n> dp{};
        u64 iteration = 1;
        double t_k    = 1;
        i64 ans       = n * m * plocal[u32(Engine::kFirst)];
    };
    static_assert((sizeof(State) & (sizeof(State) - 1)) == 0);

    std::deque<State> queue;
    queue.push_back(State{});
    do {
        State st = queue.front();
        queue.pop_front();

        const u32 i = rnd() % n;
        const u32 j = rnd() % m;

        u32 mask = 0;
        if (i > 0) {
            mask |= 1u << u32(st.dp[i - 1][j]);
        }
        if (i + 1 < n) {
            mask |= 1u << u32(st.dp[i + 1][j]);
        }
        if (j > 0) {
            mask |= 1u << u32(st.dp[i][j - 1]);
        }
        if (j + 1 < m) {
            mask |= 1u << u32(st.dp[i][j + 1]);
        }
        const Engine max_engine = Engine(std::countr_one(mask));
        assert(correct_engine(max_engine));
        std::size_t new_states_size = 1;

        if constexpr (kUseDownEngineHeuristic) {
            Engine e = max_engine;
            if (e > Engine::kFirst && dist(rnd) <= first_down_boundary) {
                e = Engine(u32(e) - 1);
                new_states_size++;
                if (e > Engine::kFirst && dist(rnd) <= kSecondDownBoundary) {
                    e = Engine(u32(e) - 1);
                    new_states_size++;
                    if (e > Engine::kFirst && dist(rnd) <= kThirdDownBoundary) {
                        new_states_size++;
                    }
                }
            }
        }

        if (new_states_size > 4) {
            __builtin_unreachable();
        }

        const auto curr_power = plocal[u32(st.dp[i][j])];
        for (u32 i_e = 0; i_e < new_states_size; i_e++) {
            Engine e = Engine(u32(max_engine) - i_e);
            assert(u32(e) < plocal.size());
            const auto new_power = plocal[u32(e)];
            const auto delta     = new_power - curr_power;
            if (delta >= 0 || dist(rnd) <= std::exp(delta / st.t_k)) {
                const auto new_st_ans = st.ans + delta;
                if (new_st_ans > max_ans) {
                    max_ans      = new_st_ans;
                    max_ans_iter = st.iteration + 1;
                }
                if (st.iteration >= kTotalIters || queue.size() >= kMaxQueueSize) {
                    break;
                }

                if (new_st_ans * (kTotalIters - i32(st.iteration)) < 20 * kMaxW) {
                    continue;
                }

                queue
                    .emplace_back(st.dp, st.iteration + 1, max(st.t_k * t_gamma, min_gamma),
                                  new_st_ans)
                    .dp[i][j] = e;
            }
        }
    } while (!queue.empty());

    return {max_ans, max_ans_iter};
}

struct SearchResult final {
    i64 max_ans = -1;
    uint_fast32_t max_ans_seed{};
    flt max_ans_gamma{};
    flt max_ans_min_gamma{};
    u64 max_ans_iter{};
    flt max_ans_first_down_boundary{};

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

template <std::size_t N>
static std::array<SearchResult, kTopKBestResults> runner(
    const std::array<flt, N> gamma_values) noexcept {
    rnd_t rnd{uint_fast32_t(get_seed())};

    i64 max_ans = -1;
    std::array<SearchResult, kTopKBestResults> best_results{};
    std::size_t best_results_size = 0;

    // static constinit array<array<Engine, m>, n> dp{}; static_assert(sizeof(dp) > 512);

    for (const flt min_gamma :
         {1e-3, 1e-4, 5e-4, 1e-5, 3e-5, 5e-6, 1e-6, 5e-6, 1e-7, 5e-8, 4e-8, 3e-8,
          2e-8, 1e-8, 9e-9, 8e-9, 7e-9, 6e-9, 5e-9, 4e-9, 3e-9, 2e-9, 1e-9, 5e-10}) {
        for (const flt gamma : gamma_values) {
            for (const flt first_down_boundary : {0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3}) {
                for (auto iter = 4u; iter > 0; iter--) {
                    const auto rndseed = rnd();
                    auto [ans, ans_iter] =
                        solve(rndseed, gamma, min_gamma, first_down_boundary /*, dp*/);
                    if (ans < max_ans) {
                        continue;
                    }

                    if (ans > max_ans) {
                        best_results_size = 0;
                        for (SearchResult& sr : best_results) {
                            sr.max_ans = -1;
                        }
                    } else if (best_results_size >= best_results.size()) {
                        continue;
                    }

                    max_ans                           = ans;
                    best_results[best_results_size++] = SearchResult{
                        .max_ans                     = ans,
                        .max_ans_seed                = rndseed,
                        .max_ans_gamma               = gamma,
                        .max_ans_min_gamma           = min_gamma,
                        .max_ans_iter                = ans_iter,
                        .max_ans_first_down_boundary = first_down_boundary,
                    };
                }
            }
        }
    }

    return best_results;
}

inline constexpr std::size_t kPackSize = 16;

template <std::size_t N>
static std::vector<SearchResult> gsolve(const std::array<flt, N>& gamma_values) {
    printf("Total: %zu gamma values\n", std::size(gamma_values));

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

    printf("Starting %zu threads\n", started_threads_count);

    std::vector<SearchResult> best_result;
    best_result.reserve(started_threads_count * kTopKBestResults);
    i64 best_ans = 0;

    for (u32 thread_num = 0; TFuture & handle_future : thread_handles) {
        thread_num++;

        if (!handle_future.valid()) {
            if (started_threads_count == kPackSize) {
                fprintf(stderr, "WARNING: invalid future from thread %u\n", thread_num);
            }
            continue;
        }
        auto results_arr      = handle_future.get();
        const auto thread_ans = results_arr.front().max_ans;
        if (thread_ans < best_ans) {
            fprintf(stderr, "INFO: thread %u has result %" PRId64 " < best_ans = %" PRId64 "\n",
                    thread_num, thread_ans, best_ans);
            continue;
        } else if (thread_ans > best_ans) {
            best_result.clear();
        }

        best_ans = thread_ans;
        const auto end_iter =
            std::find_if(results_arr.begin(), results_arr.end(),
                         [](const SearchResult& sr) constexpr noexcept { return sr.max_ans < 0; });
        std::move(results_arr.begin(), end_iter, std::back_inserter(best_result));
    }

    return best_result;
}

#include <sys/resource.h>

static int setup() {
    if (freopen("res1", "a+", stdout) == nullptr) {
        perror("freopen");
        return EXIT_FAILURE;
    }
    if (setpriority(PRIO_PROCESS, 0, -NZERO / 2)) {
        perror("setpriority");
        return EXIT_FAILURE;
    }
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
    printf(
        "\n"
        "filename=%s\n"
        "kTotalIters=%d\n"
        "kPackSize=%zu\n"
        "kUseDownEngineHeuristic=%u\n"
        "kSecondDownBoundary=%lf\n"
        "kThirdDownBoundary=%lf\n"
        "kTopKBestResults=%zu\n"
        "kMaxQueueSize=%zu\n\n",
        std::source_location::current().file_name(), kTotalIters, kPackSize,
        kUseDownEngineHeuristic, kSecondDownBoundary, kThirdDownBoundary, kTopKBestResults,
        kMaxQueueSize);

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

    auto best_result = gsolve(kGammaValues);
    i64 max_ans      = best_result.front().max_ans;
    auto sc          = flt(max_ans) / kMaxW;

    // clang-format off
    printf("max_ans = %" PRId64 "\n"
           "potential_score = %.20lf\n\n",
           max_ans, sc * sc);

    for (u32 sol_no = 0; const SearchResult& sr : best_result) {
        sol_no++;
        if (sr.max_ans != max_ans) {
            fprintf(stderr, "ERROR: invalid max ans %" PRId64 " in solution %" PRIu32 "\n", sr.max_ans, sol_no);
            continue;
        }

        printf("solution number = %" PRIu32 ":\n"
               "    maxans_seed = %" PRIuFAST32 "\n"
               "    max_ans_gamma = %.20lf\n"
               "    max_ans_min_gamma = %.20e\n"
               "    max_ans_iter = %" PRIu64 "\n"
               "    max_ans_first_down_boundary = %lf\n",
               sol_no, sr.max_ans_seed, sr.max_ans_gamma,
               sr.max_ans_min_gamma, sr.max_ans_iter,
               sr.max_ans_first_down_boundary);
    }
    // clang-format on
}
