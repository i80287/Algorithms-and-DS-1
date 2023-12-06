#pragma once

#include <cstdint>
#include <cstring>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

template <class T>
#if __cplusplus >= 202002L
    requires std::is_arithmetic_v<T>
#endif
void MergeSortImpl(T array[], T buffer[], std::size_t l, std::size_t r) {
    switch (r - l) {
        case 0:
            return;
        case 1:
            if (array[l] > array[l + 1]) {
                std::swap(array[l], array[l + 1]);
            }
            return;
    }

    std::size_t m = (l + r) / 2;
    MergeSortImpl(array, buffer, l, m);
    MergeSortImpl(array, buffer, m + 1, r);

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

template <class Iterator>
#if __cplusplus >= 202002L
    requires std::is_arithmetic_v<
        typename std::iterator_traits<Iterator>::value_type>
#endif
inline void MergeSort(Iterator begin, Iterator end) {
    auto n = static_cast<std::size_t>(end - begin);
    if (n == 0) return;

    using value_type = typename std::iterator_traits<Iterator>::value_type;
    std::unique_ptr<value_type[]> buffer = std::make_unique<value_type[]>(n);
    MergeSortImpl(&(*begin), buffer.get(), 0, n - 1);
}
