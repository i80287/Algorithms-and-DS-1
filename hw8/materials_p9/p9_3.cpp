#include <bits/stdc++.h>

using namespace std;

enum Engine {
    kNone,
    kFirst,
    kSecond,
    kThird,
    kFourth,
    kFifth,
};

inline constexpr size_t n     = 6;
inline constexpr size_t m     = 5;
inline constexpr uint32_t max_w = 85;
inline constexpr array<uint32_t, 5> p = {1, 2, 3, 4, 5};
static constinit array<array<Engine, m>, n> dp{};
static_assert(static_cast<int>(Engine::kNone) == 0);

static vector<pair<size_t, size_t>> hist_stack;

static pair<size_t, size_t> get_next_coord() {
    static size_t i = 0;
    static size_t j = 0;
    pair ans = {i, j};
    hist_stack.push_back(ans);
    if (i == 0) [[unlikely]] {
        i = min(j + 1, n - 1);
        j = j + 1 - i; 
    } else if (j == m - 1) [[unlikely]] {
        j = m - n + 1 + i;
        i = n - 1;
    } else {
        i--;
        j++;
    }
    return ans;
}

struct coord_t { size_t i = size_t(-1); size_t j = size_t(-1); };
static deque<pair<coord_t, Engine>> q;
static constinit array<array<uint8_t, m>, n> visited{};

static int64_t check_map(const size_t i0, const size_t j0) {
    for (size_t i = size_t(max(int64_t(0), int64_t(i0 - 1))); i < min(n, i0 + 2); i++) {
        for (size_t j = size_t(max(int64_t(0), int64_t(j0 - 1))); j < min(m, j0 + 2); j++) {
            auto update_mask = [&](uint32_t& mask, size_t i, size_t j) constexpr noexcept {
                if (i < n && j < m) {
                    mask |= 1u << dp[i][j];
                }
            };
            uint32_t mask = 0;
            update_mask(mask, i - 1, j);
            update_mask(mask, i + 1, j);
            update_mask(mask, i, j - 1);
            update_mask(mask, i, j + 1);
            mask >>= 1;
            const Engine max_engine = Engine(countr_one(mask) + 1);
            if (dp[i][j] > max_engine) {
                return false;
            }
        }
    }

    return true;
}

static bool visit(size_t i, size_t j, Engine e) {
    dp[i][j] = e;
    visited[i][j] = true;

    int64_t sm = check_map();
    if (sm < 0) {
        dp[i][j] = kNone;
        visited[i][j] = false;
        return false;
    }

    const pair<uint32_t, uint32_t> nbrs[] = {{i - 1, j}, {i + 1, j}, {i, j - 1}, {i, j + 1}};
    for (uint32_t e1 = kFirst; e1 <= kFifth; e1++) {
        for (const auto [ni, nj] : nbrs) {
            if (ni < n && nj < m && !visited[i][j]) {
                visit(ni, nj, Engine(e1));
            }
        }
    }
}

int main() {
    array<array<int32_t, m>, n> mp{};
    memset(&mp, -1, sizeof(mp));
    for (int32_t k = 0; k < n * m; k++) {
        auto [i, j] = get_next_coord();
        assert(i < n && j < m);
        mp[i][j] = k;
    }
    auto [a, b] = get_next_coord();
    cout << a << ' ' << b << "\n\n";
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < m; j++) {
            cout << setw(3) << mp[i][j] << ' ';
        }
        cout << '\n';
    }
}
