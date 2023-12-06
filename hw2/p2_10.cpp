#include <cassert>
#include <cstdint>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <string_view>
#include <iostream>
#include <cmath>
#include <vector>
#include <complex>
#include <cstring>

using std::string_view;

using i32 = int32_t;
using i64 = int64_t;
using u32 = uint32_t;
using u64 = uint64_t;
using f64 = double;
using f128 = long double;
using real = f64;
using complex = std::complex<real>;
using std::vector;
using std::string;
using std::pair;
using std::cin, std::cout;

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

inline static uint64_t NearestTwoPowGreaterEqual(uint64_t n) noexcept {
    return 1ull << (64 - __builtin_clzll(n) - ((n & (n - 1)) == 0));
}

inline static constexpr std::pair<size_t, size_t> MinMax(size_t a, size_t b) {
    return b < a ? std::pair(b, a) : std::pair(a, b);
}

void ForwardFFT(vector<complex>& p, const vector<complex>& points) {
    const size_t k = p.size();
    assert(k != 0 && (k & (k - 1)) == 0);
    for (size_t i = 1, j = 0; i < k; i++) {
        size_t bit = k >> 1;
        for (; j >= bit; bit >>= 1) {
            j -= bit;
        }

        j += bit;
        if (i < j) {
            std::swap(p[i], p[j]);
        }
    }

    for (size_t step = 1, point_step = k / 2; step < k; step *= 2, point_step /= 2) {
        for (size_t block_start = 0; block_start < k; ) {
            size_t point_start = 0;
            size_t block_end = block_start + step;

            for (size_t pos_in_block = block_start; pos_in_block < block_end; pos_in_block++) {
                auto p0_i = p[pos_in_block];
                auto w_j_p1_i = points[point_start] * p[pos_in_block + step];
                p[pos_in_block] = p0_i + w_j_p1_i;
                p[pos_in_block + step] = p0_i - w_j_p1_i;
                point_start += point_step;
            }

            block_start = block_end + step;
        }
    }
}

void BackwardFFT(vector<complex>& p, const vector<complex>& points) {
    const size_t k = p.size();
    assert(k != 0 && (k & (k - 1)) == 0);
    for (size_t i = 1, j = 0; i < k; i++) {
        size_t bit = k >> 1;
        for (; j >= bit; bit >>= 1) {
            j -= bit;
        }

        j += bit;
        if (i < j) {
            std::swap(p[i], p[j]);
        }
    }

    for (size_t step = 1, point_step = k / 2; step < k; step *= 2, point_step /= 2) {
        for (size_t block_start = 0; block_start < k; ) {
            size_t point_start = 0;
            size_t block_end = block_start + step;

            for (size_t pos_in_block = block_start; pos_in_block < block_end; pos_in_block++) {
                auto p0_i = p[pos_in_block];
                auto w_j_p1_i = points[point_start] * p[pos_in_block + step];
                p[pos_in_block] = p0_i + w_j_p1_i;
                p[pos_in_block + step] = p0_i - w_j_p1_i;
                point_start += point_step;
            }

            block_start = block_end + step;
        }
    }

    real one_kth = 1.0 / real(k);
    for (complex& p_i : p) {
        p_i *= one_kth;
    }
}

struct Number {
    string digits;

    inline Number() = default;

    inline explicit Number(const char* s) : digits(s) {
    }

    inline explicit Number(const string& s) : digits(s) {
    }

    inline explicit Number(string&& s) : digits(std::move(s)) {
    }

