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

#define TEST_NUMBER 1
inline constexpr u32 kTestNumber = TEST_NUMBER;

#if TEST_NUMBER == 1
inline constexpr size_t n        = 5;
inline constexpr size_t m        = 5;
inline constexpr size_t kMaxW    = 85;
inline constexpr array<u32, 5> p = {1, 2, 3, 4, 5};
#elif TEST_NUMBER == 2
inline constexpr size_t n        = 30;
inline constexpr size_t m        = 30;
inline constexpr size_t kMaxW    = 3413;
inline constexpr array<u32, 5> p = {1, 2, 3, 4, 5};
#elif TEST_NUMBER == 7
inline constexpr size_t n        = 4;
inline constexpr size_t m        = 4;
inline constexpr u32 kMaxW       = 149041;
inline constexpr array<u32, 5> p = {1, 16, 256, 4096, 65536};
#elif TEST_NUMBER == 8
inline constexpr size_t n        = 8;
inline constexpr size_t m        = 9;
inline constexpr size_t kMaxW    = 231012;
inline constexpr array<u32, 5> p = {1, 10, 100, 1000, 10000};
#elif TEST_NUMBER == 9;
inline constexpr size_t n        = 167;
inline constexpr size_t m        = 284;
inline constexpr size_t kMaxW    = 54813600;
inline constexpr array<u32, 5> p = {100, 250, 550, 1000, 2000};
#elif TEST_NUMBER == 10
inline constexpr size_t n        = 256;
inline constexpr size_t m        = 256;
inline constexpr size_t kMaxW    = 3732549;
inline constexpr array<u32, 5> p = {13, 21, 34, 55, 89};
#else
#error "Unknown TEST_NUMBER"
#endif

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
        std::array<Engine, m> row{};
        row.fill(Engine::kZero);
        dp.fill(row);
        const auto total = std::size(dp) * std::size(dp[0]);
        actions_.resize(total);
        w_       = total * p_[Engine::kZero];
        size_t i = 0;
        for (u32 x = 1; x <= dp.size(); x++) {
            for (u32 y = 1; y <= dp[0].size(); y++) {
                actions_[i++] = {.x = x, .y = y, .e = Engine::kZero};
            }
        }
    }

    constexpr void operator()(size_t x, size_t y, Engine e) noexcept {
        assert(x < dp.size());
        assert(y < dp[0].size());
        if (dp[x][y] == e) {
            return;
        }
        assert(w_ >= p_[dp[x][y]]);
        w_ -= p_[dp[x][y]];
        dp[x][y] = e;
        w_ += p_[e];
        actions_.emplace_back(x + 1, y + 1, e);
    }
    [[nodiscard]] constexpr Engine operator()(size_t x, size_t y) const noexcept {
        assert(x < dp.size());
        assert(y < dp[0].size());
        return dp[x][y];
    }
    [[nodiscard]] static constexpr i32 power(Engine e) noexcept {
        return i32(p_[e]);
    }
    [[nodiscard]] constexpr i32 power(size_t x, size_t y) const noexcept {
        return power(operator()(x, y));
    }
    [[nodiscard]] constexpr vector<Action>&& actions() && noexcept {
        assert(actions_.size() <= size_t(5e5));
        return std::move(actions_);
    }
    [[nodiscard]] constexpr uint64_t w() const noexcept {
        return w_;
    }
    [[nodiscard]] static constexpr bool correct_engine(Engine e) noexcept {
        return u32(e) - u32(Engine::kZero) <= u32(Engine::kFourth) - u32(Engine::kZero);
    }
    [[nodiscard]] constexpr Engine max_cell_engine(size_t i, size_t j) const noexcept {
        assert(i < dp.size());
        assert(j < dp[0].size());
        u32 mask = 0;
        if (i > 0) {
            mask |= 1u << u32(operator()(i - 1, j));
        }
        if (i + 1 < n) {
            mask |= 1u << u32(operator()(i + 1, j));
        }
        if (j > 0) {
            mask |= 1u << u32(operator()(i, j - 1));
        }
        if (j + 1 < m) {
            mask |= 1u << u32(operator()(i, j + 1));
        }
        const auto e = Engine(std::countr_one(mask));
        assert(correct_engine(e));
        return e;
    }
    [[nodiscard]] constexpr const auto& map() const noexcept {
        return dp;
    }
    friend std::ostream& operator<<(std::ostream& out, const State& dpst) {
        std::string s;
        constexpr auto nbytes = (sizeof("0 ") - 1) * m * n + n;
        s.reserve(nbytes);
        for (const auto& r : dpst.dp) {
            for (const Engine e : r) {
                s += std::to_string(u32(e));
                s += ' ';
            }
            s += '\n';
        }
        return out << s;
    }

    template <bool kBackwardI = false, bool kBackwardJ = false>
    constexpr void update_map_greedily() noexcept {
        if constexpr (kBackwardI) {
            if constexpr (kBackwardJ) {
                for (auto i = dp.size() - 1; int64_t(i) >= 0; i--) {
                    for (auto j = dp[0].size() - 1; int64_t(j) >= 0; j--) {
                        update_greedily(i, j);
                    }
                }
            } else {
                for (auto i = dp.size() - 1; int64_t(i) >= 0; i--) {
                    for (auto j = 0zu; j < dp[0].size(); j++) {
                        update_greedily(i, j);
                    }
                }
            }
        } else {
            if constexpr (kBackwardJ) {
                for (auto i = 0zu; i < dp.size(); i++) {
                    for (auto j = dp[0].size() - 1; int64_t(j) >= 0; j--) {
                        update_greedily(i, j);
                    }
                }
            } else {
                for (auto i = 0zu; i < dp.size(); i++) {
                    for (auto j = 0zu; j < dp[0].size(); j++) {
                        update_greedily(i, j);
                    }
                }
            }
        }
    }

