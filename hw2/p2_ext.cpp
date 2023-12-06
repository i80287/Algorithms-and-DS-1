#include <cassert>
#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>

using std::string;
using std::string_view;

inline static constexpr std::pair<size_t, size_t> MinMax(size_t a, size_t b) {
    return b < a ? std::pair(b, a) : std::pair(a, b);
}

static inline void KNumAdd(string_view n1, string_view n2, string& ans) {
    auto [min_sz, max_sz] = MinMax(n1.size(), n2.size());
    ans.resize(max_sz + 1);
    char* ans_p = ans.data();
    uint32_t res;
    uint32_t carry = 0;
    for (size_t i = 0; i < min_sz; i++) {
        res = uint32_t(n1.at(i) - '0') + uint32_t(n2.at(i) - '0') + carry;
        *ans_p = char(res % 10 + '0');
        ans_p++;
        carry = res / 10;
    }

    const char* n_s = n1.size() == max_sz ? n1.data() : n2.data();
    n_s += min_sz;
    size_t limit = max_sz - min_sz;
    // Optimize while (carry != 0)
    while (limit-- != 0) {
        res = uint32_t(*n_s - '0') + carry;
        n_s++;
        *ans_p = char(res % 10 + '0');
        ans_p++;
        carry = res / 10;
    }

    *ans_p = char(carry + '0');
}