    Number operator*(const Number& other) {
        if (unlikely(digits.empty() || digits.front() == '0'
            || other.digits.empty() || other.digits.front() == '0')) {
            return Number("0");
        }

        const size_t n = NearestTwoPowGreaterEqual(digits.size() + other.digits.size());
        vector<complex> p1(n);
#if THREE_NLOGN_FFT
        auto iter = p1.begin() + (digits.size() - 1);
        for (char c : digits) {
            *iter = real(c - '0');
            --iter;
        }
        vector<complex> p2(n);
        iter = p2.begin() + (other.digits.size() - 1);
        for (char c : other.digits) {
            *iter = real(c - '0');
            --iter;
        }

        vector<complex> points(n);
        {
            complex w = std::exp(complex(0, 2 * real(acosl(-1)) / real(n)));
            points[0] = 1;
            points[1] = w;
            complex current = w;
            for (size_t i = 2; i < n; i++) {
                current *= w;
                points[i] = current;
            }
        }

        // if (n >= 4 && n <= 32) { for (size_t i = 0; i < n; i++) { cout << '(' << p1[i].real() << ',' << p2[i].real() << ") "; } cout << "\n\n"; }
    
        ForwardFFT(p1, points);
        ForwardFFT(p2, points);

        for (size_t i = 0; i < n; i++) {
            p1[i] *= p2[i];
        }

        // for (size_t i = 0; i < n; i++) { points[i] = 1.0 / points[i]; }
        for (size_t i = 1, h = n / 2; i < h; i++) { std::swap(points[i], points[n - i]); }
        BackwardFFT(p1, points);

        char* buffer = static_cast<char*>(malloc(n + 1));
        buffer[n] = '\0';
        char* ptr = buffer + n;
        size_t length = 0;
        uint64_t carry = 0;
        for (const auto& m_i : p1) {
            auto m = uint64_t(m_i.real() + 0.5) + carry;
            *--ptr = char(m % 10 + '0');
            length++;
            carry = m / 10;
        }
#else
        auto [min_sz, max_sz] = MinMax(digits.size(), other.digits.size());
        size_t diff = max_sz - min_sz;

        auto number_1_digits_iter = digits.cbegin();
        auto number_2_digits_iter = other.digits.begin();
        auto iter = p1.begin() + i64(max_sz - 1);
        if (digits.size() == max_sz) {
            while (diff-- != 0) {
                *iter = complex(*number_1_digits_iter - '0', 0);
                --iter;
                ++number_1_digits_iter;
            }
        }
        else {
            while (diff-- != 0) {
                *iter = complex(0, *number_2_digits_iter - '0');
                --iter;
                ++number_2_digits_iter;
            }
        }

        // assert(iter == (p1.begin() + (min_sz - 1)));
        while (min_sz-- != 0) {
            *iter = complex(*number_1_digits_iter - '0', *number_2_digits_iter - '0');
            --iter;
            ++number_1_digits_iter;
            ++number_2_digits_iter;
        }

        // if (n >= 4 && n <= 32) { for (const auto& p_i : p1) { cout << p_i << ' '; } cout << '\n'; }

        vector<complex> points(n);
        {
            complex w = std::exp(complex(0, 2 * real(acosl(-1)) / real(n)));
            points[0] = 1;
            points[1] = w;
            complex current = w;
            for (size_t i = 2; i < n; i++) {
                current *= w;
                points[i] = current;
            }
        }

        ForwardFFT(p1, points);

        /*
            * A(w^j) = a_0 + a_1 * w^j + a_2 * w^{2 j} + ... + a_{n - 1} * w^{(n - 1) j}
            * B(w^j) = b_0 + b_1 * w^j + b_2 * w^{2 j} + ... + b_{n - 1} * w^{(n - 1) j}
            * 
            * P = A + B * i = [ A(w^0) + B(w^0) * i, A(w^1) + B(w^1) * i, ... A(w^(n - 1)) + B(w^(n - 1)) * i ]
            * 
            * P(w^j) + conj(P(w^{n - j})) =
            * = A(w^j) + B(w^j) * i + conj(A(w^{n - j}) + B(w^{n - j}) * i) =
            * = \sum{k=0}{n-1} (a_k + b_k * i) * w^{jk} + \sum{k=0}{n-1} conj((a_k + b_k * i) * w^{(n - j)k}) =
            * = \sum{k=0}{n-1} (a_k + b_k * i) * w^{jk} + \sum{k=0}{n-1} conj((a_k + b_k * i) * w^{-jk}) =
            * = \sum{k=0}{n-1} (a_k + b_k * i) * w^{jk} + conj((a_k + b_k * i) * w^{-jk}) =
            * = \sum{k=0}{n-1} (a_k + b_k * i) * w^{jk} + conj(a_k + b_k * i) * conj(w^{-jk}) =
            * = \sum{k=0}{n-1} (a_k + b_k * i) * w^{jk} + (a_k - b_k * i) * w^{jk} =
            * = \sum{k=0}{n-1} 2 a_k * w^{jk} = 2 * A(w^j)
            * 
            * \implies A(w^j) = (P(w^j) + conj(P(w^{n - j}))) / 2
            * 
            * By analogy it is can be proved that A(w^j) = (P(w^j) - conj(P(w^{n - j}))) / (2 * i)
            * 
            * C(w^j) = A(w^j) * B(w^j) \implies C(w^j) = 
            * = (P(w^j) + conj(P(w^{n - j}))) / 2 * (P(w^j) - conj(P(w^{n - j}))) / (2 * i) =
            * = (P(w^j) + conj(P(w^{n - j}))) * (P(w^j) - conj(P(w^{n - j}))) / (4 * i) =
            * = (P(w^j) + conj(P(w^{n - j}))) * (P(w^j) - conj(P(w^{n - j}))) / (4 * i) =
            */

        vector<complex> p2(n);
        complex one_quat_i = complex(0, -0.25); // 1 / (4 * i) == -i / 4
        for (size_t j = 0; j < n; j++) {
            size_t n_j = (n - j) & (n - 1); // <=> mod n because n is power of two
            complex p_w_j = p1[j];
            complex p_w_n_j = std::conj(p1[n_j]);
            p2[j] = (p_w_j + p_w_n_j) * (p_w_j - p_w_n_j) * one_quat_i;
        }

        // for (size_t i = 0; i < n; i++) { points[i] = 1.0 / points[i]; }
        for (size_t i = 1, h = n / 2; i < h; i++) { std::swap(points[i], points[n - i]); }
        BackwardFFT(p2, points);

        char* buffer = static_cast<char*>(malloc(n + 1));
        buffer[n] = '\0';
        char* ptr = buffer + n;
        size_t length = 0;
        uint64_t carry = 0;
        for (const auto& m_i : p2) {
            auto m = uint64_t(m_i.real() + 0.5) + carry;
            *--ptr = char(m % 10 + '0');
            length++;
            carry = m / 10;
        }
#endif
        while (ptr != buffer + n && *ptr == '0') {
            ptr++;
            length--;
        }

        string str(ptr, length);
        free(buffer);
        return Number(std::move(str));
    }

