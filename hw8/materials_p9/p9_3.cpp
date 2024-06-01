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
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <numbers>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std;

inline constexpr size_t n             = 2;
inline constexpr size_t m             = 2;
inline constexpr size_t kMaxW         = 85;
inline constexpr array<uint32_t, 5> p = {1, 2, 3, 4, 5};

enum class Engine {
    kNone  = -1,
    kFirst = 0,
    kSecond,
    kThird,
    kFourth,
    kFifth,
};
static_assert(uint32_t(Engine::kFirst) == 0 && uint32_t(Engine::kFifth) == 4);

consteval array<array<Engine, m>, n> init_dp() noexcept {
    array<array<Engine, m>, n> arr{};
    for (auto& r : arr) {
        r.fill(Engine::kNone);
    }
    return arr;
}
static constinit array<array<Engine, m>, n> dp = init_dp();

struct coord_t final {
    size_t i = size_t(-1);
    size_t j = size_t(-1);
};

static vector<coord_t> hist_stack;
static size_t g_i = 0;
static size_t g_j = 0;

static coord_t get_next_coords() {
    const coord_t ans = {.i = g_i, .j = g_j};
    hist_stack.push_back(ans);
    if (g_i == 0) [[unlikely]] {
        g_i = min(g_j + 1, n - 1);
        g_j = g_j + 1 - g_i;
    } else if (g_j == m - 1) [[unlikely]] {
        g_j = m - n + 1 + g_i;
        g_i = n - 1;
    } else {
        g_i--;
        g_j++;
    }
    return ans;
}

static void pop_coords() noexcept {
    assert(!hist_stack.empty());
    const auto [last_i, last_j] = hist_stack.back();
    g_i                         = last_i;
    g_j                         = last_j;
    hist_stack.pop_back();
}

inline constexpr std::array<uint32_t, p.size()> p_count_boundaries = {
    std::min(n * m, kMaxW / p[0]), std::min(n * m, kMaxW / p[1]), std::min(n * m, kMaxW / p[2]),
    std::min(n * m, kMaxW / p[3]),  std::min(n * m, kMaxW / p[4]),
};
static constinit std::array<uint32_t, p.size()> p_count{};
static int64_t current_w = 0;

static bool update_map(const size_t i0, const size_t j0, Engine e) noexcept {
    auto check_cell = [](size_t i, size_t j) constexpr noexcept {
        if (i >= n || j >= m || dp[i][j] == Engine::kNone) {
            return true;
        }

        auto update_mask = [](uint32_t& mask, size_t i1, size_t j1) constexpr noexcept {
            if (i1 >= n || j1 >= m) {
                return false;
            }
            const auto d = dp[i1][j1];
            if (d == Engine::kNone) {
                return true;
            }
            assert(Engine::kFirst <= d && d <= Engine::kFifth);
            size_t idx = uint32_t(d);
            mask |= 1u << idx;
            return false;
        };

        uint32_t mask = 0;
        if (update_mask(mask, i - 1, j) || update_mask(mask, i + 1, j) ||
            update_mask(mask, i, j - 1) || update_mask(mask, i, j + 1)) {
            return true;
        }
        const Engine max_engine = Engine(std::countr_one(mask));
        return dp[i][j] <= max_engine;
    };

    dp[i0][j0] = e;
    const bool adj_cells_check = check_cell(i0, j0 - 1) & check_cell(i0, j0 + 1) &
                                 check_cell(i0 - 1, j0) & check_cell(i0 + 1, j0) &
                                 check_cell(i0, j0);
    assert(Engine::kFirst <= e && e <= Engine::kFifth);
    size_t idx = uint32_t(e);
    assert(idx < p.size());
    current_w += p[idx];
    const bool b1 = current_w <= int64_t(kMaxW);
    const bool b2 = ++p_count[idx] <= p_count_boundaries[idx];
    return adj_cells_check && b1 && b2;
}

static void restore_map(size_t i, size_t j) {
    const Engine e = dp[i][j];
    dp[i][j] = Engine::kNone;
    assert(Engine::kFirst <= e && e <= Engine::kFifth);
    size_t idx = uint32_t(e);
    assert(idx < p.size());
    assert(current_w >= p[idx]);
    current_w -= p[idx];
    assert(p_count[idx] > 0);
    --p_count[idx];
}

static int64_t max_ans = -1;

static int64_t calculate_map() noexcept {
    int64_t ans = 0;
    for (const auto& r : dp) {
        for (Engine e : r) {
            size_t idx = uint32_t(e);
            assert(idx < p.size());
            ans += p[idx];
        }
    }
    return ans;
}

static void print_map(const size_t i, const size_t j, const Engine e) {
    cout << "Successfully assigned " << uint32_t(e) << " at position " << i << ' ' << j << '\n';
    for (const auto& r : dp) {
        for (Engine e1 : r) {
            cout << int(e1) << ' ';
        }
        cout << '\n';
    }
    cout << "\n\n";
}

static bool visit(const size_t i, const size_t j, const Engine e) {
    assert(i < n && j < m);
    assert(dp[i][j] == Engine::kNone);
    assert(Engine::kFirst <= e && e <= Engine::kFifth);

    bool check_res = update_map(i, j, e);
    if (check_res) {
        print_map(i, j, e);

        const auto [ni, nj] = get_next_coords();
        if (ni < n && nj < m) [[likely]] {
            for (uint32_t e1 = uint32_t(Engine::kFirst); e1 <= uint32_t(Engine::kFifth); e1++) {
                if (!visit(ni, nj, Engine(e1))) {
                    // if update_map for {ni,nj,e1} if false, it will be false for all higher engines
                    break;
                }
            }
        } else {
            assert(current_w == calculate_map());
            max_ans = max(max_ans, current_w);
        }
        pop_coords();
    }

    restore_map(i, j);
    return check_res;
}

static void solve() {
    hist_stack.reserve(n * m);
    for (uint32_t e1 = uint32_t(Engine::kFirst); e1 <= uint32_t(Engine::kFifth); e1++) {
        const auto [ni, nj] = get_next_coords();
        assert(ni == 0 && nj == 0);
        visit(ni, nj, Engine(e1));
        pop_coords();
    }
}

int main() {
    solve();
    cout << max_ans;
}
