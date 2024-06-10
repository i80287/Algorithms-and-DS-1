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

inline constexpr size_t n             = 5;
inline constexpr size_t m             = 5;
inline constexpr size_t kMaxW         = 85;
inline constexpr array<uint32_t, 6> p = {0, 1, 2, 3, 4, 5};

enum class Engine : uint32_t {
    kNone = 0,
    kFirst,
    kSecond,
    kThird,
    kFourth,
    kFifth,
};
// static_assert(uint32_t(Engine::kFirst) == 0 && uint32_t(Engine::kFifth) == 4);
static_assert(uint32_t(Engine::kNone) >= p.size() || p[uint32_t(Engine::kNone)] == 0);

consteval array<array<Engine, m>, n> init_dp() noexcept {
    array<array<Engine, m>, n> arr{};
    for (auto& r : arr) {
        r.fill(Engine::kNone);
    }
    return arr;
}
static constinit array<array<Engine, m>, n> dp = init_dp();

inline constexpr std::array<uint64_t, p.size()> p_count_boundaries = {
    n * m,
    std::min(n* m, kMaxW / p[uint32_t(Engine::kFirst)]),
    std::min(n* m, kMaxW / p[uint32_t(Engine::kSecond)]),
    std::min(n* m, kMaxW / p[uint32_t(Engine::kThird)]),
    std::min(n* m, kMaxW / p[uint32_t(Engine::kFourth)]),
    std::min(n* m, kMaxW / p[uint32_t(Engine::kFifth)]),
};
static constinit std::array<uint64_t, p.size()> p_count = {n * m, 0, 0, 0, 0, 0};
static constinit uint64_t current_w                     = n * m * p[0];
static constinit uint64_t max_ans                       = n * m * p[0];
static constinit uint64_t valid_taken                   = 0;

struct coord_t final {
    size_t i                                                 = size_t(-1);
    size_t j                                                 = size_t(-1);
    constexpr bool operator==(const coord_t&) const noexcept = default;
};

static Engine exchange_cell(size_t i, size_t j, Engine new_e) {
    const Engine old_e = dp[i][j];
    dp[i][j]           = new_e;
    assert(Engine::kNone <= new_e && new_e <= Engine::kFifth);
    assert(Engine::kNone <= old_e && old_e <= Engine::kFifth);
    size_t idx_new = uint32_t(new_e);
    size_t idx_old = uint32_t(old_e);
    assert(idx_new < p.size());
    assert(idx_old < p.size());
    assert(current_w >= p[idx_old]);
    current_w -= p[idx_old];
    current_w += p[idx_new];
    assert(p_count[idx_old] > 0);
    --p_count[idx_old];
    ++p_count[idx_new];
    valid_taken -= old_e != Engine::kNone;
    assert(int64_t(valid_taken) >= 0);
    valid_taken += new_e != Engine::kNone;
    return old_e;
}

static std::pair<bool, Engine> update_map(const size_t i0, const size_t j0,
                                          const Engine e) noexcept {
    auto check_cell = [](size_t i, size_t j) constexpr noexcept {
        if (i >= n || j >= m) {
            return true;
        }

        auto update_mask = [](uint32_t& mask, const size_t i1, const size_t j1) constexpr noexcept {
            if (i1 >= n || j1 >= m) [[unlikely]] {
                return;
            }
            const Engine d = dp[i1][j1];
            assert(Engine::kNone <= d && d <= Engine::kFifth);
            const size_t idx = uint32_t(d);
            mask |= 1u << idx;
        };

        uint32_t mask = 0;
        update_mask(mask, i - 1, j);
        update_mask(mask, i + 1, j);
        update_mask(mask, i, j - 1);
        update_mask(mask, i, j + 1);
        mask >>= 1;
        const Engine max_engine = Engine(std::countr_one(mask) + 1);
        return dp[i][j] <= max_engine;
    };

    const Engine old_e         = exchange_cell(i0, j0, e);
    const bool adj_cells_check = check_cell(i0, j0 - 1) & check_cell(i0, j0 + 1) &
                                 check_cell(i0 - 1, j0) & check_cell(i0 + 1, j0) &
                                 check_cell(i0, j0);
    assert(Engine::kFirst <= e && e <= Engine::kFifth);
    const size_t idx = uint32_t(e);
    assert(idx < p.size());
    const bool b1 = current_w <= kMaxW;
    const bool b2 = p_count[idx] <= p_count_boundaries[idx];

    return {adj_cells_check && b1 && b2, old_e};
}

static uint64_t calculate_map() noexcept {
    uint64_t ans = 0;
    for (const auto& r : dp) {
        for (Engine e : r) {
            const size_t idx = uint32_t(e);
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

static vector<coord_t> hist;

static bool acquire_hist(const size_t i, const size_t j) {
    const coord_t c{i, j};
    const size_t sz = hist.size();
    for (int64_t k = int64_t(sz - 1); k >= std::max(int64_t(0), int64_t(sz - 3)); k--) {
        assert(size_t(k) < hist.size());
        if (hist[size_t(k)] == c) {
            return false;
        }
    }
    hist.push_back(c);
    return true;
}
static void release_hist([[maybe_unused]] const size_t i,
                         [[maybe_unused]] const size_t j) noexcept {
    assert((!hist.empty() && hist.back() == coord_t{i, j}));
    hist.pop_back();
}

static bool visit(const size_t i, const size_t j, const Engine e) {
    if (dp[i][j] == e) {
        return true;
    }
    if (!acquire_hist(i, j)) {
        return false;
    }

    assert(i < n && j < m);
    assert(Engine::kFirst <= e && e <= Engine::kFifth);

    const auto [check_res, old_e] = update_map(i, j, e);
    if (check_res) {
        print_map(i, j, e);

        assert(current_w == calculate_map());
        max_ans              = max(max_ans, current_w);
        const coord_t nbrs[] = {
            {.i = i, .j = j - 1}, {.i = i, .j = j + 1}, {.i = i - 1, .j = j}, {.i = i + 1, .j = j}};
        for (const auto [ni, nj] : nbrs) {
            if (ni >= n || nj >= m) [[unlikely]] {
                continue;
            }   
            for (uint32_t e1 = uint32_t(Engine::kFirst); e1 <= uint32_t(Engine::kFifth); e1++) {
                if (!visit(ni, nj, Engine(e1))) {
                    // if update_map for {ni,nj,e1} if false,
                    //  it will be false for all higher engines.
                    break;
                }
            }
        }
    }

    [[maybe_unused]] const Engine e1 = exchange_cell(i, j, old_e);
    assert(e1 == e);
    release_hist(i, j);
    return check_res;
}

static void solve() {
    hist.reserve(4 * n * m);
    for (uint32_t e1 = uint32_t(Engine::kFirst); e1 <= uint32_t(Engine::kFifth); e1++) {
        visit(0, 0, Engine(e1));
    }
}

#include <sys/resource.h>

int main() {
    rlimit rl{};
    if (getrlimit(RLIMIT_STACK, &rl)) {
        perror("getrlimit");
        return 1;
    }
    rl.rlim_cur = 32u << 20;
    if (setrlimit(RLIMIT_STACK, &rl)) {
        perror("setrlimit");
        return 1;
    }

    solve();
    cout << max_ans;
}
