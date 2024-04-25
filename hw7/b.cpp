#include <vector>
#include <string_view>
#include <cstdint>
#include <iostream>

using namespace std;

static vector<uint32_t> z_function(string_view s) {
	const size_t n = s.length();
	vector<uint32_t> z(n);
	for (size_t i = 1, l = 0, r = 0; i < n; i++) {
		if (i <= r) {
            z[i] = std::min(z[i - l], uint32_t(r - i + 1));
        }
        while (i + z[i] < n && uint8_t(s[z[i]]) == uint8_t(s[i + z[i]])) {
            z[i]++;
        }
        size_t cur_r = i + z[i] - 1;
        if (cur_r > r) {
            r = cur_r;
            l = i;
        }
	}

	return z;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string p;
    p.reserve(size_t(1e6 + 2));
    cin >> p;
    const auto zf = z_function(p);
    if (zf.size() <= 1) {
        cout << '1';
    }
    else for (size_t i = 1; i < zf.size(); i++) {
        if (zf[i] == zf.size() - i) {
            cout << i;
            break;
        }
    }
}
