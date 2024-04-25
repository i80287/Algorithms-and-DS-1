#include <vector>
#include <string_view>
#include <cstdint>
#include <iostream>

using namespace std;

static vector<uint32_t> prefix_function(string_view s) {
	const size_t n = s.length();
	vector<uint32_t> pi(n);
	for (size_t i = 1; i < n; i++) {
		size_t j = pi[i - 1];
		uint32_t c_i = uint8_t(s[i]);
		while (j > 0 && c_i != uint8_t(s[j])) {
            j = pi[j - 1];
        }
		if (c_i == uint8_t(s[j])) {
			j++;
		}
		pi[i] = uint32_t(j);
	}

	return pi;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    s.reserve(size_t(1e6 + 2));
    cin >> s;
    for (uint32_t x : prefix_function(s)) {
        cout << x << ' ';
    }
}