    inline friend std::istream& operator>>(std::istream& in, Number& n) {
        in >> n.digits;
        return in;
    }

    inline friend std::ostream& operator<<(std::ostream& out, const Number& n) {
        out << n.digits;
        return out;
    }
};

inline constexpr size_t MAX_N = 5e4;

static void KNumMul(const char* n1, const char* n2, size_t n, char* const ans) {
    switch (n) {
        case 0:
            ans[0] = '0';
            return;
        case 1: {
            uint32_t res = uint32_t(n1[0] - '0') * uint32_t(n2[0] - '0');
            ans[0] = char(res % 10 + '0');
            ans[1] = char(res / 10 + '0');
            return;
        }
        case 2: {
            uint32_t res = (uint32_t(n1[0] - '0') + uint32_t(n1[1] - '0') * 10) * (uint32_t(n2[0] - '0') + uint32_t(n2[1] - '0') * 10);
            ans[0] = char(res % 10 + '0');
            res /= 10;
            ans[1] = char(res % 10 + '0');
            res /= 10;
            ans[2] = char(res % 10 + '0');
            ans[3] = char(res / 10 + '0');
            return;
        }
    }

    const size_t low_sz = n >> 1;
    const char* a2 = n1;
    size_t a2_size = low_sz;
    const char* b2 = n2;
    size_t b2_size = low_sz;
    size_t a2b2_size = 2 * low_sz;
    assert(a2b2_size == n - (n & 1));
    char* const a2b2 = ans; // static_cast<char*>(malloc(a2b2_size));
    KNumMul(a2, b2, low_sz, a2b2);

    const size_t hi_sz = n - low_sz;
    const char* a1 = n1 + low_sz;
    size_t a1_size = hi_sz;
    const char* b1 = n2 + low_sz;
    size_t b1_size = hi_sz;
    size_t a1b1_size = 2 * hi_sz;
    assert(a1b1_size == n + (n & 1));
    char* const a1b1 = ans + a2b2_size; // static_cast<char*>(malloc(a1b1_size));
    assert(a1b1 != nullptr);
    KNumMul(a1, b1, hi_sz, a1b1);

    assert(a1_size >= a2_size);
    // auto [min_sz_a, max_sz_a] = MinMax(a1_size, a2_size);
    size_t a1_p_a2_size = a1_size + 1;
    assert(a1_p_a2_size == (n - n / 2) + 1);
    char* const a1_p_a2 = static_cast<char*>(malloc(a1_p_a2_size));
    assert(a1_p_a2 != nullptr);
    {
        uint32_t res;
        uint32_t carry = 0;
        char* a1_p_a2_p = a1_p_a2;
        const char* a1_ptr = a1;
        const char* a2_ptr = a2;
        for (size_t i = 0; i < a2_size; i++) {
            assert(a1_ptr < n1 + n);
            assert(a2_ptr < n1 + n);
            res = uint32_t(*a1_ptr - '0') + uint32_t(*a2_ptr - '0') + carry;
            *a1_p_a2_p = char(res % 10 + '0');
            a1_p_a2_p++;
            a1_ptr++;
            a2_ptr++;
            carry = res / 10;
        }

        size_t limit = a1_size - a2_size;
        // Optimize while (carry != 0) + memset '0'
        while (limit-- != 0) {
            assert(a1_ptr < n1 + n);
            res = uint32_t(*a1_ptr - '0') + carry;
            a1_ptr++;
            *a1_p_a2_p = char(res % 10 + '0');
            a1_p_a2_p++;
            carry = res / 10;
        }

        *a1_p_a2_p = char(carry + '0');
    }

    assert(b1_size >= b2_size);
    // auto [min_sz_b, max_sz_b] = MinMax(b1_size, b2_size);
    size_t b1_p_b2_size = b1_size + 1;
    char* const b1_p_b2 = static_cast<char*>(malloc(b1_p_b2_size));
    assert(b1_p_b2 != nullptr);
    {
        uint32_t res;
        uint32_t carry = 0;
        char* b1_p_b2_p = b1_p_b2;
        const char* b1_ptr = b1;
        const char* b2_ptr = b2;
        for (size_t i = 0; i < b2_size; i++) {
            assert(b1_ptr < n2 + n);
            assert(b2_ptr < n2 + n);
            res = uint32_t(*b1_ptr - '0') + uint32_t(*b2_ptr - '0') + carry;
            *b1_p_b2_p = char(res % 10 + '0');
            b1_p_b2_p++;
            b1_ptr++;
            b2_ptr++;
            carry = res / 10;
        }

        size_t limit = b1_size - b2_size;
        // Optimize while (carry != 0) + memset '0'
        while (limit-- != 0) {
            assert(b1_ptr < n2 + n);
            res = uint32_t(*b1_ptr - '0') + carry;
            b1_ptr++;
            *b1_p_b2_p = char(res % 10 + '0');
            b1_p_b2_p++;
            carry = res / 10;
        }

        *b1_p_b2_p = char(carry + '0');
    }

    assert(a1_p_a2_size == b1_p_b2_size);
    if (a1_p_a2_size > 1 && a1_p_a2[a1_p_a2_size - 1] == '0' && b1_p_b2[b1_p_b2_size - 1] == '0') {
        a1_p_a2_size--;
        b1_p_b2_size--;
    }

    size_t t_size = a1_p_a2_size * 2;
    char* const t = static_cast<char*>(malloc(t_size));
    assert(t != nullptr);
    KNumMul(a1_p_a2, b1_p_b2, a1_p_a2_size, t);

    free(b1_p_b2);
    free(a1_p_a2);

    {
        assert(t_size >= a1b1_size && a1b1_size >= a2b2_size);
        uint32_t carry = 0;
        uint32_t res;
        char* n1_ptr = t;
        const char* n2_ptr = a1b1;
        const char* n3_ptr = a2b2;

        size_t limit = a2b2_size;
        while (limit--) {
            assert(n1_ptr < t + t_size);
            res = uint32_t(*n1_ptr - '0');
            assert(n2_ptr < a1b1 + a1b1_size);
            assert(n3_ptr < a2b2 + a2b2_size);
            uint32_t sub = uint32_t(*n2_ptr + *n3_ptr - '0' - '0') + carry;
            carry = 0;
            if (res < sub) {
                res += 10; carry++;
                if (res < sub) {
                    res += 10; carry++;
                }
            }

            assert(res >= sub);
            *n1_ptr = char(res - sub + '0');
            n1_ptr++;
            n2_ptr++;
            n3_ptr++;
        }

        limit = a1b1_size - a2b2_size;
        while (limit--) {
            assert(n1_ptr < t + t_size);
            res = uint32_t(*n1_ptr - '0');
            assert(n2_ptr < a1b1 + a1b1_size);
            assert(carry <= 2);
            uint32_t sub = uint32_t(*n2_ptr - '0') + carry;
            carry = 0;
            if (res < sub) {
                res += 10; carry++;
                if (res < sub) {
                    res += 10; carry++;
                }
            }

            assert(res >= sub);
            *n1_ptr = char(res - sub + '0');
            n1_ptr++;
            n2_ptr++;
        }

        limit = t_size - a1b1_size;
        while (limit-- && carry != 0){
            assert(n1_ptr < t + t_size);
            res = uint32_t(*n1_ptr - '0');
            uint32_t sub = carry;
            carry = 0;
            if (res < sub) {
                res += 10; carry++;
            }

            assert(res >= sub);
            *n1_ptr = char(res - sub + '0');
            n1_ptr++;
        }

        assert(carry == 0);
    }

    // memcpy(ans, a2b2, a2b2_size);
    // memcpy(ans + a2b2_size, a1b1, a1b1_size);
    // free(a1b1);
    // free(a2b2);

    while (t_size != 0 && t[t_size - 1] == '0') {
        t_size--;
    }

    char* ans_p = ans + n / 2;
    uint32_t carry = 0;
    assert(t_size <= n + 2 + n % 2);
    for (const char* t_iter = t, *t_iter_end = t + t_size; t_iter != t_iter_end; ++t_iter) {
        assert(ans_p < ans + 2 * n);
        uint32_t res = uint32_t(*ans_p - '0') + uint32_t(*t_iter - '0') + carry;
        *ans_p = char(res % 10 + '0');
        ans_p++;
        carry = res / 10;
    }

    ssize_t limit = ssize_t(2 * n - n / 2) - ssize_t(t_size);
    while (limit-- > 0 && carry != 0) {
        assert(ans_p < ans + 2 * n);
        uint32_t res = uint32_t(*ans_p - '0') + carry;
        *ans_p = char(res % 10 + '0');
        ans_p++;
        carry = res / 10;
    }

    free(t);
    assert(carry == 0);
}

