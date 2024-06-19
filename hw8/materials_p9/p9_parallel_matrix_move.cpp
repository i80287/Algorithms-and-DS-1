#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cinttypes>
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

#define TEST_NUMBER 9
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
#elif TEST_NUMBER == 9
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
    0.24, 0.25, 0.26, 0.27, 0.28, 0.29, 0.3,  0.31, 0.32, 0.33,   0.34,     0.35,       0.36,
    0.37, 0.38, 0.39, 0.4,  0.41, 0.42, 0.43, 0.44, 0.45, 0.46,   0.47,     0.48,       0.49,
    0.5,  0.51, 0.52, 0.53, 0.54, 0.56, 0.57, 0.59, 0.6,  0.61,   0.63,     0.65,       0.67,
    0.68, 0.69, 0.7,  0.71, 0.72, 0.73, 0.74, 0.75, 0.76, 0.77,   0.79,     0.8,        0.81,
    0.82, 0.83, 0.85, 0.88, 0.9,  0.92, 0.95, 0.97, 0.99, 0.9999, 0.999999, 0.99999999,
};
inline constexpr std::array kMinGammaValues = {
    // 1e-3, 1e-4, 1e-5,
    // 5e-6,
    // 1e-6,
    5e-6,
    // 1e-7,
    // 5e-8,
    // 3e-8,
    // 2e-8,
    // 1e-8,
    9e-9,
    // 8e-9,
    // 7e-9,
    // 6e-9,
    // 5e-9,
    // 4e-9,
    3e-9,
    // 2e-9,
    // 1e-9,
    // 5e-10,
    // 1e-10,
    // 5e-11,
};
inline constexpr std::array kFirstDecreaseProbability = {
    // 0.99, 0.95,
    //// 0.9,
    0.87,
    //// , 0.85, 0.83, 0.8, 0.77,
    0.75, 0.73,
    //// 0.7, 0.67, 0.6, 0.5,
    // 0.4,  0.3, -1.0
};
inline constexpr std::array kSecondDecreaseProbability = {
    // 0.55,
    // 0.5,
    0.4,
    // 0.3,
    0.2, 0.1,
    // 0.05,
    // 0.001,
};
inline constexpr std::array kMinTemperatureForEngDecreaseValues = {
    // 1e-5,
    // 1e-6,
    // 1e-7,
    1e-8,
    // 1e-9,
    // 1e-10,
    1e-11,
    -1.0,
};
inline constexpr std::array kInitialTempValues = {
    5.0,
    //, 4.5, 4.0, 3.5,
    3.0,
    // 2.5, 2.0, 1.5,
    1.0,
    0.5,
};

inline constexpr uint32_t kMaxTries  = std::min(n * m, 64zu);
inline constexpr uint32_t kBlockSize = std::min({n, m, 5zu});
static_assert(kBlockSize >= 2);
inline constexpr uint32_t kBlockSkipStep           = 1;
inline constexpr uint32_t kIterationsPerParamsPack = 60;
inline constexpr size_t kThreadsCount              = 16;
static_assert(std::size(kGammaValues) % kThreadsCount == 0);
inline constexpr size_t kGammaValuesPerThread = std::size(kGammaValues) / kThreadsCount;

static_assert(n >= kBlockSize);
static_assert(m >= kBlockSize);
inline constexpr auto kTotalBlocksPerThread =
    ((n - kBlockSize + 1) / kBlockSkipStep) * ((m - kBlockSize + 1) / kBlockSkipStep);
inline constexpr auto kTotalRecordsFromThreads = kTotalBlocksPerThread * kThreadsCount;

struct OptimizeParams {
    uint_fast32_t seed;
    double gamma;
    double min_gamma;
    double first_decrease_probability;
    double second_decrease_probability;
    double min_temperature_for_eng_decrease;
    double initial_temp_value;
};
enum class Engine : uint32_t {
    kZero,
    kFirst,
    kSecond,
    kThird,
    kFourth,
};

struct Action {
    size_t x;
    size_t y;
    Engine e;
};
struct ThreadResult {
    uint64_t reached_power;
    std::vector<Action> actions;
};

