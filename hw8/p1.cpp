#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

#pragma GCC target("avx,fma,popcnt,tune=native")
#include <array>
#include <bitset>
#include <cassert>
#include <cstring>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    size_t n, w;
    cin >> n >> w;

    struct pr {
        uint64_t c;
        uint64_t w;
    };
    vector<pr> cw(n);
    for (size_t i = 0; i < n; i++) {
        cin >> cw[i].c >> cw[i].w;
    }

    sort(cw.begin(), cw.end(),
         [](pr cw1, pr cw2) constexpr noexcept { return cw1.c * cw2.w > cw2.c * cw1.w; });

    uint64_t ans = 0;
    using flt    = long double;
    flt ans1     = 0;
    for (size_t i = 0; i < n; i++) {
        if (w >= cw[i].w) {
            w -= cw[i].w;
            ans += cw[i].c;
            continue;
        }
        ans1 = flt(cw[i].c * w) / cw[i].w;
        break;
    }
    cout << setprecision(10) << flt(ans) + ans1;
}
