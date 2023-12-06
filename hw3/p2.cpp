#include <cstdint>
#include <cstring>
#include <iostream>
#include <iterator>
#if __cplusplus >= 202002L
#include <type_traits>
#endif
#include <utility>

template <class Iterator>
#if __cplusplus >= 202002L
requires std::is_integral_v<typename std::iterator_traits<Iterator>::value_type>
#endif
void countingSort(Iterator iter_begin, Iterator iter_end) {
    if (iter_begin == iter_end) {
        return;
    }

    auto min_elem = *iter_begin;
    auto max_elem = min_elem;
    for (Iterator iter = iter_begin + 1; iter != iter_end; ++iter) {
        auto current_elem = *iter;
        if (current_elem < min_elem) {
            min_elem = current_elem;
        }
        else if (current_elem > max_elem) {
            max_elem = current_elem;
        }
    }

    size_t threshold = static_cast<size_t>(max_elem - min_elem) + 1;
    uint32_t* const counters = static_cast<uint32_t*>(operator new(threshold * sizeof(uint32_t)));
    memset(counters, 0, threshold * sizeof(uint32_t));
    for (Iterator iter = iter_begin; iter != iter_end; ++iter) {
        size_t i = static_cast<size_t>(*iter - min_elem);
        counters[i]++;
    }

    for (uint32_t* counters_iter = counters, *counters_end = counters_iter + threshold; counters_iter != counters_end; ++counters_iter, ++min_elem) {
        uint32_t count = *counters_iter;
        while (count--) {
            *iter_begin = min_elem;
            ++iter_begin;
        }
    }

    operator delete(counters);
}

template <class Iterator>
#if __cplusplus >= 202002L
requires std::is_integral_v<typename std::iterator_traits<Iterator>::value_type>
#endif
void countingSort(Iterator iter_begin, size_t length) {
    countingSort(iter_begin, iter_begin + length);
}

int main() {
    using std::cin, std::cout;
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);
    uint32_t n = 0;
    cin >> n;
    int32_t* array = static_cast<int32_t*>(operator new(n * sizeof(int32_t)));
    for (int32_t* iter = array, *iter_end = iter + n; iter != iter_end; ++iter) {
        cin >> *iter;
    }
    countingSort(array, static_cast<size_t>(n));
    for (int32_t* iter = array, *iter_end = iter + n; iter != iter_end; ++iter) {
        cout << *iter << ' ';
    }
    operator delete(array);
}