private:
    uint64_t w_ = 0;
    vector<Action> actions_;
    std::array<std::array<Engine, m>, n> dp{};

    constexpr void update_greedily(size_t i, size_t j) noexcept {
        const auto e = max_cell_engine(i, j);
        if (e > dp[i][j]) {
            operator()(i, j, e);
        }
    }

    static constexpr struct PWrapper final : decltype(p) {
        using Base = decltype(p);

        [[nodiscard]] constexpr u32 operator[](Engine e) const noexcept {
            assert(correct_engine(e));
            return Base::operator[](std::size_t(e));
        }
    } p_{p};
};

inline constexpr i32 kTotalIters = u64(1000000);
using flt                        = double;
using rnd_t                      = mt19937;

inline constexpr u64 kUnknownMaxAns = std::numeric_limits<u64>::max();

template <size_t kDepth = 0>
constexpr uint64_t try_greedy(const array<array<Engine, m>, n>& dp) noexcept {
    uint64_t max_ans{};
    {
        auto dpc(dp);
        uint64_t ans = 0;
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < m; j++) {
                u32 mask = 0;
                if (i > 0) {
                    mask |= 1u << u32(dpc[i - 1][j]);
                }
                if (i + 1 < n) {
                    mask |= 1u << u32(dpc[i + 1][j]);
                }
                if (j > 0) {
                    mask |= 1u << u32(dpc[i][j - 1]);
                }
                if (j + 1 < m) {
                    mask |= 1u << u32(dpc[i][j + 1]);
                }
                const auto max_engine = Engine(std::countr_one(mask));
                dpc[i][j]             = std::max(dpc[i][j], max_engine);
                ans += u32(p[u32(dpc[i][j])]);
            }
        }
        max_ans = std::max(max_ans, ans);
        if constexpr (kDepth + 1 < 2) {
            max_ans = std::max(max_ans, try_greedy<kDepth + 1>(dpc));
        }
    }
    {
        auto dpc(dp);
        uint64_t ans = 0;
        for (u32 i = 0; i < n; i++) {
            for (u32 j = m - 1; j != u32(-1); j--) {
                u32 mask = 0;
                if (i > 0) {
                    mask |= 1u << u32(dpc[i - 1][j]);
                }
                if (i + 1 < n) {
                    mask |= 1u << u32(dpc[i + 1][j]);
                }
                if (j > 0) {
                    mask |= 1u << u32(dpc[i][j - 1]);
                }
                if (j + 1 < m) {
                    mask |= 1u << u32(dpc[i][j + 1]);
                }
                const auto max_engine = Engine(std::countr_one(mask));
                dpc[i][j]             = std::max(dpc[i][j], max_engine);
                ans += u32(p[u32(dpc[i][j])]);
            }
        }
        max_ans = std::max(max_ans, ans);
        if constexpr (kDepth + 1 < 2) {
            max_ans = std::max(max_ans, try_greedy<kDepth + 1>(dpc));
        }
    }
    {
        auto dpc(dp);
        uint64_t ans = 0;
        for (u32 i = n - 1; i != u32(-1); i--) {
            for (u32 j = 0; j < m; j++) {
                u32 mask = 0;
                if (i > 0) {
                    mask |= 1u << u32(dpc[i - 1][j]);
                }
                if (i + 1 < n) {
                    mask |= 1u << u32(dpc[i + 1][j]);
                }
                if (j > 0) {
                    mask |= 1u << u32(dpc[i][j - 1]);
                }
                if (j + 1 < m) {
                    mask |= 1u << u32(dpc[i][j + 1]);
                }
                const auto max_engine = Engine(std::countr_one(mask));
                dpc[i][j]             = std::max(dpc[i][j], max_engine);
                ans += u32(p[u32(dpc[i][j])]);
            }
        }
        max_ans = std::max(max_ans, ans);
        if constexpr (kDepth + 1 < 2) {
            max_ans = std::max(max_ans, try_greedy<kDepth + 1>(dpc));
        }
    }
    {
        auto dpc(dp);
        uint64_t ans = 0;
        for (u32 i = n - 1; i != u32(-1); i--) {
            for (u32 j = m - 1; j != u32(-1); j--) {
                u32 mask = 0;
                if (i > 0) {
                    mask |= 1u << u32(dpc[i - 1][j]);
                }
                if (i + 1 < n) {
                    mask |= 1u << u32(dpc[i + 1][j]);
                }
                if (j > 0) {
                    mask |= 1u << u32(dpc[i][j - 1]);
                }
                if (j + 1 < m) {
                    mask |= 1u << u32(dpc[i][j + 1]);
                }
                const auto max_engine = Engine(std::countr_one(mask));
                dpc[i][j]             = std::max(dpc[i][j], max_engine);
                ans += u32(p[u32(dpc[i][j])]);
            }
        }
        max_ans = std::max(max_ans, ans);
        if constexpr (kDepth + 1 < 2) {
            max_ans = std::max(max_ans, try_greedy<kDepth + 1>(dpc));
        }
    }

    return max_ans;
}

