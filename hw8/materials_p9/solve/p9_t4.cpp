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
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numbers>
#include <numeric>
#include <random>
#include <set>
#include <tuple>
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
    State() {
        actions_.reserve(size_t(2e4));
        for (u32 x = 1; x <= dp.size(); x++) {
            for (u32 y = 1; y <= dp[0].size(); y++) {
                dp[x - 1][y - 1] = Engine::kZero;
                w_ += p[Engine::kZero];
                actions_.emplace_back(x, y, Engine::kZero);
            }
        }
    }

    constexpr void operator()(u32 x, u32 y, Engine e) noexcept {
        assert(1 <= x && x <= dp.size());
        assert(1 <= y && y <= dp[0].size());
        assert(w_ >= p[dp[x - 1][y - 1]]);
        w_ -= p[dp[x - 1][y - 1]];
        dp[x - 1][y - 1] = e;
        w_ += p[e];
        actions_.emplace_back(x, y, e);
    }
    constexpr Engine operator()(u32 x, u32 y) const noexcept {
        assert(1 <= x && x <= dp.size());
        assert(1 <= y && y <= dp[0].size());
        return dp[x][y];
    }

    constexpr const vector<Action>& actions() const noexcept {
        return actions_;
    }
    constexpr uint64_t w() const noexcept {
        return w_;
    }
private:
    uint64_t w_ = 0;
    vector<Action> actions_;
    std::array<std::array<Engine, 1000>, 2> dp{};

    static constexpr struct P final : std::array<u32, 5> {
        using Base = std::array<u32, 5>;
        constexpr u32 operator[](Engine e) const noexcept {
            return Base::operator[](std::size_t(e));
        }
    } p = {1, 4, 8, 8, 8};
};

int main() {
    State dp;
    for (u32 x = 1; x <= 1000; x++) {
        dp(1, x, Engine::kFirst);
    }
    for (u32 x = 1; x <= 999; x++) {
        dp(2, x, Engine::kSecond);
    }
    for (u32 x = 1; x <= 1000; x++) {
        dp(1, x, Engine::kZero);
    }
    for (u32 x = 1; x + 1 <= 1000; x += 2) {
        dp(1, x, Engine::kFirst);
        dp(1, x + 1, Engine::kSecond);
    }

    std::ofstream fout("output4.txt");
    fout << dp.w() << ' ' << dp.actions().size() << '\n';
    for (auto [x, y, e] : dp.actions()) {
        fout << x << ' ' << y << ' ' << u32(e) << '\n';
    }
}
