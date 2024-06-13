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
#include <span>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

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

// inline constexpr u32 kTestNumber = 1;
// inline constexpr size_t n        = 5;
// inline constexpr size_t m        = 5;
// inline constexpr size_t kMaxW    = 85;
// inline constexpr array<i32, 5> p = {1, 2, 3, 4, 5};

// inline constexpr u32 kTestNumber = 2;
// inline constexpr size_t n        = 30;
// inline constexpr size_t m        = 30;
// inline constexpr size_t kMaxW    = 3413;
// inline constexpr array<i32, 5> p = {1, 2, 3, 4, 5};

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

inline constexpr u32 kTestNumber = 10;
inline constexpr size_t n        = 256;
inline constexpr size_t m        = 256;
inline constexpr size_t kMaxW    = 3732549;
inline constexpr array<i32, 5> p = {13, 21, 34, 55, 89};

enum class Engine : u32 {
    kZero,
    kFirst,
    kSecond,
    kThird,
    kFourth,
};

struct Action final {
    u32 x{};
    u32 y{};
    Engine e{};
};

class State final {
public:
    constexpr State() {
        actions_.reserve(size_t(5e5));
        std::array<Engine, m> row;
        row.fill(kInvalidEngineSentinel);
        dp.fill(row);
    }

    constexpr void operator()(u32 x, u32 y, Engine e) noexcept {
        assert(x < dp.size());
        assert(y < dp[0].size());
        check_dp_cell_neighbours(x, y, e);
        if (dp[x][y] != kInvalidEngineSentinel) {
            if (dp[x][y] == e) {
                return;
            }
            assert(w_ >= p_[dp[x][y]]);
            w_ -= p_[dp[x][y]];
        }
        dp[x][y] = e;
        w_ += p_[e];
        actions_.emplace_back(x + 1, y + 1, e);
    }
    constexpr Engine operator()(u32 x, u32 y) noexcept {
        assert(x < dp.size());
        assert(y < dp[0].size());
        check_dp_cell(x, y);
        return dp[x][y];
    }
    static constexpr i32 power(Engine e) noexcept {
        return p_[e];
    }
    constexpr i32 power(u32 x, u32 y) noexcept {
        return power(operator()(x, y));
    }

    constexpr vector<Action>&& actions() && noexcept {
        assert(actions_.size() <= size_t(5e5));
        return std::move(actions_);
    }
    constexpr int64_t w() const noexcept {
        return w_;
    }
    static constexpr bool correct_engine(Engine e) noexcept {
        return u32(e) - u32(Engine::kZero) <= u32(Engine::kFourth) - u32(Engine::kZero);
    }

private:
    int64_t w_ = 0;
    vector<Action> actions_;
    std::array<std::array<Engine, m>, n> dp{};

    static constexpr Engine kInvalidEngineSentinel = Engine(u32(-1));

    constexpr void check_dp_cell_neighbours(u32 x, u32 y, Engine e) noexcept {
        if (e == Engine::kZero) {
            return;
        }
        const bool need_only_one = e == Engine::kFirst;
        if (x > 0 && dp[x - 1][y] == kInvalidEngineSentinel) {
            actions_.emplace_back(x - 1 + 1, y + 1, Engine::kZero);
            dp[x - 1][y] = Engine::kZero;
            w_ += p_[Engine::kZero];
            if (need_only_one) {
                return;
            }
        }
        if (y > 0 && dp[x][y - 1] == kInvalidEngineSentinel) {
            actions_.emplace_back(x + 1, y - 1 + 1, Engine::kZero);
            dp[x][y - 1] = Engine::kZero;
            w_ += p_[Engine::kZero];
            if (need_only_one) {
                return;
            }
        }
        if (x + 1 < n && dp[x + 1][y] == kInvalidEngineSentinel) {
            actions_.emplace_back(x + 1 + 1, y + 1, Engine::kZero);
            dp[x + 1][y] = Engine::kZero;
            w_ += p_[Engine::kZero];
            if (need_only_one) {
                return;
            }
        }
        if (y + 1 < m && dp[x][y + 1] == kInvalidEngineSentinel) {
            actions_.emplace_back(x + 1, y + 1 + 1, Engine::kZero);
            dp[x][y + 1] = Engine::kZero;
            w_ += p_[Engine::kZero];
        }
    }

    constexpr void check_dp_cell(u32 x, u32 y) noexcept {
        if (dp[x][y] == kInvalidEngineSentinel) {
            dp[x][y] = Engine::kZero;
            actions_.emplace_back(x + 1, y + 1, Engine::kZero);
            w_ += p_[Engine::kZero];
        }
    }

    static constexpr struct PWrapper final : decltype(p) {
        using Base = decltype(p);

        constexpr i32 operator[](Engine e) const noexcept {
            assert(correct_engine(e));
            return Base::operator[](std::size_t(e));
        }
    } p_{p};
};

inline constexpr i32 kTotalIters = u64(5e5);
using flt                        = double;
using rnd_t                      = mt19937;

