#include <algorithm>
#include <cstdint>
#include <vector>
#include <set>
#include <iostream>

// ordinary O(n^2)
size_t CountArrayRotations(const std::vector<int>& array) {
    size_t max_cnt = 0;
    size_t max_cnt_cnt = 0;
    std::set<uint64_t> hs1;
    std::set<uint64_t> hs2;
    for (size_t shift = 0, len = array.size(); shift < len; shift++) {
        size_t i = shift;
        size_t cnt = 0;

        uint64_t h1 = 0;
        constexpr uint64_t P1 = 1e9 + 7;
        uint64_t h2 = 0;
        constexpr uint64_t P2 = 257;
        for (int elem : array) {
            int shifted = array.at(i);
            cnt += elem != shifted;
            i++;
            i *= (i != len);
            h1 = static_cast<uint32_t>(shifted) + h1 * P1;
            h2 = static_cast<uint32_t>(shifted) + h2 * P2;
        }

        if (cnt >= max_cnt) {
            if (cnt > max_cnt) {
                max_cnt = cnt;
                max_cnt_cnt = 0;
                hs1.clear();
                hs2.clear();
            } else if (hs1.find(h1) != hs1.end() && hs2.find(h2) != hs2.end()) {
                continue;
            }

            hs1.insert(h1);
            hs2.insert(h2);
            max_cnt_cnt++;
        }
    }

    return max_cnt_cnt;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    size_t size;
    std::cin >> size;
    std::vector<int> array(size);
    for (size_t i = 0; i < size; ++i) {
        std::cin >> array[i];
    }

    std::cout << CountArrayRotations(array);
    std::cout.flush();
    return 0;
}
