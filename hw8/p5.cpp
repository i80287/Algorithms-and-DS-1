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
using i64 = int64_t;
using v64 = vector<i64>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    i64 n, m;
    cin >> n >> m;
    vector<v64> dp(n + 1, v64(m + 1));
    v64 w(n);
    v64 c(n);
    for (auto& m_i : w) {
        cin >> m_i;
    }
    for (auto& c_i : c) {
        cin >> c_i;
    }

    for (i64 i = 1; i <= n; i++) {
        for (i64 W = 1; W <= m; W++) {
            dp[i][W] = dp[i - 1][W];
            if (w[i - 1] <= W) {
                dp[i][W] = max(dp[i][W], dp[i - 1][W - w[i - 1]] + c[i - 1]);
            }
        }
    }

    cout << dp[n][m];
}
