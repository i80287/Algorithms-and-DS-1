#include <cstdint>
#include <cstdio>
#include <cmath>
#include <vector>
#include <cstring>
#include <cassert>

using std::vector;

size_t slowMergeSortWithCounting(vector<int64_t>& array, size_t l, size_t r) {
    switch(r - l) {
        case 0:
            return 0;
        case 1:
            size_t current_count = array[l] > 2 * array[l + 1];
            if (array[l] > array[l + 1]) {
                std::swap(array[l], array[l + 1]);
            }
            return current_count;
    }
    size_t m = (l + r) / 2;
    size_t left_count = slowMergeSortWithCounting(array, l, m);
    size_t right_count = slowMergeSortWithCounting(array, m + 1, r);

    size_t current_count = 0;
    for (size_t l1 = l, l2 = m + 1; l1 <= m && l2 <= r;  l2++) {
        while (l1 <= m && array[l1] <= 2 * array[l2]) {
            l1++;
        }
        current_count += m - l1 + 1;
    }

    vector<int64_t> tmp(r - l + 1);
    size_t l1 = l;
    size_t l2 = m + 1;
    size_t l3 = 0;
    while (l1 <= m && l2 <= r) {
        if (array[l1] <= array[l2]) {
            tmp[l3] = array[l1];
            l1++;
        }
        else {
            tmp[l3] = array[l2];
            l2++;
        }
        l3++;
    }
    while (l1 <= m) {
        tmp[l3] = array[l1];
        l1++;
        l3++;
    }
    while (l2 <= r) {
        tmp[l3] = array[l2];
        l2++;
        l3++;
    }
    memcpy(&array[l], &tmp[0], tmp.size() * sizeof(tmp[0]));
    return left_count + right_count + current_count;
}

size_t countPermutations(const vector<int64_t>& array) {
    if (array.empty()) {
        return 0;
    }

    vector<int64_t> copy(array);
    return slowMergeSortWithCounting(copy, 0, array.size() - 1);
}

#include <random>
int main() {
    constexpr size_t N = size_t(5e3);
    vector<int64_t> array(N);
    std::mt19937 rnd;
    uint32_t limit = 16;
    while (limit-- != 0) {
        for (int64_t& a_i : array) {
            a_i = int64_t(int32_t(rnd()));
        }

        size_t ans0 = 0;
        size_t i = 1;
        for (int64_t a_i : array) {
            for (size_t j = i; j < N; j++) {
                ans0 += a_i > 2 * array[j];
            }
            i++;
        }
        auto ans1 = slowMergeSortWithCounting(array, 0, array.size() - 1);
        assert(array.size() == N);
        for (i = 1; i < N; i++) {
            assert(array[i - 1] <= array[i]);
        }
        assert(ans0 == ans1);
    }
}