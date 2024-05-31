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

constexpr bool f() {
    constexpr array arr = {1, 2, 5, 8, 19, 20, 22, 22, 22, 39, 40, 56};
    if (lower_bound(arr.begin(), arr.end(), 0) - arr.begin() != 0)
        return false;
    if (lower_bound(arr.begin(), arr.end(), 1) - arr.begin() != 0)
        return false;
    if (lower_bound(arr.begin(), arr.end(), 2) - arr.begin() != 1)
        return false;
    if (lower_bound(arr.begin(), arr.end(), 20) - arr.begin() != 5)
        return false;
    if (lower_bound(arr.begin(), arr.end(), 21) - arr.begin() != 6)
        return false;
    if (lower_bound(arr.begin(), arr.end(), 22) - arr.begin() != 6)
        return false;
    if (lower_bound(arr.begin(), arr.end(), 56) - arr.begin() != 11)
        return false;
    if (lower_bound(arr.begin(), arr.end(), 57) - arr.begin() != 12)
        return false;

    if (upper_bound(arr.begin(), arr.end(), 22) - arr.begin() != 9)
        return false;
    if (upper_bound(arr.begin(), arr.end(), 23) - arr.begin() != 9)
        return false;
    if (upper_bound(arr.begin(), arr.end(), 38) - arr.begin() != 9)
        return false;
    if (upper_bound(arr.begin(), arr.end(), 39) - arr.begin() != 10)
        return false;

    return true;
}

int main() {
    static_assert(f());
}