static inline void KNumSubTwoInplace(string& n1, const string& n2, const string& n3) {
    assert(n1.size() >= n2.size() && n2.size() >= n3.size());
    uint32_t carry = 0;
    uint32_t res;
    char* n1_ptr = n1.data();
    const char* n2_ptr = n2.data();
    const char* n3_ptr = n3.data();

    size_t limit = n3.size();
    while (limit--) {
        res = uint32_t(*n1_ptr - '0');
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

    limit = n2.size() - n3.size();
    while (limit--) {
        res = uint32_t(*n1_ptr - '0');
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

    limit = n1.size() - n2.size();
    while (limit-- && carry != 0){
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

static void KNumMul(string_view n1, string_view n2, string& ans) {
    const size_t n = n1.size();
    assert(n == n2.size());
    switch (n) {
        case 0:
            ans.resize(1);
            ans[0] = '0';
            return;
        case 1: {
            uint32_t res = uint32_t(n1[0] - '0') * uint32_t(n2[0] - '0');
            ans.resize(2);
            ans[0] = char(res % 10 + '0');
            ans[1] = char(res / 10 + '0');
            return;
        }
        case 2: {
            uint32_t res = (uint32_t(n1[0] - '0') + uint32_t(n1[1] - '0') * 10) * (uint32_t(n2[0] - '0') + uint32_t(n2[1] - '0') * 10);
            ans.resize(4);
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
    auto a2 = string_view(n1.data(), low_sz);
    auto b2 = string_view(n2.data(), low_sz);
    string a2b2;
    KNumMul(a2, b2, a2b2);
    assert(a2b2.size() == n - (n & 1));

    const size_t hi_sz = n - low_sz;
    auto a1 = string_view(n1.data() + low_sz, hi_sz);
    auto b1 = string_view(n2.data() + low_sz, hi_sz);
    string a1b1;
    KNumMul(a1, b1, a1b1);
    assert(a1b1.size() == n + (n & 1));

    string a1_p_a2;
    KNumAdd(a1, a2, a1_p_a2);
    string b1_p_b2;
    KNumAdd(b1, b2, b1_p_b2);
    if (a1_p_a2.back() == '0' && b1_p_b2.back() == '0') {
        a1_p_a2.pop_back();
        b1_p_b2.pop_back();
    }
    string t;
    KNumMul(a1_p_a2, b1_p_b2, t);
    KNumSubTwoInplace(t, a1b1, a2b2);

    while (!t.empty() && t.back() == '0') {
        t.pop_back();
    }

    ans.resize(n * 2);
    memcpy(ans.data(), a2b2.c_str(), a2b2.size());
    memcpy(ans.data() + a2b2.size(), a1b1.c_str(), a1b1.size());

    char* ans_p = ans.data() + n / 2;
    uint32_t carry = 0;
    for (char c : t) {
        uint32_t res = uint32_t(*ans_p - '0') + uint32_t(c - '0') + carry;
        *ans_p = char(res % 10 + '0');
        ans_p++;
        carry = res / 10;
    }

    size_t n1_sz = 2 * n - n / 2;
    assert(n1_sz >= t.size());
    size_t limit = n1_sz - t.size();
    while (limit-- != 0 && carry != 0) {
        assert(ans_p < ans.data() + 2 * n);
        uint32_t res = uint32_t(*ans_p - '0') + carry;
        *ans_p = char(res % 10 + '0');
        ans_p++;
        carry = res / 10;
    }

    assert(carry == 0);
}

static inline void Reverse(string& s) {
    char* s_i = s.data();
    char* s_j = s_i + s.size() - 1;
    while (s_i < s_j) {
        auto tmp = *s_j;
        *s_j = *s_i;
        *s_i = tmp;
        ++s_i;
        --s_j;
    }
}

string KaratsubaMult(string& n1, string& n2) {
    Reverse(n1);
    Reverse(n2);
    size_t n = std::max(n1.size(), n2.size());
    if (n1.size() != n) {
        n1.resize(n, '0');
    }
    if (n2.size() != n) {
        n2.resize(n, '0');
    }

    string ans;
    KNumMul(n1, n2, ans);
    while (ans.size() > 1 && ans.back() == '0') {
        ans.pop_back();
    }
    Reverse(ans);
    return ans;
}

void TestKNumberAdd() {
    std::string s1, s2, s3, s4;
    s1.reserve(6);
    s2.reserve(6);
    s3.reserve(6);
    s4.reserve(6);
    for (uint32_t i = 0; i <= 5000; i++) {
        for (uint32_t j = 0; j <= 5000; j++) {
            s1 = std::to_string(i);
            s2 = std::to_string(j);
            s3 = std::to_string(i + j);
            Reverse(s1);
            Reverse(s2);
            Reverse(s3);
            if (s3.size() < std::max(s1.size(), s2.size()) + 1) { s3.push_back('0'); }
            KNumAdd(s1, s2, s4);
            assert(s4 == s3);
        }
    }

    for (uint64_t i = (1ull << 63) - 1; i >= (1ull << 63) - 5001; i--) {
        for (uint64_t j = (1ull << 63) - 1; j >= (1ull << 63) - 5001; j--) {
            s1 = std::to_string(i);
            s2 = std::to_string(j);
            s3 = std::to_string(i + j);
            Reverse(s1);
            Reverse(s2);
            Reverse(s3);
            if (s3.size() < std::max(s1.size(), s2.size()) + 1) { s3.push_back('0'); }
            KNumAdd(s1, s2, s4);
            assert(s4 == s3);
        }
    }
}

[[maybe_unused]] static inline void TestKNumberSubTwo() {
    string s1 = "87269130";
    string s2 = "19685626";
    string s3 = "21903112";
    Reverse(s1);
    Reverse(s2);
    Reverse(s3);
    KNumSubTwoInplace(s1, s2, s3);
    Reverse(s1);
    assert(s1 == "45680392");
}

[[maybe_unused]] static inline void TestKNumberMult() {
    string s1, s2, s3;
    s1.reserve(6);
    s2.reserve(6);
    s3.reserve(6);
    for (uint32_t i = 0; i < 4096; i++) {
        for (uint32_t j = 0; j < 4096; j++) {
            s1 = std::to_string(i);
            s2 = std::to_string(j);
            s3 = KaratsubaMult(s1, s2);
            assert(s3 == std::to_string(i * j));
        }
    }
}

[[maybe_unused]] static inline void Tests() {
    puts("started tests");
    TestKNumberAdd();
    puts("add tests done");
    TestKNumberSubTwo();
    puts("sub tests done");
    TestKNumberMult();
    puts("mult tests done");
    puts("tests done");
}

int main() {
    Tests();
    // string n1 = "0";
    // string n2 = "919";
    // auto a = KaratsubaMult(n1, n2);
    // puts(a.c_str());
}
