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
#include <cmath>
#include <cstring>
#include <limits>
#include <numeric>

using namespace std;
using i64   = int64_t;
using u64  = uint64_t;
using v64  = vector<i64>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    u64 l, n;
    cin >> l >> n;
    vector<u64> c(n + 2);
    c[0] = 0;
    for (u64 i = 1; i <= n; i++) {
        cin >> c[i];
    }
    c[n + 1] = l;

    constexpr auto inf = numeric_limits<u64>::max();
    vector<vector<u64>> dp(n + 2, vector<u64>(n + 2, inf));
    for (u64 i = 0; i <= n + 1; i++) {
        for (u64 j = 0; j <= i + 1 && j <= n + 1; j++) {
            dp[i][j] = 0;
        }
    }
    for (u64 i = n + 1; i64(i) >= 0; i--) {
        for (u64 j = i + 2; j <= n + 1; j++) {
            u64 val = inf;
            for (u64 k = i + 1; k <= j - 1; k++) {
                assert(dp[i][k] != inf);
                assert(dp[k][j] != inf);
                val = min(val, dp[i][k] + dp[k][j]);
            }
            assert(val != inf);
            dp[i][j] = val + c[j] - c[i];
        }
    }

    cout << dp[0][n + 1];
}
