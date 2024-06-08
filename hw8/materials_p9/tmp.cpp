#include <algorithm>
#include <any>
#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <numbers>
#include <numeric>
#include <random>

#include <type_traits>
#include <utility>
#include <vector>

using namespace std;
using u8                         = uint8_t;
using u16                        = uint16_t;
using u32                        = uint32_t;
using u64                        = uint64_t;
using u128                       = __uint128_t;
using i8                         = int8_t;
using i16                        = int16_t;
using i32                        = int32_t;
using i64                        = int64_t;
using i128                       = __int128_t;


int main() {
    cout << -std::log10(1 - exp10l(-6.0L / 2e7L));
}
