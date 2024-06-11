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
    ansvec.reserve(u64(3e4));
    u64 w = 0;
    for (u32 x = 1; x <= 100; x++) {
        for (u32 y = 1; y <= 100; y++) {
            ansvec.emplace_back(x, y, Engine::kFirst);
            w++;
        }
    }
    for (u32 x = 1; x <= 100; x++) {
        for (u32 y = 1; y <= (x != 100 ? 100 : 99); y++) {
            ansvec.emplace_back(x, y, Engine::kSecond);
            w -= 1;
            w += 2;
        }
    }
    
    {
        std::ofstream fout("output5.txt");
        assert(fout.is_open());
        assert(fout);
        fout << w << ' ' << ansvec.size() << '\n';
        assert(fout);
        for (auto [x, y, e] : ansvec) {
            fout << x << ' ' << y << ' ' << u32(e) << '\n';
        }
        assert(fout);
    }
}