template <size_t kDepth = 0>
constexpr bool improve_greedily(State& dp, uint64_t needed_ans) noexcept {
    {
        State copy(dp);
        copy.update_map_greedily<false, false>();
        if (copy.w() == needed_ans) {
            dp = copy;
            return true;
        }
        if constexpr (kDepth + 1 < 2) {
            if (improve_greedily<kDepth + 1>(copy, needed_ans)) {
                dp = copy;
                return true;
            }
        }
    }
    {
        State copy(dp);
        copy.update_map_greedily<false, true>();
        if (copy.w() == needed_ans) {
            dp = copy;
            return true;
        }
        if constexpr (kDepth + 1 < 2) {
            if (improve_greedily<kDepth + 1>(copy, needed_ans)) {
                dp = copy;
                return true;
            }
        }
    }
    {
        State copy(dp);
        copy.update_map_greedily<true, false>();
        if (copy.w() == needed_ans) {
            dp = copy;
            return true;
        }
        if constexpr (kDepth + 1 < 2) {
            if (improve_greedily<kDepth + 1>(copy, needed_ans)) {
                dp = copy;
                return true;
            }
        }
    }
    {
        State copy(dp);
        copy.update_map_greedily<true, true>();
        if (copy.w() == needed_ans) {
            dp = copy;
            return true;
        }
        if constexpr (kDepth + 1 < 2) {
            if (improve_greedily<kDepth + 1>(copy, needed_ans)) {
                dp = copy;
                return true;
            }
        }
    }
    return false;
}

