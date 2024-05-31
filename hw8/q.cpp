#include <algorithm>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>

#pragma GCC target("avx,fma,popcnt,tune=native")
#include <array>
#include <bitset>
#include <cassert>
#include <cstring>

using namespace std;

#if defined(__cpp_constinit) && __cpp_constinit >= 201907L
static constinit bitset<4096> m_a[4096] __attribute__((aligned(64))) = {};
static constinit bitset<4096> m_b[4096] __attribute__((aligned(64))) = {};
static constinit bitset<4096> m_c[4096] __attribute__((aligned(64))) = {};
#else
static bitset<4096> m_a[4096] __attribute__((aligned(64))) = {};
static bitset<4096> m_b[4096] __attribute__((aligned(64))) = {};
static bitset<4096> m_c[4096] __attribute__((aligned(64))) = {};
#endif

inline constexpr uint64_t kTable1[256] = {
    0, 0, 0, 0, 0,      0,      0,      0,      0,      0,      0,      0,      0,      0, 0,
    0, 0, 0, 0, 0,      0,      0,      0,      0,      0,      0,      0,      0,      0, 0,
    0, 0, 0, 0, 0,      0,      0,      0,      0,      0,      0,      0,      0,      0, 0,
    0, 0, 0, 0, 0b0001, 0b0010, 0b0011, 0b0100, 0b0101, 0b0110, 0b0111, 0b1000, 0b1001, 0, 0,
    0, 0, 0, 0, 0,      0b1010, 0b1011, 0b1100, 0b1101, 0b1110, 0b1111,
};

inline constexpr uint64_t kTable[256] = {
    0, 0, 0, 0, 0,      0,      0,      0,      0,      0,      0,      0,      0,      0, 0,
    0, 0, 0, 0, 0,      0,      0,      0,      0,      0,      0,      0,      0,      0, 0,
    0, 0, 0, 0, 0,      0,      0,      0,      0,      0,      0,      0,      0,      0, 0,
    0, 0, 0, 0, 0b1000, 0b0100, 0b1100, 0b0010, 0b1010, 0b0110, 0b1110, 0b0001, 0b1001, 0, 0,
    0, 0, 0, 0, 0,      0b0101, 0b1101, 0b0011, 0b1011, 0b0111, 0b1111,
};

constexpr uint8_t bit_reverse(uint8_t b) noexcept {
    return uint8_t(((b * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32);
}

static_assert(kTable['0'] == bit_reverse(0) >> 4);
static_assert(kTable['1'] == bit_reverse(1) >> 4);
static_assert(kTable['2'] == bit_reverse(2) >> 4);
static_assert(kTable['3'] == bit_reverse(3) >> 4);
static_assert(kTable['4'] == bit_reverse(4) >> 4);
static_assert(kTable['5'] == bit_reverse(5) >> 4);
static_assert(kTable['6'] == bit_reverse(6) >> 4);
static_assert(kTable['7'] == bit_reverse(7) >> 4);
static_assert(kTable['8'] == bit_reverse(8) >> 4);
static_assert(kTable['9'] == bit_reverse(9) >> 4);
static_assert(kTable['A'] == bit_reverse(10) >> 4);
static_assert(kTable['B'] == bit_reverse(11) >> 4);
static_assert(kTable['C'] == bit_reverse(12) >> 4);
static_assert(kTable['D'] == bit_reverse(13) >> 4);
static_assert(kTable['E'] == bit_reverse(14) >> 4);
static_assert(kTable['F'] == bit_reverse(15) >> 4);

static void readmrow(bitset<4096>& m, size_t n, string& s) {
    assert(s.capacity() >= 1024);
    cin >> s;
    s[s.size()]     = '\0';
    s[s.size() + 1] = '\0';
    s[s.size() + 2] = '\0';
    s[s.size() + 3] = '\0';

    // size_t i               = 0;
    // uint16_t* m_ptr        = reinterpret_cast<uint16_t*>(&m);
    // const uint8_t* s_ptr   = reinterpret_cast<const uint8_t*>(s.c_str());
    // const size_t roundsize = (s.size() + 3) / 4 * 4;
    // for (; i < roundsize; i += 4) {
    //     *m_ptr++ = uint16_t((kTable[s_ptr[0]] << 0) | (kTable[s_ptr[1]] << 4) |
    //                         (kTable[s_ptr[2]] << 8) | (kTable[s_ptr[3]] << 12));
    //     s_ptr += 4;
    // }

    size_t i               = 0;
    uint8_t* m_ptr         = reinterpret_cast<uint8_t*>(&m);
    const uint8_t* s_ptr   = reinterpret_cast<const uint8_t*>(s.c_str());
    const size_t roundsize = (s.size() + 1) / 2 * 2;
    for (; i < roundsize; i += 2) {
        auto cd = (kTable[uint8_t(s_ptr[0])] << 0) | (kTable[uint8_t(s_ptr[1])] << 4);
        *m_ptr  = uint8_t(cd);
        m_ptr++;
        s_ptr += 2;
    }

    m[n]     = 0;
    m[n + 1] = 0;
    m[n + 2] = 0;
    m[n + 3] = 0;
}

static void readms(size_t n) {
    string s;
    s.reserve(1024);
    for (size_t i = 0; i < n; i++) {
        readmrow(m_a[i], n, s);
    }
    for (size_t i = 0; i < n; i++) {
        readmrow(m_b[i], n, s);
    }
    for (size_t i = 0; i < n; i++) {
        readmrow(m_c[i], n, s);
    }
}

[[maybe_unused]] static bool solve() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    size_t n;
    cin >> n;
    readms(n);

    bool ans = true;
    std::mt19937_64 rnd;
    bitset<4096> r __attribute__((aligned(64))) = {};
    bitset<4096> br{};
    bitset<4096> tmp{};
    static array<uint8_t, 4096> abr{};
    static array<uint8_t, 4096> cr{};
    for (size_t iter = 100; iter > 0; iter--) {
        uint64_t* r_p = reinterpret_cast<uint64_t*>(&r);
        for (size_t i = 0; i < 64; i++) {
            r_p[i] = rnd();
        }
        for (size_t i = 0; i < n; i++) {
            tmp = m_b[i];
            tmp &= r;
            br[i] = bool(tmp.count() % 2);
            tmp   = m_c[i];
            tmp &= r;
            cr[i] = uint8_t(tmp.count() % 2);
        }
        for (size_t i = 0; i < n; i++) {
            tmp = m_a[i];
            tmp &= br;
            abr[i] = uint8_t(tmp.count() % 2);
        }
        ans &= abr == cr;
    }

    return ans;
}

int main() {
    cout << string_view(solve() ? "YES" : "NO");
}
