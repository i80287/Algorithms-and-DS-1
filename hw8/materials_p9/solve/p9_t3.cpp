#include <unistd.h>

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
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numbers>
#include <numeric>
#include <random>
#include <set>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std;
using u8   = uint8_t;
using u16  = uint16_t;
using u32  = uint32_t;
using u64  = uint64_t;
using u128 = __uint128_t;
using i8   = int8_t;
using i16  = int16_t;
using i32  = int32_t;
using i64  = int64_t;
using i128 = __int128_t;

enum class Engine : u32 {
    kFirst,
    kSecond,
    kThird,
    kFourth,
    kFifth,
};

int main() {
    vector<tuple<u32, u32, Engine>> ansvec;
    ansvec.reserve(2010);
    u64 w = 0;
    for (u32 x = 1; x <= 1000; x++) {
        ansvec.emplace_back(1u, x, Engine::kFirst);
        w += 1;
    }
    for (u32 x = 1; x <= 1000; x += 2) {
        ansvec.emplace_back(1u, x, Engine::kSecond);
        w -= 1;
        w += 2;
        if (x + 2 <= 1000) {
            w -= 1;
            w += 4;
            ansvec.emplace_back(1u, x + 1, Engine::kThird);
        }
    }
    std::ofstream fout("output3.txt");
    fout << w << ' ' << ansvec.size() << '\n';
    for (auto [x, y, e] : ansvec) {
        fout << x << ' ' << y << ' ' << u32(e) << '\n';
    }
}