template <u64 MaxAns, bool kUseDownEngineHeuristic, bool kDPEqualPrecheck = false>
static std::pair<u64, std::vector<Action>> solve(uint_fast32_t rndseed, const double t_gamma,
                                                 const double min_gamma,
                                                 const double first_down_boundary,
                                                 const double second_down_boundary,
                                                 const double third_down_boundary,
                                                 const double eng_down_temp_boundary,
                                                 const double initial_temp_value) noexcept {
    assert(0 < t_gamma && t_gamma < 1);
    assert(0 < min_gamma && min_gamma < 1);
    assert(0 < first_down_boundary && first_down_boundary < 1);
    assert(0 <= second_down_boundary && second_down_boundary < 1);
    assert(-1 <= third_down_boundary && third_down_boundary < 1);
    assert(-1 <= eng_down_temp_boundary && eng_down_temp_boundary < 1);
    assert(min_gamma <= initial_temp_value);

    // State dp; static_assert(sizeof(dp) <= 4096, "prefer static State dp");
    static State dp;
    // static_assert(sizeof(dp) > 4096, "prefer State dp");

    rnd_t rnd{rndseed};
    uniform_real_distribution<double> dist(0, 1);
    flt t_k = initial_temp_value;
    [[maybe_unused]] u64 max_ans{};

    u32 iters = 1;
    for (; iters <= kTotalIters; t_k = max(t_k * t_gamma, min_gamma), iters++) {
        const u32 j = u32(rnd() % m);
        const u32 i = u32(rnd() % n);

        Engine max_engine = dp.max_cell_engine(i, j);
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

        if constexpr (kDPEqualPrecheck) {
            if (dp(i, j) == max_engine) {
                continue;
            }
        }

        const auto curr_power = dp.power(i, j);
        const auto new_power  = dp.power(max_engine);
        const auto delta      = new_power - curr_power;
        if (delta >= 0 || dist(rnd) <= std::exp(delta / t_k)) {
            dp(i, j, max_engine);
            if constexpr (MaxAns != kUnknownMaxAns) {
                if (try_greedy(dp.map()) == MaxAns) {
                    improve_greedily(dp, MaxAns);
                    break;
                }
            } else {
                max_ans = std::max(max_ans, try_greedy(dp.map()));
            }
        }
    }

    if constexpr (MaxAns != kUnknownMaxAns) {
        return {dp.w(), std::move(std::move(dp).actions())};
    } else {
        return {max_ans, {std::move(std::move(dp).actions())}};
    }
}

static bool are_actions_correct(bool has_zero_coords, u64 w, std::span<const Action> ans) {
    if (ans.size() > 500'000) {
        return false;
    }

    array<std::bitset<m>, n> bitmap{};
    array<array<Engine, m>, n> dp{};
    const u32 shift = !has_zero_coords;
    for (const auto [x, y, e] : ans) {
        const u32 i = x - shift;
        const u32 j = y - shift;
        if (i >= n || j >= m) {
            return false;
        }
        bitmap[i][j] = true;
        dp[i][j]     = e;
    }
    const bool all_ones = std::ranges::all_of(
        bitmap, [](const std::bitset<m>& row) constexpr noexcept { return row.all(); });
    if (!all_ones) {
        return false;
    }

    return w == std::accumulate(dp.cbegin(), dp.cend(), u32(),
                                [](u32 cur_sum, const array<Engine, m>& row) constexpr noexcept {
                                    return cur_sum +
                                           std::accumulate(
                                               row.cbegin(), row.cend(), u32(),
                                               [](u32 cur_row_sum, Engine e) constexpr noexcept {
                                                   return cur_row_sum + p[u32(e)];
                                               });
                                });
}

static void print_solution(u64 w, std::span<const Action> ans) {
    const bool has_zero_coords = std::ranges::any_of(
        ans, [](Action act) constexpr noexcept { return act.x == 0 || act.y == 0; });

    assert(are_actions_correct(has_zero_coords, w, ans));

    // State dp;
    // for (const auto [x, y, e] : ans) {
    //     dp(x - 1, y - 1, e);
    //     cout << dp << "---------------------------------------------------------------------\n";
    // }

#define GET_OUTPUT_FILENAME(tn) "output" #tn ".txt"
#define OUTPUT_FILENAME(tn)     GET_OUTPUT_FILENAME(tn)
    std::ofstream fout(OUTPUT_FILENAME(TEST_NUMBER));
#undef OUTPUT_FILENAME
#undef GET_OUTPUT_FILENAME

    assert(fout.is_open());
    assert(fout);
    fout << w << ' ' << ans.size() << '\n';
    assert(fout);
    for (auto [x, y, e] : ans) {
        fout << x + has_zero_coords << ' ' << y + has_zero_coords << ' ' << u32(e) << '\n';
    }
    assert(fout);
}

