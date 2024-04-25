#include <vector>
#include <string_view>
#include <cstdint>
#include <iostream>
#include <string>
#include <cstring>
#include <cassert>

using namespace std;

static vector<size_t> prefix_function(string_view s) {
	const size_t n = s.length();
	vector<size_t> pi(n);
	for (size_t i = 1; i < n; i++) {
		size_t j = pi[i - 1];
		uint32_t c_i = uint8_t(s[i]);
		while (j > 0 && c_i != uint8_t(s[j])) {
            assert(j - 1 < n);
            j = pi[j - 1];
        }
        assert(j < n);
		if (c_i == uint8_t(s[j])) {
			j++;
		}
		pi[i] = j;
	}

    for (size_t i = 0; i + 1 < n; i++) {
        if (pi[i] + 1 == pi[i + 1]) {
            pi[i] = 0;
        }
    }

	return pi;
}

int main() {
    static uint8_t p[size_t(1e3) + 3];
    static uint8_t t[size_t(1e7) + 3];
    auto ret = fgets(reinterpret_cast<char*>(p), 1000 + 2, stdin);
    assert(ret);
    size_t p_size = strlen(reinterpret_cast<const char*>(p));
    assert(p_size >= 2);
    assert(p[p_size - 1] == '\n');
    p[p_size - 1] = '\0';
    p_size--;
    assert(isalpha(p[0]));
    assert(isalpha(p[p_size - 1]));

    ret = fgets(reinterpret_cast<char*>(t), 10'000'000 + 2, stdin);
    assert(ret);
    size_t t_size = strlen(reinterpret_cast<const char*>(t));
    assert(t_size >= 2);
    if (t[t_size - 1] == '\n') {
        t[t_size - 1] = '\0';
        t_size--;
    }
    assert(isalpha(t[0]));
    assert(isalpha(t[t_size - 1]));
    // cin >> p >> t;
    const auto pi = prefix_function(string_view(reinterpret_cast<const char*>(p), p_size));
    vector<size_t> ans(size_t(1e6) + 10);
    assert(ans.size() == 1'000'010);
    size_t ans_size = 0;
    size_t p_i = 0;
    size_t t_i = 0;
    while (t_i < t_size) {
        if (uint8_t(p[p_i]) == uint8_t(t[t_i])) {
            p_i++;
            t_i++;
            if (p_i == p_size) {
                assert(t_i >= p_i);
                ans[ans_size++] = t_i - p_i;
            }
        } else if (p_i == 0) {
            t_i++;
        } else {
            p_i = pi[p_i - 1];
        }
    }
    printf("%u\n", uint32_t(ans_size));
    for (size_t i = 0; i < ans_size; i++) {
        printf("%u\n", uint32_t(ans[i]));
    }
}
