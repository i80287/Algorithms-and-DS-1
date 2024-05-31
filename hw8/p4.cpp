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
using v64 = vector<int64_t>;

int main() {
    size_t n;
    cin >> n;
    v64 dp[2] = {v64(n + 1), v64(n + 1)};

    constexpr int64_t mod = 1e9 + 7;
    dp[0][1] = 1;
    dp[1][1] = 1;
    for (size_t len = 2; len <= n; len++) {
        dp[0][len] = (dp[0][len - 1] + dp[1][len - 1]) % mod;
        dp[1][len] = dp[0][len - 1];
    }
    cout << (dp[0][n] + dp[1][n]) % mod;
}
