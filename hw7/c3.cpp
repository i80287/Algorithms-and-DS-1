#include <vector>
#include <string_view>
#include <cstdint>
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

static vector<size_t> prefix_function(string_view s) {
	const size_t n = s.length();
	vector<size_t> pi(n);
	for (size_t i = 1; i < n; i++) {
		size_t j = pi[i - 1];
		uint32_t c_i = uint8_t(s[i]);
		while (j > 0 && c_i != uint8_t(s[j])) {
            j = pi[j - 1];
        }
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

void verify(string_view text, string_view substr, const std::vector<size_t>& ans) {
	string s;
	s.reserve(substr.size() + 1 + text.size());
	s.append(substr);
	s.push_back('#');
	s.append(text);
	vector<size_t> pref_func = prefix_function(s);
	auto it = pref_func.begin() + static_cast<std::ptrdiff_t>(substr.size() + 1); // Skip "substr#"
	auto end = pref_func.end();
    size_t j = 0;
	for (size_t i = 0; it != end; ++it, ++i) {
		if (*it == substr.size()) {
            size_t start_index = i + 1 - substr.size();
            assert(j < ans.size());
            assert(ans[j] == start_index);
            j++;
		}
	}
    assert(j == ans.size());
}

#include <random>

int main() {
    // ios::sync_with_stdio(false);
    // cin.tie(nullptr);
    // string p;
    // string t;
    // p.reserve(1010);
    // t.reserve(size_t(1e7 + 10));
    // cin >> p >> t;

    constexpr size_t P_SIZE = 200;
    constexpr size_t T_SIZE = size_t(1e6);
    string p(P_SIZE, '\0');
    string t(T_SIZE, '\0');
    std::mt19937 rnd;
    while (true) {
        for (char& c : p) {
            c = char('a' + rnd() % ('z' - 'a' + 1));
        }
        for (char& c : t) {
            c = char('a' + rnd() % ('z' - 'a' + 1));
        }

        const auto pi = prefix_function(p);
        vector<size_t> ans;
        ans.reserve(size_t(1e6) + 10);
        size_t p_i = 0;
        size_t t_i = 0;
        while (t_i < t.size()) {
            if (uint8_t(p[p_i]) == uint8_t(t[t_i])) {
                p_i++;
                t_i++;
                if (p_i == p.size()) {
                    assert(t_i >= p.size());
                    ans.push_back(t_i - p_i);
                }
            } else if (p_i == 0) {
                t_i++;
            } else {
                p_i = pi[p_i - 1];
            }
        }
        // cout << ans.size() << '\n';
        // for (size_t i : ans) {
        //     cout << uint32_t(i) << '\n';
        // }
        verify(t, p, ans);
        puts("ok");
    }
}
