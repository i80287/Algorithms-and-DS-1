#pragma once

#include <cstdint>
#include <cstring>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

template <typename Iterator>
constexpr void InsertionSort(Iterator iter, size_t n) {
    for (size_t i = 1; i < n; i++) {
        auto tmp = std::move(*(iter + i));
        size_t j = i;
        for (; j != 0 && *(iter + (j - 1)) > tmp; j--) {
            *(iter + j) = std::move(*(iter + (j - 1)));
        }
        *(iter + j) = std::move(tmp);
    }
}

template <size_t Cutoff, class T>
#if __cplusplus >= 202002L
    requires std::is_arithmetic_v<T>
#endif
void HybridSortImpl(T array[], T buffer[], std::size_t l, std::size_t r) {
    size_t n = r - l + 1;
    if (n <= Cutoff) {
        InsertionSort(&array[l], n);
        return;
    }

    std::size_t m = (l + r) / 2;
    HybridSortImpl<Cutoff>(array, buffer, l, m);
    HybridSortImpl<Cutoff>(array, buffer, m + 1, r);

    std::size_t l1 = l;
    std::size_t l2 = m + 1;
    std::size_t l3 = 0;
    while (l1 <= m && l2 <= r) {
        if (array[l1] <= array[l2]) {
            buffer[l3] = array[l1];
            l1++;
        } else {
            buffer[l3] = array[l2];
            l2++;
        }
        l3++;
    }

    if (l1 <= m) {
        memmove(&array[l + l3], &array[l1], (m - l1 + 1) * sizeof(T));
    }

    memcpy(&array[l], &buffer[0], l3 * sizeof(T));
}

template <size_t Cutoff, class Iterator>
#if __cplusplus >= 202002L
    requires std::is_arithmetic_v<
        typename std::iterator_traits<Iterator>::value_type>
#endif
inline void HybridSort(Iterator begin, Iterator end) {
    auto n = static_cast<std::size_t>(end - begin);
    if (n == 0) return;

    using value_type = typename std::iterator_traits<Iterator>::value_type;
    std::unique_ptr<value_type[]> buffer = std::make_unique<value_type[]>(n);
    HybridSortImpl<Cutoff>(&(*begin), buffer.get(), 0, n - 1);
}
