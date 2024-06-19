#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <future>
#include <iostream>
#include <random>
#include <ranges>
#include <source_location>
#include <stacktrace>
#include <type_traits>

#include "config_macros.hpp"

using std::size_t, std::uint64_t, std::uint32_t, std::int32_t, std::int64_t, std::array;

#define TEST_NUMBER 1
inline constexpr uint32_t kTestNumber = TEST_NUMBER;

#if TEST_NUMBER == 1
inline constexpr size_t n            = 5;
inline constexpr size_t m            = 5;
inline constexpr size_t kMaxW        = 85;
inline constexpr array<int32_t, 5> p = {1, 2, 3, 4, 5};
#elif TEST_NUMBER == 2
inline constexpr size_t n            = 30;
inline constexpr size_t m            = 30;
inline constexpr size_t kMaxW        = 3413;
inline constexpr array<int32_t, 5> p = {1, 2, 3, 4, 5};
#elif TEST_NUMBER == 7
inline constexpr size_t n            = 4;
inline constexpr size_t m            = 4;
inline constexpr uint32_t kMaxW      = 149041;
inline constexpr array<int32_t, 5> p = {1, 16, 256, 4096, 65536};
#elif TEST_NUMBER == 8
inline constexpr size_t n            = 8;
inline constexpr size_t m            = 9;
inline constexpr size_t kMaxW        = 231012;
inline constexpr array<int32_t, 5> p = {1, 10, 100, 1000, 10000};
#elif TEST_NUMBER == 9;
inline constexpr size_t n            = 167;
inline constexpr size_t m            = 284;
inline constexpr size_t kMaxW        = 54813600;
inline constexpr array<int32_t, 5> p = {100, 250, 550, 1000, 2000};
#elif TEST_NUMBER == 10
inline constexpr size_t n            = 256;
inline constexpr size_t m            = 256;
inline constexpr size_t kMaxW        = 3732549;
inline constexpr array<int32_t, 5> p = {13, 21, 34, 55, 89};
#else
#error "Unknown TEST_NUMBER"
#endif

