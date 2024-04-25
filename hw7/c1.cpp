#pragma GCC optimize("O2")
#define NDEBUG

#include <vector>
#include <string_view>
#include <cstdint>
#include <string>
#include <cstring>
#include <cassert>

using namespace std;

int main() {
    uint8_t p[size_t(1e3) + 3];
    static uint8_t t[size_t(1e7) + 3];
#ifndef NDEBUG
    auto ret =
#endif
    fgets(reinterpret_cast<char*>(p), 1000 + 2, stdin);
    assert(ret);
    size_t p_size = strlen(reinterpret_cast<const char*>(p));
    assert(p_size >= 2);
    assert(p[p_size - 1] == '\n');
    p[p_size - 1] = '\0';
    p_size--;
    assert(isalpha(p[0]));
    assert(isalpha(p[p_size - 1]));

#ifndef NDEBUG
    ret =
#endif
    fgets(reinterpret_cast<char*>(t), 10'000'000 + 2, stdin);
    assert(ret);
    size_t t_size = strlen(reinterpret_cast<const char*>(t));
    assert(t_size >= 2);
    if (t[t_size - 1] == '\n') {
        t[t_size - 1] = '\0';
        t_size--;
    }
    assert(isalpha(t[0]));
    assert(isalpha(t[t_size - 1]));

    size_t pi[1000] = {};
    {
        const size_t n = p_size;
        for (size_t i = 1; i < n; i++) {
            size_t j = pi[i - 1];
            uint32_t c_i = p[i];
            while (j > 0 && c_i != p[j]) {
                assert(j - 1 < n);
                j = pi[j - 1];
            }
            assert(j < n);
            if (c_i == p[j]) {
                j++;
            }
            pi[i] = j;
        }

        for (size_t i = 0; i + 1 < n; i++) {
            if (pi[i] + 1 == pi[i + 1]) {
                pi[i] = 0;
            }
        }
    }

    vector<size_t> ans(size_t(1e6) + 10);
    size_t ans_size = 0;
    // size_t iters = 0;
    for (size_t t_i = 0, p_i = 0; t_i < t_size; t_i++) {
        uint32_t c = t[t_i];
        assert(p_i < p_size);
        // iters++;
        while (p_i > 0 && p[p_i] != c) {
            // iters++;
            p_i = pi[p_i - 1];
        }

        assert(p_i < p_size);
        if (p[p_i] == c) {
            p_i++;
        }

        if (p_i == p_size) {
            assert(ans_size < ans.size());
            ans[ans_size++] = t_i + 1 - p_i;
            p_i = pi[p_i - 1];
        }
    }

#ifndef NDEBUG
    int reti =
#endif
    setvbuf(stdout, reinterpret_cast<char*>(t), _IOFBF, sizeof(t) - 1);
    assert(reti == 0);
    printf("%u\n", uint32_t(ans_size));
    for (size_t i = 0; i < ans_size; i++) {
        printf("%u\n", uint32_t(ans[i]));
    }
    fflush(stdout);
    // printf("iters=%zu\n", iters);
}