static inline void Reverse(char* s, size_t size) {
    char* s_i = s;
    char* s_j = s_i + size - 1;
    while (s_i < s_j) {
        auto tmp = *s_j;
        *s_j = *s_i;
        *s_i = tmp;
        ++s_i;
        --s_j;
    }
}

// Free ans by free(void*);
inline string_view KaratsubaMult(string& s1, string& s2) {
    char* n1 = s1.data();
    size_t n1_size = s1.size();
    char* n2 = s2.data();
    size_t n2_size = s2.size();

    Reverse(n1, n1_size);
    Reverse(n2, n2_size);
    size_t n = std::max(n1_size, n2_size);
    if (n1_size != n) {
        memset(n1 + n1_size, '0', n - n1_size);
    }
    if (n2_size != n) {
        memset(n2 + n2_size, '0', n - n2_size);
    }

    char* mult = static_cast<char*>(malloc(MAX_N + 1));
    assert(mult != nullptr);
    KNumMul(n1, n2, n, mult);
    size_t number_size = n * 2;
    while (number_size > 1 && mult[number_size - 1] == '0') {
        number_size--;
    }
    mult[number_size] = '\0';
    Reverse(mult, number_size);
    return string_view(mult, number_size);
}

int main() {
    constexpr size_t n = 5000;
    string n1(n, '\0');
    string n2(n, '\0');

    for (size_t i = 0; i < n; i++) {
        n1[i] = char('0' + (uint32_t(rand()) % 10));
        n2[i] = char('0' + (uint32_t(rand()) % 10));
    }
    if (n1[0] == '0') { n1[0] = '1'; }
    if (n2[0] == '0') { n2[0] = '1'; }
    for (size_t i = 0; i < n; i++) { assert(uint32_t(n1[i] - '0') <= '9' - '0' && uint32_t(n2[i] - '0') <= '9' - '0'); }

    Number m_fft = Number(n1) * Number(n2);
    auto m_k = KaratsubaMult(n1, n2);
    if (m_fft.digits != m_k) {
        cout << n1 << ' ' << n2 << '\n';
        cout << m_fft.digits << '\n';
        cout << m_k << '\n';
    }
    free(const_cast<char*>(m_k.data()));
}
