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
using i64  = int64_t;
using v64 = vector<i64>;

struct seg {
    double start;
    double end;
    double weight;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    i64 n;
    cin >> n;
    vector<seg> segments(n);
    for (i64 i = 0; i < n; i++) {
        double b, e, w;
        cin >> b >> e >> w;
        segments[i] = {.start = b, .end = e, .weight = w};
    }

    sort(segments.begin(), segments.end(),
         [](const seg& s1, const seg& s2) constexpr noexcept { return s1.end < s2.end; });

    v64 p(n);
    for (i64 i = 0; i < n; i++) {
        auto citer = upper_bound(
            segments.cbegin(), segments.cend(), segments[i].start,
            [](double val, const seg& mid) constexpr noexcept { return val < mid.end; });
        i64 le_cnt = p[i] = citer - segments.cbegin();

        assert(le_cnt <= n);
        for (i64 j = 0; j < le_cnt; j++) {
            assert(segments[j].end <= segments[i].start);
        }
        for (i64 j = le_cnt; j < n; j++) {
            assert(segments[i].start < segments[j].end);
        }
    }
    vector<double> dp(n + 1);
    for (i64 i = 1; i <= n; i++) {
        dp[i] = max(dp[i - 1], dp[p[i - 1]] + segments[i - 1].weight);
    }

    cout << setprecision(10) << dp[n];
}