inline constexpr uint32_t kMaxIterationsInBlock =
    std::min((uint64_t(5e5) - n * m) / kTotalBlocksPerThread, 70zu);

using SummaryRecordsContainer = std::array<ThreadResult, kThreadsCount>;

using Map      = array<array<Engine, m>, n>;
using MapBlock = array<array<Engine, kBlockSize>, kBlockSize>;

constexpr Engine& operator--(Engine& e) noexcept {
    return e = static_cast<Engine>(static_cast<uint32_t>(e) - 1);
}

inline std::ostream& operator<<(std::ostream& out, const OptimizeParams& params) {
    std::ostringstream str;
    str << "OptimizeParams(seed = " << params.seed << ", gamma = " << params.gamma
        << ", min_gamma = " << params.min_gamma
        << ", first_decrease_probability = " << params.first_decrease_probability
        << ", second_decrease_probability = " << params.second_decrease_probability
        << ", min_temperature_for_eng_decrease = " << params.min_temperature_for_eng_decrease
        << ", initial_temp_value = " << params.initial_temp_value << ')';
    return out << std::move(str).str();
}

inline std::ostream& operator<<(std::ostream& out, const MapBlock& block_dp) {
    std::ostringstream str;
    for (const auto& row : block_dp) {
        for (const Engine e : row) {
            str << uint32_t(e) << ' ';
        }
        str << '\n';
    }
    return out << std::move(str).str();
}

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

template <bool kAutoInitWithZeros = false>
static uint64_t validate_actions(bool has_zero_coords, std::span<const Action> actions) noexcept {
    if (actions.size() > 500'000) {
        std::cerr << "ERROR: actions.size(): " << actions.size() << "\n";
        return uint64_t(-1);
    }

    array<std::bitset<m>, n> bitmap{};
    array<array<Engine, m>, n> dp{};
    const size_t shift = !has_zero_coords;
    for (const auto [x, y, e] : actions) {
        const size_t i = x - shift;
        const size_t j = y - shift;
        if (i >= n || j >= m) {
            std::cerr << "ERROR: invalid coords " << i << ' ' << j << "\n";
            return uint64_t(-1);
        }
        if (e > max_cell_engine(dp, i, j)) {
            std::cerr << "ERROR: invalid engine " << uint32_t(e) << " set on " << i << ' ' << j
                      << "\n";
            // return uint64_t(-1);
        }
        bitmap[i][j] = true;
        dp[i][j]     = e;
    }
    if constexpr (!kAutoInitWithZeros) {
        const bool all_ones = std::ranges::all_of(
            bitmap, [](const std::bitset<m>& row) constexpr noexcept { return row.all(); });
        if (!all_ones) {
            std::cerr << "ERROR: not all dp cells taken\n";
            return uint64_t(-1);
        }
    }

    return std::accumulate(
        dp.cbegin(), dp.cend(), uint64_t(),
        [](uint64_t cur_sum, const array<Engine, m>& row) constexpr noexcept {
            return cur_sum + std::accumulate(row.begin(), row.end(), uint64_t(),
                                             [](uint64_t cur_row_sum, Engine e) constexpr noexcept {
                                                 return cur_row_sum + uint32_t(p[uint32_t(e)]);
                                             });
        });
}

template <size_t N, size_t M>
ATTRIBUTE_CONST constexpr bool compare_dp_power(const array<array<Engine, M>, N>& dp,
                                                uint64_t expected_power) noexcept {
    uint64_t total_power = 0;
    for (const auto& row : dp) {
        for (const Engine e : row) {
            total_power += uint32_t(p[size_t(e)]);
        }
    }
    return total_power == expected_power;
}

inline constexpr size_t kMaxDepth = 1;

template <size_t kDepth = 0>
    requires(kDepth <= kMaxDepth)