inline constexpr std::array kGammaValues = {
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
inline constexpr std::array kMinGammaValues = {
    // 1e-3, 1e-4, 1e-5,
    5e-6,
    1e-6,
    5e-6,
    1e-7,
    // 5e-8,
    3e-8,
    // 2e-8,
    1e-8,
    9e-9,
    8e-9,
    7e-9,
    // 6e-9,
    5e-9,
    // 4e-9,
    3e-9,
    2e-9,
    1e-9,
    5e-10,
    1e-10,
    5e-11,
};
inline constexpr std::array kFirstDownBoundaryValues = {
    // 0.99, 0.95,
    0.9, 0.87, 0.85, 0.83, 0.8, 0.77, 0.75, 0.73, 0.7, 0.67, 0.6, 0.5,
    // 0.4,  0.3, -1.0
};
inline constexpr std::array kSecondDownBoundaryValues = {
    0.55,
    0.5,
    0.4,
    0.3,
    0.2,
    0.1,
    // 0.05,
    0.001,
};
inline constexpr std::array kThirdDownBoundaryValues = {
    0.4, 0.3, 0.2, 0.1, 0.05,
};
inline constexpr std::array kEngDownTempBoundaryValues = {
    1e-5,
    1e-6,
    1e-7,
    1e-8,
    // 1e-9,
    1e-10,
    1e-11,
    -1.0,
};
inline constexpr std::array kInitialTempValues = {
    5.0, 4.5, 4.0, 3.5, 3.0, 2.5, 2.0, 1.5, 1.0, 0.5,
};

inline constexpr uint32_t kBlockSize = std::min({n, m, static_cast<decltype(n)>(4)});
static_assert(kBlockSize >= 2);
inline constexpr uint32_t kBlockSkipStep            = 1;
inline constexpr uint32_t kMaxIterationsInBlock     = 550;
inline constexpr uint32_t kIterationsPerPararmsPack = 128;
inline constexpr size_t kThreadsCount               = 10;
static_assert(std::size(kGammaValues) % kThreadsCount == 0);
inline constexpr size_t kGammaValuesPerThread = std::size(kGammaValues) / kThreadsCount;

enum class Engine : uint32_t {
    kZero,
    kFirst,
    kSecond,
    kThird,
    kFourth,
};

using Map      = array<array<Engine, m>, n>;
using MapBlock = array<array<Engine, kBlockSize>, kBlockSize>;

constexpr Engine& operator--(Engine& e) noexcept {
    return e = static_cast<Engine>(static_cast<uint32_t>(e) - 1);
}

struct OptimizeParams {
    uint_fast32_t seed;
    double gamma;
    double min_gamma;
    double first_decrease_probability;
    double second_decrease_probability;
    double third_decrease_probability;
    double min_temperature_for_eng_decrease;
    double initial_temp_value;
};

template <std::size_t N, std::size_t M>
ATTRIBUTE_CONST constexpr Engine max_cell_engine(const array<array<Engine, M>, N>& dp, size_t i,
                                                 size_t j) noexcept {
    uint32_t mask = 0;
    if (i > 0) {
        mask |= 1u << uint32_t(dp[i - 1][j]);
    }
    if (i + 1 < N) {
        mask |= 1u << uint32_t(dp[i + 1][j]);
    }
    if (j > 0) {
        mask |= 1u << uint32_t(dp[i][j - 1]);
    }
    if (j + 1 < M) {
        mask |= 1u << uint32_t(dp[i][j + 1]);
    }
    return Engine(std::countr_one(mask));
}

template <size_t N, size_t M>
constexpr bool compare_dp_power(const array<array<Engine, M>, N>& dp,
                                uint64_t expected_power) noexcept {
    uint64_t total_power = 0;
    for (const auto& row : dp) {
        for (const Engine e : row) {
            total_power += uint32_t(p[size_t(e)]);
        }
    }
    return total_power == expected_power;
}

template <size_t kDepth = 0>
ATTRIBUTE_CONST constexpr uint32_t try_improve_greedily(const MapBlock& dp) noexcept {
    uint32_t max_ans{};
    {
        auto dpc(dp);
        uint32_t ans = 0;
        for (auto i = 0zu; i < kBlockSize; i++) {
            for (auto j = 0zu; j < kBlockSize; j++) {
                dpc[i][j] = std::max(dpc[i][j], max_cell_engine(dpc, i, j));
                ans += uint32_t(p[uint32_t(dpc[i][j])]);
            }
        }
        max_ans = std::max(max_ans, ans);
        if constexpr (kDepth + 1 < 2) {
            max_ans = std::max(max_ans, try_improve_greedily<kDepth + 1>(dpc));
        }
    }
    {
        auto dpc(dp);
        uint32_t ans = 0;
        for (auto i = 0zu; i < kBlockSize; i++) {
            for (size_t j = kBlockSize - 1; j != size_t(-1); j--) {
                dpc[i][j] = std::max(dpc[i][j], max_cell_engine(dpc, i, j));
                ans += uint32_t(p[uint32_t(dpc[i][j])]);
            }
        }
        max_ans = std::max(max_ans, ans);
        if constexpr (kDepth + 1 < 2) {
            max_ans = std::max(max_ans, try_improve_greedily<kDepth + 1>(dpc));
        }
    }
    {
        auto dpc(dp);
        uint32_t ans = 0;
        for (size_t i = kBlockSize - 1; i != size_t(-1); i--) {
            for (auto j = 0zu; j < kBlockSize; j++) {
                dpc[i][j] = std::max(dpc[i][j], max_cell_engine(dpc, i, j));
                ans += uint32_t(p[uint32_t(dpc[i][j])]);
            }
        }
        max_ans = std::max(max_ans, ans);
        if constexpr (kDepth + 1 < 2) {
            max_ans = std::max(max_ans, try_improve_greedily<kDepth + 1>(dpc));
        }
    }
    {
        auto dpc(dp);
        uint32_t ans = 0;
        for (size_t i = kBlockSize - 1; i != size_t(-1); i--) {
            for (size_t j = kBlockSize - 1; j != size_t(-1); j--) {
                dpc[i][j] = std::max(dpc[i][j], max_cell_engine(dpc, i, j));
                ans += uint32_t(p[uint32_t(dpc[i][j])]);
            }
        }
        max_ans = std::max(max_ans, ans);
        if constexpr (kDepth + 1 < 2) {
            max_ans = std::max(max_ans, try_improve_greedily<kDepth + 1>(dpc));
        }
    }

    return max_ans;
}

template <size_t kDepth = 0>
constexpr bool improve_greedily(MapBlock& dp, uint64_t needed_power) noexcept {
    {
        auto dpc(dp);
        uint64_t ans = 0;
        for (auto i = 0zu; i < kBlockSize; i++) {
            for (auto j = 0zu; j < kBlockSize; j++) {
                dpc[i][j] = std::max(dpc[i][j], max_cell_engine(dpc, i, j));
                ans += uint32_t(p[uint32_t(dpc[i][j])]);
            }
        }
        if (ans == needed_power) {
            dp = dpc;
            return true;
        }
        if constexpr (kDepth + 1 < 2) {
            if (improve_greedily<kDepth + 1>(dpc, needed_power)) {
                dp = dpc;
                return true;
            }
        }
    }
    {
        auto dpc(dp);
        uint64_t ans = 0;
        for (auto i = 0zu; i < kBlockSize; i++) {
            for (size_t j = kBlockSize - 1; j != size_t(-1); j--) {
                dpc[i][j] = std::max(dpc[i][j], max_cell_engine(dpc, i, j));
                ans += uint32_t(p[uint32_t(dpc[i][j])]);
            }
        }
        if (ans == needed_power) {
            dp = dpc;
            return true;
        }
        if constexpr (kDepth + 1 < 2) {
            if (improve_greedily<kDepth + 1>(dpc, needed_power)) {
                dp = dpc;
                return true;
            }
        }
    }
    {
        auto dpc(dp);
        uint64_t ans = 0;
        for (size_t i = kBlockSize - 1; i != size_t(-1); i--) {
            for (auto j = 0zu; j < kBlockSize; j++) {
                dpc[i][j] = std::max(dpc[i][j], max_cell_engine(dpc, i, j));
                ans += uint32_t(p[uint32_t(dpc[i][j])]);
            }
        }
        if (ans == needed_power) {
            dp = dpc;
            return true;
        }
        if constexpr (kDepth + 1 < 2) {
            if (improve_greedily<kDepth + 1>(dpc, needed_power)) {
                dp = dpc;
                return true;
            }
        }
    }
    {
        auto dpc(dp);
        uint64_t ans = 0;
        for (size_t i = kBlockSize - 1; i != size_t(-1); i--) {
            for (size_t j = kBlockSize - 1; j != size_t(-1); j--) {
                dpc[i][j] = std::max(dpc[i][j], max_cell_engine(dpc, i, j));
                ans += uint32_t(p[uint32_t(dpc[i][j])]);
            }
        }
        if (ans == needed_power) {
            dp = dpc;
            return true;
        }
        if constexpr (kDepth + 1 < 2) {
            if (improve_greedily<kDepth + 1>(dpc, needed_power)) {
                dp = dpc;
                return true;
            }
        }
    }

    return false;
}

struct SimulatedAnnealingResult {
    uint64_t max_power;
    uint64_t max_power_iteration;
};

static SimulatedAnnealingResult apply_simulated_annealing(const MapBlock& original_block_dp,
                                                          const OptimizeParams& params) noexcept {
                                                            MapBlock block_dp(original_block_dp);
    std::mt19937 rnd{params.seed};
    std::uniform_real_distribution<double> dist(0, 1);

    uint64_t total_power = 0;
    for (const auto& row : block_dp) {
        for (const Engine e : row) {
            total_power += uint32_t(p[size_t(e)]);
        }
    }
    uint64_t max_power          = total_power;
    size_t max_power_iteration = 0;
    double t_k                 = 1;
    for (size_t iteration = 1; iteration <= kMaxIterationsInBlock;
         t_k              = std::max(t_k * params.gamma, params.min_gamma), iteration++) {
        const size_t i          = uint32_t(rnd()) % kBlockSize;
        const size_t j          = uint32_t(rnd()) % kBlockSize;
        const Engine max_engine = max_cell_engine(block_dp, i, j);

        auto try_decrease_engine = [&](Engine e) noexcept {
            if (t_k >= params.min_temperature_for_eng_decrease && e != Engine::kZero &&
                dist(rnd) <= params.first_decrease_probability) {
                --e;
                if (e != Engine::kZero && dist(rnd) <= params.second_decrease_probability) {
                    --e;
                    if (e != Engine::kZero && dist(rnd) <= params.third_decrease_probability) {
                        --e;
                    }
                }
            }
            return e;
        };
        const Engine placed_engine = try_decrease_engine(max_engine);

        if (block_dp[i][j] == placed_engine) {
            continue;
        }

        const auto curr_power = p[size_t(block_dp[i][j])];
        const auto new_power  = p[size_t(placed_engine)];
        const auto delta      = new_power - curr_power;
        static_assert(std::is_signed_v<decltype(delta)>);
        if (delta >= 0 || dist(rnd) <= std::exp(delta / t_k)) {
            block_dp[i][j] = placed_engine;
            total_power = uint64_t(int64_t(total_power) + delta);
            auto greedy_improved_ans = try_improve_greedily(block_dp);
            if (greedy_improved_ans > max_power) {
                max_power           = greedy_improved_ans;
                max_power_iteration = iteration;
            }
        }
    }

    assert(compare_dp_power(block_dp, total_power));

    return {
        .max_power           = max_power,
        .max_power_iteration = max_power_iteration,
    };
}

static void optimize_block_with_params(MapBlock& block_dp, const OptimizeParams& params,
                                       uint64_t best_reached_power,
                                       size_t best_reached_power_sa_iteration) noexcept {
    if (best_reached_power_sa_iteration > kMaxIterationsInBlock) {
        __builtin_unreachable();
    }

    std::mt19937 rnd{params.seed};
    std::uniform_real_distribution<double> dist(0, 1);

    uint64_t total_power = 0;
    for (const auto& row : block_dp) {
        for (const Engine e : row) {
            total_power += uint32_t(p[size_t(e)]);
        }
    }
    double t_k                 = params.initial_temp_value;
    for (size_t iteration = 1; iteration <= kMaxIterationsInBlock;
         t_k              = std::max(t_k * params.gamma, params.min_gamma), iteration++) {
        const size_t i          = uint32_t(rnd()) % kBlockSize;
        const size_t j          = uint32_t(rnd()) % kBlockSize;
        const Engine max_engine = max_cell_engine(block_dp, i, j);

        auto try_decrease_engine = [&](Engine e) noexcept {
            if (t_k >= params.min_temperature_for_eng_decrease && e != Engine::kZero &&
                dist(rnd) <= params.first_decrease_probability) {
                --e;
                if (e != Engine::kZero && dist(rnd) <= params.second_decrease_probability) {
                    --e;
                    if (e != Engine::kZero && dist(rnd) <= params.third_decrease_probability) {
                        --e;
                    }
                }
            }
            return e;
        };
        const Engine placed_engine = try_decrease_engine(max_engine);

        if (block_dp[i][j] == placed_engine) {
            continue;
        }

        const auto curr_power = p[size_t(block_dp[i][j])];
        const auto new_power  = p[size_t(placed_engine)];
        const auto delta      = new_power - curr_power;
        static_assert(std::is_signed_v<decltype(delta)>);
        if (delta >= 0 || dist(rnd) <= std::exp(delta / t_k)) {
            block_dp[i][j] = placed_engine;
            total_power = uint64_t(int64_t(total_power) + delta);
            if (iteration == best_reached_power_sa_iteration) {
                assert(total_power == best_reached_power);
                improve_greedily(block_dp, best_reached_power);
                break;
            }
        }
    }

    assert(total_power == best_reached_power);
    assert(compare_dp_power(block_dp, best_reached_power));
}

struct BlockOptimizationResult {
    uint64_t block_reached_power;
    size_t best_reached_power_iteration;
    OptimizeParams block_params;
};

template <std::size_t N>
static BlockOptimizationResult optimize_block(const size_t j_shift, const size_t i_shift, Map& dp,
                                              std::mt19937& seed_generator,
                                              const std::array<double, N> gamma_values) noexcept {
    assert(i_shift + kBlockSize <= dp.size());
    assert(j_shift + kBlockSize <= dp[0].size());

    array<array<Engine, kBlockSize>, kBlockSize> block{};
    for (size_t i = 0; i < kBlockSize; i++) {
        std::copy_n(dp[i_shift + i].cbegin() + j_shift, kBlockSize, block[i].begin());
    }

    uint64_t best_reached_power             = 0;
    size_t best_reached_power_iteration = 0;
    OptimizeParams best_params{};
    for (const double gamma : gamma_values) {
        for (const double min_gamma : kMinGammaValues) {
            for (const double first_decrease_probability : kFirstDecreaseProbability) {
                for (const double second_decrease_probability : kSecondDecreaseProbability) {
                    for (const double third_decrease_probability : kThirdDecreaseProbability) {
                        for (const double min_temperature_for_eng_decrease :
                             kMinTemperatureForEngDecreaseValues) {
                            for (const double initial_temp_value : kInitialTempValues) {
                                for (size_t i = kIterationsPerPararmsPack; i > 0; i--) {
                                    const OptimizeParams params = {
                                        .seed                        = seed_generator(),
                                        .gamma                       = gamma,
                                        .min_gamma                   = min_gamma,
                                        .first_decrease_probability  = first_decrease_probability,
                                        .second_decrease_probability = second_decrease_probability,
                                        .third_decrease_probability  = third_decrease_probability,
                                        .min_temperature_for_eng_decrease =
                                            min_temperature_for_eng_decrease,
                                        .initial_temp_value = initial_temp_value,
                                    };
                                    const auto [max_reached_power, max_reached_power_iteration] =
                                        apply_simulated_annealing(block, params);
                                    if (max_reached_power > best_reached_power) {
                                        best_reached_power = max_reached_power;
                                        best_reached_power_iteration =
                                            max_reached_power_iteration;
                                        best_params = params;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    optimize_block_with_params(block, best_params, best_reached_power, best_reached_power_iteration);

    for (size_t i = 0; i < kBlockSize; i++) {
        std::copy_n(block[i].cbegin(), kBlockSize, dp[i_shift + i].begin() + j_shift);
    }

    return {
        .block_reached_power              = best_reached_power,
        .best_reached_power_iteration = best_reached_power_iteration,
        .block_params                     = best_params,
    };
}

ATTRIBUTE_COLD static uint_fast32_t get_seed_fallback() noexcept {
    fprintf(stderr, "WARNING: calling %s\n", std::source_location::current().function_name());
    srand(uint32_t(time(nullptr)));
    return uint32_t(rand()) ^ uint_fast32_t(time(nullptr));
}

static uint_fast32_t get_seed() noexcept {
    try {
        std::ifstream fin("/dev/urandom", std::ios::in | std::ios::binary);
        if (!fin.is_open() || !fin) [[unlikely]] {
            return get_seed_fallback();
        }
        uint_fast32_t useed{};
        char* ptr = reinterpret_cast<char*>(&useed);
        fin.read(ptr, sizeof(useed));
        return fin ? useed : get_seed_fallback();
    } catch (const std::exception& ex) {
        fprintf(stderr, "ERROR: exception: %s\n", ex.what());
        return get_seed_fallback();
    } catch (...) {
        fprintf(stderr, "ERROR: unknown exception\n");
        return get_seed_fallback();
    }
}

static_assert(n >= kBlockSize, "");
static_assert(m >= kBlockSize, "");
inline constexpr auto kTotalRecordsFromThread =
    (n - kBlockSize + 1 + kBlockSkipStep - 1) * (m - kBlockSize + 1 + kBlockSkipStep - 1);
inline constexpr auto kTotalRecordsFromThreads = kTotalRecordsFromThread * kThreadsCount;
using RecordsContainer = std::array<BlockOptimizationResult, kTotalRecordsFromThread>;
struct ThreadResult {
    int64_t max_power;
    RecordsContainer records;
};

using SummaryRecordsContainer = std::array<ThreadResult, kThreadsCount>;

static ThreadResult solve(std::size_t slice_index) noexcept {
    static_assert(uint64_t(Engine::kZero) == 0);
    Map dp{};
    std::mt19937 seed_generator{get_seed()};

    std::array<double, kGammaValuesPerThread> gamma_values{};
    {
        constexpr auto kLocalGammaValues(kGammaValues);
        assert(slice_index * kGammaValuesPerThread + kGammaValuesPerThread <=
               kLocalGammaValues.size());
        std::copy_n(kLocalGammaValues.begin() + slice_index * kGammaValuesPerThread,
                    kGammaValuesPerThread, gamma_values.begin());
    }

    ThreadResult ans{};
    size_t write_index = 0;

    if constexpr (kBlockSize == n && kBlockSize == m) {
        ans.records[write_index++] = optimize_block(0, 0, dp, seed_generator, gamma_values);
    } else {
        for (size_t i_shift = 0; i_shift + kBlockSize <= n; i_shift += kBlockSkipStep) {
            for (size_t j_shift = 0; j_shift + kBlockSize <= m; j_shift += kBlockSkipStep) {
                assert(write_index < ans.records.size());
                ans.records[write_index++] =
                    optimize_block(j_shift, i_shift, dp, seed_generator, gamma_values);
            }
        }
    }
    assert(write_index == ans.records.size());

    for (const auto& row : dp) {
        for (const Engine e : row) {
            ans.max_power += p[size_t(e)];
        }
    }

    return ans;
}

static SummaryRecordsContainer run_solvers() noexcept {
    using TFuture = std::future<ThreadResult>;
    std::array<TFuture, kThreadsCount> thread_handles{};
    SummaryRecordsContainer results{};

    try {
        for (std::size_t i = 0; TFuture & handle_future : thread_handles) {
            handle_future = std::async(std::launch::async, &solve, i++);
        }

        for (size_t results_write_index = 0; TFuture & handle_future : thread_handles) {
            if (!handle_future.valid()) {
                fprintf(stderr, "WARNING: invalid future from thread %zu\n", results_write_index);
                continue;
            }

            assert(results_write_index < results.size());
            results[results_write_index++] = handle_future.get();
        }

        std::ranges::sort(results,
                          [](const ThreadResult& tr1, const ThreadResult& tr2) constexpr noexcept {
                              return tr1.max_power > tr2.max_power;
                          });
    } catch (const std::exception& ex) {
        fprintf(stderr, "ERROR: exception: %s\n", ex.what());
    } catch (...) {
        fprintf(stderr, "ERROR: unknown exception\n");
    }

    return results;
}

#include <sys/resource.h>

static int setup() noexcept {
#define GET_OUTPUT_FILENAME(tn) "result_" #tn
#define OUTPUT_FILENAME(tn)     GET_OUTPUT_FILENAME(tn)
    if (freopen(OUTPUT_FILENAME(TEST_NUMBER), "a+", stdout) == nullptr) {
        perror("freopen");
        return EXIT_FAILURE;
    }
#undef OUTPUT_FILENAME
#undef GET_OUTPUT_FILENAME
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

class SolversStarter final {
public:
    SolversStarter() noexcept : start_(time(nullptr)) {
        assert(start_ != time_t(-1));
    }
    void run() const noexcept {
        print_formatted_config();
        auto results = run_solvers();
        print_formatted_results(results);
    }
    ~SolversStarter() {
        print_formatted_config_end();
    }

private:
    void print_formatted_config() const noexcept {
        auto tostr = [](const auto& arr) {
            std::string s;
            s.reserve(1000);
            s += '[';
            for (double f : arr) {
                char buffer[std::numeric_limits<double>::max_exponent10 + 24]{};
                [[maybe_unused]] int ret =
                    snprintf(buffer, sizeof(buffer), std::abs(f) > 1e-6 ? "%.15lf," : "%e,", f);
                assert(ret > 0);
                s += buffer;
            }
            s += ']';
            s.shrink_to_fit();
            return s;
        };
        auto gv    = tostr(kGammaValues);
        auto mgv   = tostr(kMinGammaValues);
        auto fdbv  = tostr(kFirstDecreaseProbability);
        auto sdbv  = tostr(kSecondDecreaseProbability);
        auto tdbv  = tostr(kThirdDecreaseProbability);
        auto edtbv = tostr(kMinTemperatureForEngDecreaseValues);
        // clang-format off
        printf(
            "{\n"
            "    start_time = %s"
            "    filename = \"%s\"\n"
            "    kBlockSize = %" PRIu32 "\n"
            "    kBlockSkipStep = %" PRIu32 "\n"
            "    kMaxIterationsInBlock = %" PRIu32 "\n"
            "    kIterationsPerPararmsPack = %" PRIu32 "\n"
            "    kThreadsCount = %zu\n"
            "    kTotalRecordsFromThread = %zu\n"
            "    kTotalRecordsFromThreads = %zu\n"
            "    kGammaValues = %s\n"
            "    kMinGammaValues = %s\n"
            "    kFirstDecreaseProbability = %s\n"
            "    kSecondDecreaseProbability = %s\n"
            "    kThirdDecreaseProbability = %s\n"
            "    kMinTemperatureForEngDecreaseValues = %s\n",
            asctime(localtime(&start_)), std::source_location::current().file_name(), kBlockSize,
            kBlockSkipStep, kMaxIterationsInBlock, kIterationsPerPararmsPack, kThreadsCount,
            kTotalRecordsFromThread, kTotalRecordsFromThreads, gv.c_str(), mgv.c_str(),
            fdbv.c_str(), sdbv.c_str(), tdbv.c_str(), edtbv.c_str());
        // clang-format on
        fflush(stdout);
    }
    void print_formatted_results(const SummaryRecordsContainer& results) const noexcept {
        // clang-format off
        printf("    duration_in_seconds = %" PRIu64 "\n"
               "    max_power = %" PRId64 "\n",
               uint64_t(time(nullptr)) - uint64_t(start_),
               std::ranges::max(results, [](const ThreadResult& tr1,
                                            const ThreadResult& tr2) constexpr noexcept {
                   return tr1.max_power < tr2.max_power;
               }).max_power);
        for (uint32_t thread_num = 0; const ThreadResult& tr : results) {
            printf("    thread_number = %" PRIu32 ":\n"
                   "        max_power = %" PRId64 "\n",
                   thread_num++, tr.max_power);
            for (uint32_t record_num = 0; const BlockOptimizationResult& rec : tr.records) {
                printf("        block_num = %" PRIu32 ":\n"
                       "            best_reached_power_iteration = %" PRIu64 "\n"
                       "            block_reached_power = %" PRId64 "\n"
                       "            seed = %" PRIuFAST32 "\n"
                       "            gamma = %.15e\n"
                       "            min_gamma = %.15e\n"
                       "            first_decrease_probability = %.7f\n"
                       "            second_decrease_probability = %.7f\n"
                       "            third_decrease_probability = %.7f\n"
                       "            min_temperature_for_eng_decrease = %.15e\n",
                       record_num++, rec.best_reached_power_iteration, rec.block_reached_power,
                       rec.block_params.seed, rec.block_params.gamma, rec.block_params.min_gamma,
                       rec.block_params.first_decrease_probability,
                       rec.block_params.second_decrease_probability,
                       rec.block_params.third_decrease_probability,
                       rec.block_params.min_temperature_for_eng_decrease);
            }
        }
        // clang-format on
    }
    void print_formatted_config_end() const noexcept {
        printf("}\n");
    }

    time_t start_{};
};

int main() {
    if (int ret = setup(); ret) {
        return ret;
    }
    SolversStarter{}.run();
}
