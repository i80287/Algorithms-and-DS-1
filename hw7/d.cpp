#define NDEBUG 1

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <limits>

using namespace std;

static constexpr auto chartoindex(char c) noexcept {
    return uint8_t(c);
}

template <class T, size_t N>
static void filltbl(array<T, N>& tbl, string_view p) noexcept {
    tbl.fill(p.size());
    for (size_t i = 0; i + 1 < p.size(); i++) {
        const auto c_ind = chartoindex(p[i]);
        assert(c_ind < tbl.size());
        tbl[c_ind] = p.size() - i - 1;
        assert(tbl[c_ind] < p.size());
    }
}

static vector<size_t> solve(string_view p, string_view t) {
    // Just 256
    constexpr auto kSize = std::numeric_limits<uint8_t>::max() + 1;
    array<size_t, kSize> tbl;
    filltbl(tbl, p);

    size_t t_i = p.size() - 1;
    vector<size_t> ans;
    while (t_i < t.size()) {
        const auto start_pos = t_i - p.size() + 1;
        const string_view pt = t.substr(start_pos, p.size());
        if (pt == p) {
            assert(start_pos < t.size() - p.size() + 1);
            ans.push_back(start_pos);
            t_i++;
        } else {
            t_i += tbl[chartoindex(t[t_i])];
        }
    }

    return ans;
}

int main() {
    assert((solve("aaaaaaa", "axaaaaaaaaaaaaaa") == vector<size_t>{2, 3, 4, 5, 6, 7, 8, 9}));
    assert((solve("ndud", "bundudxtafndudndud") == vector<size_t>{2, 10, 14}));
    assert((solve("liqi", "emliqizlhlliqiliqi") == vector<size_t>{2, 10, 14}));
    assert((solve("telb", "wyllbyrsyy").empty()));

    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string p, t;
    p.reserve(size_t(1e3) + 10);
    t.reserve(size_t(1e7) + 10);
    cin >> p >> t;
    const auto ans = solve(p, t);
    cout << ans.size() << '\n';
    for (auto i : ans) {
        cout << i << '\n';
    }
}
