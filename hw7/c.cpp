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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string p;
    string t;
    p.reserve(1010);
    t.reserve(size_t(1e7 + 10));
    cin >> p >> t;
    const auto pi = prefix_function(p);
    vector<size_t> ans;
    ans.reserve(size_t(1e6) + 10);
    size_t p_i = 0;
    size_t t_i = 0;
    while (t_i < t.size()) {
        if (uint8_t(p[p_i]) == uint8_t(t[t_i])) {
            p_i++;
            t_i++;
            if (p_i == p.size()) [[unlikely]] {
                assert(t_i >= p.size());
                ans.push_back(t_i - p_i);
                p_i = pi[p_i - 1];
            }
        } else if (p_i == 0) {
            t_i++;
        } else {
            p_i = pi[p_i - 1];
        }
    }
    cout << ans.size() << '\n';
    for (size_t i : ans) {
        cout << uint32_t(i) << '\n';
    }
}
