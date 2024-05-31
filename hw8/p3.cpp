#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#pragma GCC target("avx,fma,popcnt,tune=native")
#include <array>
#include <bitset>
#include <cassert>
#include <cstring>

using namespace std;
using i64  = int64_t;
using v64 = vector<i64>;

struct segp {
    i64 coord;
    bool is_right;
    uint32_t index;
    constexpr auto operator<=>(const segp&) const noexcept = default;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    size_t n;
    cin >> n;
    assert(n <= 200'000);
    vector<segp> points(2 * n);
    for (uint32_t i = 0; i < n; i++) {
        i64 l{}, r{};
        cin >> l >> r;
        points[i * 2]     = {.coord = l, .is_right = false, .index = i};
        points[i * 2 + 1] = {.coord = r, .is_right = true, .index = i};
    }
    sort(points.begin(), points.end());

    vector<segp> stack;
    static constinit bitset<200'000 + 10> visited{};
    v64 ans;
    for (size_t i = 0; i < 2 * n; ) {
        auto p            = points[i];
        assert(stack.empty() || (!stack.back().is_right && stack.back().coord <= p.coord));
        if (!p.is_right) {
            stack.push_back(p);
            i++;
            continue;
        }
        if (p.index >= visited.size()) { i = 2 * n; break; }
        if (visited[p.index]) {
            i++;
            continue;
        }
        assert(!stack.empty());
        ans.push_back(p.coord);
        for (const auto& sp : stack) {
            if (sp.index >= visited.size()) { i = 2 * n; break; }
            visited[sp.index] = true;
        }
        stack.clear();
        for (i++; i < 2 * n && p.coord == points[i].coord; i++) {
            if (points[i].index >= visited.size()) { i = 2 * n; break; }
            visited[points[i].index] = true;
        }
        assert(i == 2 * n || (p.coord < points[i].coord));
    }
    // assert(stack.empty());
    cout << ans.size() << '\n';
    for (auto c : ans) {
        cout << c << ' ';
    }
}