template <i64 MaxAns, bool kUseDownEngineHeuristic>
static std::pair<i64, std::vector<Action>> solve(uint_fast32_t rndseed, const flt t_gamma,
                                                 const flt min_gamma,
                                                 const flt first_down_boundary    = 0.5,
                                                 const flt second_down_boundary   = 0.1,
                                                 const flt third_down_boundary    = -1,
                                                 const flt eng_down_temp_boundary = -1) noexcept {
    assert(0 < min_gamma && min_gamma < t_gamma && t_gamma < 1);

    // State dp; static_assert(sizeof(dp) <= 4096, "prefer static State dp");
    static State dp;
    // static_assert(sizeof(dp) > 4096, "prefer State dp");

    rnd_t rnd{rndseed};
    uniform_real_distribution<double> dist(0, 1);
    flt t_k = 1;

    for (u32 iters = 1; iters <= kTotalIters; t_k = max(t_k * t_gamma, min_gamma), iters++) {
        const u32 i = u32(rnd() % n);
        const u32 j = u32(rnd() % m);

        u32 mask = 0;
        if (i > 0) {
            mask |= 1u << u32(dp(i - 1, j));
        }
        if (i + 1 < n) {
            mask |= 1u << u32(dp(i + 1, j));
        }
        if (j > 0) {
            mask |= 1u << u32(dp(i, j - 1));
        }
        if (j + 1 < m) {
            mask |= 1u << u32(dp(i, j + 1));
        }

        Engine max_engine = Engine(std::countr_one(mask));
        assert(dp.correct_engine(max_engine));
        if constexpr (kUseDownEngineHeuristic) {
            if (max_engine > Engine::kZero && t_k > eng_down_temp_boundary &&
                dist(rnd) <= first_down_boundary) {
                max_engine = Engine(u32(max_engine) - 1);
                if (max_engine > Engine::kZero && dist(rnd) <= second_down_boundary) {
                    max_engine = Engine(u32(max_engine) - 1);
                    if (max_engine > Engine::kZero && dist(rnd) <= third_down_boundary) {
                        max_engine = Engine(u32(max_engine) - 1);
                    }
                }
            }
        }

        const auto curr_power = dp.power(i, j);
        const auto new_power  = dp.power(max_engine);
        const auto delta      = new_power - curr_power;
        if (delta >= 0 || dist(rnd) <= std::exp(delta / t_k)) {
            dp(i, j, max_engine);
            if (dp.w() == MaxAns) {
                break;
            }
        }
    }

    return {dp.w(), std::move(std::move(dp).actions())};
}

static void print_solution(i64 w, std::span<const Action> ans) {
    bool use_shift = false;
    for (auto [x, y, e] : ans) {
        use_shift |= x == 0 || y == 0;
    }

    if (use_shift) {
        std::cerr << "Warning: using shift (detected x == 0 or y == 0)\n";
    }

    std::ofstream fout("output" + std::to_string(kTestNumber) + ".txt");
    assert(fout.is_open());
    assert(fout);
    fout << w << ' ' << ans.size() << '\n';
    assert(fout);
    for (auto [x, y, e] : ans) {
        fout << x + use_shift << ' ' << y + use_shift << ' ' << u32(e) << '\n';
    }
    assert(fout);
}

template <i64 MaxAns, bool kUseDownEngineHeuristic>
static void solve_wrapper(uint_fast32_t rndseed, const flt t_gamma, const flt min_gamma,
                          const flt first_down_boundary = 0.5, const flt second_down_boundary = 0.1,
                          const flt third_down_boundary    = -1,
                          const flt eng_down_temp_boundary = -1) noexcept {
    const auto [w, ans] = solve<MaxAns, kUseDownEngineHeuristic>(
        rndseed, t_gamma, min_gamma, first_down_boundary, second_down_boundary, third_down_boundary,
        eng_down_temp_boundary);
    print_solution(w, ans);
}

int main() {
    // clang-format off
    if constexpr (kTestNumber == 1) {
        solve_wrapper<82, true>(1613086534, 0.52000000000000001776, 2.00000000000000004185e-08, 0.700000, 0.050000, 0.050000);
    }
    if constexpr (kTestNumber == 2) {
        solve_wrapper<2674, true>(152756690, 0.33000000000000001554, 2.00000000000000012456e-09, 0.850000, 0.500000, 0.200000);
    }
    if constexpr (kTestNumber == 7) {
        solve_wrapper<149041, true>(2930974558, 0.63000000000000000444, 1.00000000000000004792e-04);
    }
    if constexpr (kTestNumber == 8) {
        solve_wrapper<166563, true>(974579828, 0.81999999999999995115, 5.00000000000000031141e-10, 0.950000, 0.400000, 0.400000);
    }
    if constexpr (kTestNumber == 9) {
        solve_wrapper<29835900, true>(3791382605, 0.58999999999999996891, 5.00000000000000010461e-09, 0.800000, 0.300000, 0.400000);
    }
    if constexpr (kTestNumber == 10) {
        solve_wrapper<2483071, true>(770862419, 0.90000000000000002220, 5.00000000000000010461e-09, 0.750000, 0.001000, 0.050000);
    }
    // clang-format on
}