template <u64 MaxAns, bool kUseDownEngineHeuristic = true, bool kDPEqualPrecheck = true>
static void solve_wrapper(uint_fast32_t random_seed, const double t_gamma, const double min_gamma,
                          const double first_down_boundary    = 0.5,
                          const double second_down_boundary   = 0.1,
                          const double third_down_boundary    = -1,
                          const double eng_down_temp_boundary = -1,
                          const double initial_temp_value     = 1) noexcept {
    auto [w, actions] = solve<MaxAns, kUseDownEngineHeuristic, kDPEqualPrecheck>(
        random_seed, t_gamma, min_gamma, first_down_boundary, second_down_boundary,
        third_down_boundary, eng_down_temp_boundary, initial_temp_value);
    if (MaxAns != kUnknownMaxAns) {
        assert(w == MaxAns);
        print_solution(w, actions);
    } else {
        cout << "Max ans = " << w << '\n';
    }
}

int main() {
    // clang-format off
    if constexpr (kTestNumber == 1) {
        solve_wrapper<84>(164102429, 0.9999999999990000221217201, 5.00000000000000040902e-06,
                          0.870000, 0.300000, 0.100000, 9.99999999999999939497e-12, 5.000000);
    }
    else if constexpr (kTestNumber == 2) {
        solve_wrapper<2674, true>(152756690, 0.33000000000000001554, 2.00000000000000012456e-09, 0.850000, 0.500000, 0.200000);
        // solve_wrapper<2665, true>(180412732, 0.56999999999999995115, 8.99999999999999952656e-09, 0.850000, 0.300000, 0.200000);
        // solve_wrapper<2661, true>(2379024859, 0.60999999999999998668, 8.99999999999999952656e-09, 0.900000, 0.500000, 0.300000);
    }
    else if constexpr (kTestNumber == 7) {
        solve_wrapper<149041, true>(2930974558, 0.63000000000000000444, 1.00000000000000004792e-04);
    }
    else if constexpr (kTestNumber == 8) {
        solve_wrapper<166563, true>(974579828, 0.81999999999999995115, 5.00000000000000031141e-10, 0.950000, 0.400000, 0.400000);
    }
    else if constexpr (kTestNumber == 9) {
        solve_wrapper<29835900, true>(3791382605, 0.58999999999999996891, 5.00000000000000010461e-09, 0.800000, 0.300000, 0.400000);
    }
    else if constexpr (kTestNumber == 10) {
        // solve_wrapper<2483071, true>(770862419, 0.90000000000000002220, 5.00000000000000010461e-09, 0.750000, 0.001000, 0.050000);
        // solve_wrapper<2482694, true>(3672067068, 0.20000000000000001110, 2.00000000000000004185e-08, 0.750000, 0.001000, 0.200000);
        solve_wrapper<2482486, true>(1815575930, 0.99999999900000002828, 6.99999999999999981559e-09, 0.750000, 0.001000, 0.100000);

        // solve_wrapper<2481559, true>(3180546647, 0.46999999999999997335, 5.00000000000000010461e-09, 0.750000, 0.050000, 0.100000);
        // solve_wrapper<2481391, true>(3563872237, 0.76000000000000000888, 5.00000000000000010461e-09, 0.750000, 0.001000, 0.200000);
        // solve_wrapper<2481287, true>(443457612, 0.99999000000000004551, 5.00000000000000010461e-09, 0.750000, 0.001000, 0.100000);
        // solve_wrapper<2481270, true>(869379917, 0.70999999999999996447, 2.00000000000000004185e-08, 0.750000, 0.001000, 0.100000);
        // solve_wrapper<2481180, true>(3330694998, 0.54000000000000003553, 5.00000000000000010461e-09, 0.750000, 0.001000, 0.050000);
    }
    // clang-format on
}