constexpr uint64_t try_improve_greedily(const MapBlock& dp) noexcept {
    uint64_t max_ans{};
    {
        auto dpc(dp);
        uint64_t ans = 0;
        for (auto i = 0zu; i < kBlockSize; i++) {
            for (auto j = 0zu; j < kBlockSize; j++) {
                dpc[i][j] = std::max(dpc[i][j], max_cell_engine(dpc, i, j));
                ans += uint32_t(p[uint32_t(dpc[i][j])]);
            }
        }
        max_ans = std::max(max_ans, ans);
        if constexpr (kDepth + 1 <= kMaxDepth) {
            max_ans = std::max(max_ans, try_improve_greedily<kDepth + 1>(dpc));
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
        max_ans = std::max(max_ans, ans);
        if constexpr (kDepth + 1 <= kMaxDepth) {
            max_ans = std::max(max_ans, try_improve_greedily<kDepth + 1>(dpc));
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
        max_ans = std::max(max_ans, ans);
        if constexpr (kDepth + 1 <= kMaxDepth) {
            max_ans = std::max(max_ans, try_improve_greedily<kDepth + 1>(dpc));
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
        max_ans = std::max(max_ans, ans);
        if constexpr (kDepth + 1 <= kMaxDepth) {
            max_ans = std::max(max_ans, try_improve_greedily<kDepth + 1>(dpc));
        }
    }
    assert(max_ans <= kBlockSize * kBlockSize * p[uint32_t(Engine::kFourth)]);
    return max_ans;
}

template <size_t kDepth = 0>
    requires(kDepth <= kMaxDepth)
constexpr bool improve_greedily(MapBlock& dp, uint64_t needed_power,
                                std::vector<Action>& actions) noexcept {
    {
        std::vector<Action> local_actions;
        auto dpc(dp);
        uint64_t ans = 0;
        for (auto i = 0zu; i < kBlockSize; i++) {
            for (auto j = 0zu; j < kBlockSize; j++) {
                const auto e = max_cell_engine(dpc, i, j);
                if (e > dpc[i][j]) {
                    local_actions.emplace_back(i, j, e);
                    dpc[i][j] = e;
                }
                ans += uint32_t(p[uint32_t(dpc[i][j])]);
            }
        }
        if (ans == needed_power) {
            dp = dpc;
            std::copy(local_actions.begin(), local_actions.end(), std::back_inserter(actions));
            return true;
        }
        if constexpr (kDepth + 1 <= kMaxDepth) {
            if (improve_greedily<kDepth + 1>(dpc, needed_power, local_actions)) {
                dp = dpc;
                std::copy(local_actions.begin(), local_actions.end(), std::back_inserter(actions));
                return true;
            }
        }
    }
    {
        std::vector<Action> local_actions;
        auto dpc(dp);
        uint64_t ans = 0;
        for (auto i = 0zu; i < kBlockSize; i++) {
            for (size_t j = kBlockSize - 1; j != size_t(-1); j--) {
                const auto e = max_cell_engine(dpc, i, j);
                if (e > dpc[i][j]) {
                    local_actions.emplace_back(i, j, e);
                    dpc[i][j] = e;
                }
                ans += uint32_t(p[uint32_t(dpc[i][j])]);
            }
        }
        if (ans == needed_power) {
            dp = dpc;
            std::copy(local_actions.begin(), local_actions.end(), std::back_inserter(actions));
            return true;
        }
        if constexpr (kDepth + 1 <= kMaxDepth) {
            if (improve_greedily<kDepth + 1>(dpc, needed_power, local_actions)) {
                dp = dpc;
                std::copy(local_actions.begin(), local_actions.end(), std::back_inserter(actions));
                return true;
            }
        }
    }
    {
        std::vector<Action> local_actions;
        auto dpc(dp);
        uint64_t ans = 0;
        for (size_t i = kBlockSize - 1; i != size_t(-1); i--) {
            for (auto j = 0zu; j < kBlockSize; j++) {
                const auto e = max_cell_engine(dpc, i, j);
                if (e > dpc[i][j]) {
                    local_actions.emplace_back(i, j, e);
                    dpc[i][j] = e;
                }
                ans += uint32_t(p[uint32_t(dpc[i][j])]);
            }
        }
        if (ans == needed_power) {
            dp = dpc;
            std::copy(local_actions.begin(), local_actions.end(), std::back_inserter(actions));
            return true;
        }
        if constexpr (kDepth + 1 <= kMaxDepth) {
            if (improve_greedily<kDepth + 1>(dpc, needed_power, local_actions)) {
                dp = dpc;
                std::copy(local_actions.begin(), local_actions.end(), std::back_inserter(actions));
                return true;
            }
        }
    }
    {
        std::vector<Action> local_actions;
        auto dpc(dp);
        uint64_t ans = 0;
        for (size_t i = kBlockSize - 1; i != size_t(-1); i--) {
            for (size_t j = kBlockSize - 1; j != size_t(-1); j--) {
                const auto e = max_cell_engine(dpc, i, j);
                if (e > dpc[i][j]) {
                    local_actions.emplace_back(i, j, e);
                    dpc[i][j] = e;
                }
                ans += uint32_t(p[uint32_t(dpc[i][j])]);
            }
        }
        if (ans == needed_power) {
            dp = dpc;
            std::copy(local_actions.begin(), local_actions.end(), std::back_inserter(actions));
            return true;
        }
        if constexpr (kDepth + 1 <= kMaxDepth) {
            if (improve_greedily<kDepth + 1>(dpc, needed_power, local_actions)) {
                dp = dpc;
                std::copy(local_actions.begin(), local_actions.end(), std::back_inserter(actions));
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
    uint64_t max_power         = total_power;
    size_t max_power_iteration = 0;
    double t_k                 = params.initial_temp_value;
    for (size_t iteration = 1; iteration <= kMaxIterationsInBlock;
         t_k              = std::max(t_k * params.gamma, params.min_gamma), iteration++) {
        size_t i{};
        size_t j{};
        Engine placed_engine{};
        uint32_t max_tries = kMaxTries;
        do {
            i                       = uint32_t(rnd()) % kBlockSize;
            j                       = uint32_t(rnd()) % kBlockSize;
            const Engine max_engine = max_cell_engine(block_dp, i, j);

            auto try_decrease_engine = [&](Engine e) noexcept {
                if (t_k >= params.min_temperature_for_eng_decrease && e != Engine::kZero &&
                    dist(rnd) <= params.first_decrease_probability) {
                    --e;
                    if (e != Engine::kZero && dist(rnd) <= params.second_decrease_probability) {
                        --e;
                    }
                }
                return e;
            };
            placed_engine = try_decrease_engine(max_engine);
            assert(placed_engine <= max_cell_engine(block_dp, i, j));
        } while (block_dp[i][j] == placed_engine && --max_tries > 0);
        if (max_tries == 0) {
            break;
        }

        const auto curr_power = p[size_t(block_dp[i][j])];
        const auto new_power  = p[size_t(placed_engine)];
        const auto delta      = new_power - curr_power;
        static_assert(std::is_signed_v<decltype(delta)>);
        if (delta >= 0 || dist(rnd) <= std::exp(delta / t_k)) {
            assert(placed_engine <= max_cell_engine(block_dp, i, j));
            block_dp[i][j]           = placed_engine;
            total_power              = uint64_t(int64_t(total_power) + delta);
            auto greedy_improved_ans = try_improve_greedily(block_dp);
            if (greedy_improved_ans > max_power) {
                max_power           = greedy_improved_ans;
                max_power_iteration = iteration;
            }
        }
    }

    assert(max_power_iteration <= kMaxIterationsInBlock);
    assert(compare_dp_power(block_dp, total_power));
    return {
        .max_power           = max_power,
        .max_power_iteration = max_power_iteration,
    };
}

static void optimize_block_with_params(MapBlock& original_block_dp, const OptimizeParams& params,
                                       uint64_t best_reached_power,
                                       size_t best_reached_power_iteration,
                                       std::vector<Action>& actions) noexcept {
    assert(best_reached_power_iteration <= kMaxIterationsInBlock);
    if (best_reached_power_iteration > kMaxIterationsInBlock) {
        __builtin_unreachable();
    }

    MapBlock block_dp(original_block_dp);
    std::mt19937 rnd{params.seed};
    std::uniform_real_distribution<double> dist(0, 1);

    uint64_t total_power = 0;
    for (const auto& row : block_dp) {
        for (const Engine e : row) {
            total_power += uint32_t(p[size_t(e)]);
        }
    }
    assert(best_reached_power >= total_power);
    assert(best_reached_power_iteration > 0);
    double t_k = params.initial_temp_value;
    for (size_t iteration = 1; iteration <= kMaxIterationsInBlock;
         t_k              = std::max(t_k * params.gamma, params.min_gamma), iteration++) {
        size_t i{};
        size_t j{};
        Engine placed_engine{};
        uint32_t max_tries = kMaxTries;
        do {
            i                       = uint32_t(rnd()) % kBlockSize;
            j                       = uint32_t(rnd()) % kBlockSize;
            const Engine max_engine = max_cell_engine(block_dp, i, j);

            auto try_decrease_engine = [&](Engine e) noexcept {
                if (t_k >= params.min_temperature_for_eng_decrease && e != Engine::kZero &&
                    dist(rnd) <= params.first_decrease_probability) {
                    --e;
                    if (e != Engine::kZero && dist(rnd) <= params.second_decrease_probability) {
                        --e;
                    }
                }
                return e;
            };
            placed_engine = try_decrease_engine(max_engine);
            assert(placed_engine <= max_cell_engine(block_dp, i, j));
        } while (block_dp[i][j] == placed_engine && --max_tries > 0);
        if (max_tries == 0) {
            return;
        }

        const auto curr_power = p[size_t(block_dp[i][j])];
        const auto new_power  = p[size_t(placed_engine)];
        const auto delta      = new_power - curr_power;
        static_assert(std::is_signed_v<decltype(delta)>);
        if (delta >= 0 || dist(rnd) <= std::exp(delta / t_k)) {
            assert(placed_engine <= max_cell_engine(block_dp, i, j));
            block_dp[i][j] = placed_engine;
            actions.emplace_back(i, j, placed_engine);
            total_power = uint64_t(int64_t(total_power) + delta);
            assert(total_power <= best_reached_power);
            if (iteration == best_reached_power_iteration) {
                assert(try_improve_greedily(block_dp) == best_reached_power);
                [[maybe_unused]] bool res = improve_greedily(block_dp, best_reached_power, actions);
                assert(res);
                assert(compare_dp_power(block_dp, best_reached_power));
                original_block_dp = block_dp;
                return;
            }
        }
    }
    assert(false);
}

template <std::size_t N>
static void optimize_block(
    const size_t j_shift, const size_t i_shift, Map& dp, std::mt19937& seed_generator,
    const std::array<std::tuple<double, double, double, double, double, double>, N>&
        all_params_packs,
    std::vector<Action>& actions) noexcept {
    assert(actions.capacity() >= uint64_t(6e5));
    assert(i_shift + kBlockSize <= dp.size());
    assert(j_shift + kBlockSize <= dp[0].size());

    array<array<Engine, kBlockSize>, kBlockSize> block{};
    for (size_t i = 0; i < kBlockSize; i++) {
        std::copy_n(dp[i_shift + i].cbegin() + j_shift, kBlockSize, block[i].begin());
    }

    uint64_t best_reached_power         = 0;
    size_t best_reached_power_iteration = 0;
    OptimizeParams best_params{};
    for (const auto [gamma, min_gamma, first_decrease_probability, second_decrease_probability,
                     min_temperature_for_eng_decrease, initial_temp_value] : all_params_packs) {
        for (size_t i = kIterationsPerParamsPack; i > 0; i--) {
            const OptimizeParams params = {
                .seed                             = seed_generator(),
                .gamma                            = gamma,
                .min_gamma                        = min_gamma,
                .first_decrease_probability       = first_decrease_probability,
                .second_decrease_probability      = second_decrease_probability,
                .min_temperature_for_eng_decrease = min_temperature_for_eng_decrease,
                .initial_temp_value               = initial_temp_value,
            };
            const auto [max_reached_power, max_reached_power_iteration] =
                apply_simulated_annealing(block, params);
            if (max_reached_power > best_reached_power) {
                best_reached_power           = max_reached_power;
                best_reached_power_iteration = max_reached_power_iteration;
                best_params                  = params;
            }
        }
    }

    if (best_reached_power_iteration > 0) {
        optimize_block_with_params(block, best_params, best_reached_power,
                                   best_reached_power_iteration, actions);
    }

    for (size_t i = 0; i < kBlockSize; i++) {
        std::copy_n(block[i].cbegin(), kBlockSize, dp[i_shift + i].begin() + j_shift);
    }
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

constexpr void shift_actions(std::span<Action> actions, size_t shift_x, size_t shift_y) noexcept {
    for (Action& act : actions) {
        act.x += shift_x;
        act.y += shift_y;
    }
}

static ThreadResult solve(std::size_t slice_index) {
    constexpr std::size_t kTotalParamsPacks =
        kGammaValuesPerThread * kMinGammaValues.size() * kFirstDecreaseProbability.size() *
        kSecondDecreaseProbability.size() * kMinTemperatureForEngDecreaseValues.size() *
        kInitialTempValues.size();
    std::array<std::tuple<double, double, double, double, double, double>, kTotalParamsPacks>
        all_params_packs{};
    {
        std::array<double, kGammaValuesPerThread> gamma_values{};
        {
            constexpr auto kLocalGammaValues(kGammaValues);
            assert(slice_index * kGammaValuesPerThread + kGammaValuesPerThread <=
                   kLocalGammaValues.size());
            std::copy_n(kLocalGammaValues.begin() + slice_index * kGammaValuesPerThread,
                        kGammaValuesPerThread, gamma_values.begin());
        }

        std::ranges::cartesian_product_view params_product = std::views::cartesian_product(
            gamma_values, kMinGammaValues, kFirstDecreaseProbability, kSecondDecreaseProbability,
            kMinTemperatureForEngDecreaseValues, kInitialTempValues);

        assert(params_product.size() == kTotalParamsPacks);
        std::copy(params_product.cbegin(), params_product.cend(), all_params_packs.begin());
    }

    static_assert(uint64_t(Engine::kZero) == 0);
    Map dp{};
    std::mt19937 seed_generator{get_seed()};

    ThreadResult ans{};
    ans.actions.reserve(uint64_t(6e5));
    for (size_t i_shift = 0; i_shift + kBlockSize <= n; i_shift += kBlockSkipStep) {
        for (size_t j_shift = 0; j_shift + kBlockSize <= m; j_shift += kBlockSkipStep) {
            size_t shift_start_pos = ans.actions.size();
            optimize_block(j_shift, i_shift, dp, seed_generator, all_params_packs, ans.actions);
            shift_actions(std::span<Action>(ans.actions).subspan(shift_start_pos), i_shift,
                          j_shift);

            std::cerr << "INFO: thread " << gettid() << " ended in block "
                      << (i_shift / kBlockSkipStep) * kBlockSize + j_shift / kBlockSkipStep << " / "
                      << kTotalBlocksPerThread - 1 << '\n';
        }
    }

    for (const auto& row : dp) {
        for (const Engine e : row) {
            ans.reached_power += uint32_t(p[size_t(e)]);
        }
    }

    return ans;
}

static SummaryRecordsContainer run_solvers() {
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
                              return tr1.reached_power > tr2.reached_power;
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
    void run() const {
        print_actions(run_impl());
    }
    ~SolversStarter() {
        print_formatted_config_end();
    }

private:
    std::vector<Action> run_impl() const {
        print_formatted_config();
        auto results = run_solvers();
        auto iter    = print_formatted_results(results);
        return make_actions(iter->actions);
    }

    static std::vector<Action> make_actions(const std::vector<Action>& old_actions) {
        std::vector<Action> actions;
        actions.reserve(uint64_t(5e5));
        actions.resize(n * m + old_actions.size());
        size_t k = 0;
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < m; j++) {
                actions[k++] = {.x = i, .y = j, .e = Engine::kZero};
            }
        }
        std::copy(old_actions.begin(), old_actions.end(), actions.begin() + (n * m));
        return actions;
    }

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
        auto edtbv = tostr(kMinTemperatureForEngDecreaseValues);
        auto itv   = tostr(kInitialTempValues);
        // clang-format off
        printf(
            "{\n"
            "    start_time = %s"
            "    filename = \"%s\"\n"
            "    kBlockSize = %" PRIu32 "\n"
            "    kBlockSkipStep = %" PRIu32 "\n"
            "    kMaxIterationsInBlock = %" PRIu32 "\n"
            "    kIterationsPerParamsPack = %" PRIu32 "\n"
            "    kThreadsCount = %zu\n"
            "    kTotalBlocksPerThread = %zu\n"
            "    kTotalRecordsFromThreads = %zu\n"
            "    kGammaValues = %s\n"
            "    kMinGammaValues = %s\n"
            "    kFirstDecreaseProbability = %s\n"
            "    kSecondDecreaseProbability = %s\n"
            "    kMinTemperatureForEngDecreaseValues = %s\n"
            "    kInitialTempValues = %s\n",
            asctime(localtime(&start_)),
            std::source_location::current().file_name(),
            kBlockSize,
            kBlockSkipStep,
            kMaxIterationsInBlock,
            kIterationsPerParamsPack,
            kThreadsCount,
            kTotalBlocksPerThread,
            kTotalRecordsFromThreads,
            gv.c_str(),
            mgv.c_str(),
            fdbv.c_str(),
            sdbv.c_str(),
            edtbv.c_str(),
            itv.c_str());
        // clang-format on
        fflush(stdout);
    }
    [[nodiscard]] SummaryRecordsContainer::const_iterator print_formatted_results(
        const SummaryRecordsContainer& results) const noexcept {
        auto iter = std::ranges::max_element(
            results, [](const ThreadResult& tr1, const ThreadResult& tr2) constexpr noexcept {
                return tr1.reached_power < tr2.reached_power;
            });
        printf("    duration_in_seconds = %" PRIu64
               "\n"
               "    max_reached_power = %" PRIu64 "\n",
               uint64_t(time(nullptr)) - uint64_t(start_), iter->reached_power);
        for (uint32_t thread_num = 0; const ThreadResult& tr : results) {
            printf("    thread_number = %" PRIu32
                   ":\n"
                   "        max_power = %" PRIu64
                   "\n"
                   "        actions_size = %zu\n",
                   thread_num++, tr.reached_power, tr.actions.size());
        }
        return iter;
    }
    static void print_actions(const std::vector<Action>& actions) {
        const bool has_zero_coords = std::ranges::any_of(
            actions, [](Action act) constexpr noexcept { return act.x == 0 || act.y == 0; });

        uint64_t w = validate_actions(has_zero_coords, actions);
        if (w == uint64_t(-1)) {
            std::cerr << "ERROR: incorrect actions\n";
        }

        // State dp;
        // for (const auto [x, y, e] : ans) {
        //     dp(x - 1, y - 1, e);
        //     cout << dp <<
        //     "---------------------------------------------------------------------\n";
        // }

#define GET_OUTPUT_FILENAME(tn) "./solve/dev_output" #tn ".txt"
#define OUTPUT_FILENAME(tn)     GET_OUTPUT_FILENAME(tn)
        std::ofstream fout(OUTPUT_FILENAME(TEST_NUMBER));
#undef OUTPUT_FILENAME
#undef GET_OUTPUT_FILENAME

        assert(fout.is_open());
        assert(fout);
        fout << w << ' ' << actions.size() << '\n';
        assert(fout);
        for (auto [x, y, e] : actions) {
            fout << x + has_zero_coords << ' ' << y + has_zero_coords << ' ' << uint32_t(e) << '\n';
        }
        assert(fout);
    }
    static void print_formatted_config_end() noexcept {
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
