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
#include <iostream>
#include <limits>
#include <numbers>
#include <numeric>
#include <random>
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

int main() {
    // cout << -std::log10(1 - exp10l(-6.0L / 2e7L));
    printf("%30e\n", 12387978.824673246743e-10);
    printf("%e\n", 12387978.824673246743e-10);
    printf("%le\n", 12387978.824673246743e-10);
    printf("%.30e\n", 12387978.824673246743e-10);
}

static void solve() noexcept {
    static_assert(int(Engine::kFourth) == 4);
    static_assert(int(Engine::kZero) == 0);
    static constinit array<array<array<array<State, 5>, kMaxM>, kMaxN>, kMaxSteps> dp{};
    int64_t max_seen_power = -1;
    for (size_t i = 0; i < kMaxN; i++) {
        for (size_t j = 0; j < kMaxM; j++) {
            for (Engine e = Engine::kZero; e <= Engine::kFourth; ++e) {
                State &state = dp[0][i][j][size_t(e)];
                if (state.can_place_cell(i, j, e)) {
                    state(i, j, e);
                    assert(state.is_valid());
                    max_seen_power = std::max(max_seen_power, state.power());
                } else {
                    state.set_invalid();
                }
            }
        }
    }
    for (size_t step = 1; step < kMaxSteps; step++) {
        for (size_t i = 0; i < kMaxN; i++) {
            for (size_t j = 0; j < kMaxM; j++) {
                for (Engine e = Engine::kZero; e <= Engine::kFourth; ++e) {
                    int64_t max_power  = State::kInvalidPower;
                    size_t max_power_i = 0;
                    size_t max_power_j = 0;
                    Engine max_power_e{};
                    for (size_t i1 = 0; i1 < kMaxN; i1++) {
                        for (size_t j1 = 0; j1 < kMaxM; j1++) {
                            for (Engine e1 = Engine::kZero; e1 <= Engine::kFourth; ++e1) {
                                const State &another_state = dp[step - 1][i1][j1][size_t(e1)];
                                const auto power_if_place  = another_state.power_if_place(i, j, e);
                                if (power_if_place > max_power) {
                                    assert(another_state.is_valid());
                                    max_power = power_if_place;
#ifndef NDEBUG
                                    assert(State(another_state)(i, j, e).power() == power_if_place);
#endif
                                    assert(max_power != State::kInvalidPower);
                                    max_power_i = i1;
                                    max_power_j = j1;
                                    max_power_e = e1;
                                }

                                //                                if (i == 4 && j == 1 && e ==
                                //                                Engine(3)) {
                                //                                    if (another_state.map() == )
                                //                                }
                            }
                        }
                    }

                    if (max_power != State::kInvalidPower) {
                        State &state = dp[step][i][j][size_t(e)] =
                            dp[step - 1][max_power_i][max_power_j][size_t(max_power_e)];
                        state(i, j, e);
                        assert(state.is_valid());
                        assert(state.power() == max_power);
                        max_seen_power = std::max(max_seen_power, state.power());
                    } else {
                        dp[step][i][j][size_t(e)].set_invalid();
                    }
                }
            }
        }
    }

    constexpr std::tuple<int, int, int> actions[61] = {
        {1, 1, 0}, {1, 2, 0}, {1, 3, 0}, {1, 4, 0}, {1, 5, 0}, {2, 1, 0}, {2, 2, 0}, {2, 3, 0},
        {2, 4, 0}, {2, 5, 0}, {3, 1, 0}, {3, 2, 0}, {3, 3, 0}, {3, 4, 0}, {3, 5, 0}, {4, 1, 0},
        {4, 2, 0}, {4, 3, 0}, {4, 4, 0}, {4, 5, 0}, {5, 1, 0}, {5, 2, 0}, {5, 3, 0}, {5, 4, 0},
        {5, 5, 0}, {2, 1, 1}, {3, 1, 2}, {5, 4, 1}, {5, 1, 1}, {5, 5, 2}, {4, 4, 1}, {1, 1, 1},
        {1, 2, 2}, {4, 1, 2}, {2, 3, 1}, {2, 1, 2}, {2, 1, 3}, {2, 2, 3}, {5, 3, 2}, {1, 3, 2},
        {1, 5, 1}, {3, 3, 1}, {2, 3, 3}, {4, 5, 3}, {3, 2, 3}, {5, 2, 3}, {4, 4, 2}, {3, 2, 4},
        {4, 3, 2}, {2, 5, 1}, {3, 5, 2}, {4, 4, 3}, {3, 4, 3}, {4, 3, 4}, {2, 5, 2}, {4, 1, 3},
        {1, 4, 2}, {3, 4, 4}, {2, 5, 3}, {2, 3, 4}, {1, 4, 3},
    };

    static constinit State checker;
    for (size_t step = 0; const auto [ir, jr, er] : actions) {
        const auto i = size_t(uint32_t(ir) - 1);
        const auto j = size_t(uint32_t(jr) - 1);
        const auto e = Engine(er);
        assert(Engine::kZero <= e && e <= Engine::kFourth);
        checker(i, j, e);
        if (step == 44) {
            std::cout << "| step = " << step << " | i = " << i << " | j = " << j
                      << " | Engine = " << size_t(e) << " |\n"
                      << checker;
        }
        if (checker.power() > dp[step][i][j][size_t(e)].power()) {
            std::cout << "| step = " << step << " | i = " << i << " | j = " << j
                      << " | Engine = " << size_t(e) << " |\n"
                      << checker << '\n'
                      << dp[step][i][j][size_t(e)] << '\n';
            return;
        }
        step++;
    }

    std::cout << dp[60][0][3][size_t(Engine(3))] << '\n';
}

namespace recursive {

static constinit int64_t max_ans = 0;

static void solve3_impl() noexcept {
    constexpr size_t kMaxRecursiveSteps = 10;
    static constinit State dp{};
    static constinit size_t depth = 0;

    for (size_t i = 0; i < kMaxN; i++) {
        for (size_t j = 0; j < kMaxM; j++) {
            for (Engine e = Engine::kZero; e <= Engine::kFourth; ++e) {
                if (dp.power_if_place(i, j, e) > dp.power()) {
                    const auto old_e = dp.map()[i][j];
                    dp(i, j, e);
                    ++depth;
                    max_ans = std::max(max_ans, dp.power());
                    if (depth < kMaxRecursiveSteps) {
                        solve3_impl();
                    }
                    --depth;
                    dp(i, j, old_e);
                }
            }
        }
    }
}

static int64_t solve3() {
    solve3_impl();
    return max_ans;
}

}  // namespace recursive

constexpr uint64_t operator-(const timespec &t2, const timespec &t1) noexcept {
    const auto sec_passed        = static_cast<uint64_t>(t2.tv_sec - t1.tv_sec);
    auto nanoseconds_passed      = sec_passed * 1'000'000'000;
    using unsigned_nanoseconds_t = std::make_unsigned_t<decltype(timespec::tv_nsec)>;
    nanoseconds_passed += static_cast<unsigned_nanoseconds_t>(t2.tv_nsec);
    nanoseconds_passed -= static_cast<unsigned_nanoseconds_t>(t1.tv_nsec);
    return nanoseconds_passed;
}
