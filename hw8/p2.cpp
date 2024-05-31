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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    i64 n,l,r;
    cin >> n >> l >> r;
    l--;
    r--;
    v64 ans(3 * n);
    for (int i = 0; i < (n + 1) / 2; i++) {
        ans[i] = i * 2 + 1;
    }
    for (int i = (n + 1) / 2; i < n; i++) {
        ans[i] = 2 * n - 2 * i;
    }
    copy(ans.begin(), ans.begin() + n, ans.begin() + n);
    copy(ans.begin(), ans.begin() + n, ans.begin() + 2 * n);
    
    i64 himid = n + n / 2;
    i64 mid = (l + r + 1 - n % 2) / 2;
    i64 b = himid - mid;
    i64 e = b + n;
    for (i64 i = b; i < e; i++) {
        cout << ans[i] << ' ';
    }
}
