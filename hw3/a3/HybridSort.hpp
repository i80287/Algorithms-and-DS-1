#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <random>
#include <utility>

template <class Iterator, class Comparator>
static inline void BuildHeap(Iterator begin, size_t length,
                             Comparator comparator) {
    for (size_t i = length / 2; i != 0;) {
        size_t parent_index = --i;

        size_t son_index = parent_index * 2 | 1;
        if (son_index + 1 < length &&
            comparator(*(begin + ptrdiff_t(son_index)),
                       *(begin + ptrdiff_t(son_index + 1)))) {
            ++son_index;
        }

        while (comparator(*(begin + ptrdiff_t(parent_index)),
                          *(begin + ptrdiff_t(son_index)))) {
            std::swap(*(begin + ptrdiff_t(parent_index)),
                      *(begin + ptrdiff_t(son_index)));

            parent_index = son_index;
            son_index = parent_index * 2 | 1;
            if (son_index >= length) {
                break;
            }

            if (son_index + 1 != length &&
                comparator(*(begin + ptrdiff_t(son_index)),
                           *(begin + ptrdiff_t(son_index + 1)))) {
                ++son_index;
            }
        }
    }
}

template <class Iterator,
          class Comparator =
              std::less<typename std::iterator_traits<Iterator>::value_type>>
void HeapSort(Iterator begin, Iterator end,
              Comparator comparator = Comparator()) {
    size_t length = static_cast<size_t>(end - begin);
    if (length <= 1) {
        return;
    }

    BuildHeap(begin, length, comparator);

    for (size_t heap_sz = length - 1; heap_sz != 1; heap_sz--) {
        std::swap(*begin, *(begin + ptrdiff_t(heap_sz)));

        size_t parent_index = 0;
        size_t son_index = 1;
        if (heap_sz != 2 &&
            comparator(*(begin + ptrdiff_t(son_index)), *(begin + 2))) {
            son_index = 2;
        }

        while (comparator(*(begin + ptrdiff_t(parent_index)),
                          *(begin + ptrdiff_t(son_index)))) {
            std::swap(*(begin + ptrdiff_t(parent_index)),
                      *(begin + ptrdiff_t(son_index)));

            parent_index = son_index;
            son_index = son_index * 2 | 1;
            if (son_index >= heap_sz) {
                break;
            }

            if (son_index + 1 != heap_sz &&
                comparator(*(begin + ptrdiff_t(son_index)),
                           *(begin + ptrdiff_t(son_index + 1)))) {
                ++son_index;
            }
        }
    }

    std::swap(*begin, *(begin + 1));
}

template <class Iterator, class Comparator>
static inline Iterator HSortPartitionImpl(Iterator begin, Iterator end,
                                          Iterator pivot_it, Comparator comp) {
    Iterator l_it = begin;
    while (l_it != end && !comp(*l_it, *pivot_it)) {
        ++l_it;
    }
    if (l_it == end) {
        if (pivot_it != begin) {
            std::swap(*begin, *pivot_it);
        }
        return begin - 1;
    }
    if (begin == pivot_it) {
        pivot_it = l_it;
    } else if (l_it == pivot_it) {
        pivot_it = begin;
    }
    std::swap(*begin, *l_it);
    l_it = begin;

    Iterator r_it = begin;
    do {
        ++r_it;
    } while (r_it != end && comp(*r_it, *pivot_it));
    Iterator last = end;
    --last;
    if (r_it == end) {
        if (begin != end) {
            std::swap(*pivot_it, *last);
        }
        return last;
    }
    if (last == pivot_it) {
        pivot_it = r_it;
    } else if (r_it == pivot_it) {
        pivot_it = last;
    }
    std::swap(*last, *r_it);
    r_it = last;
    while (r_it - l_it >= 2) {
        Iterator next_l_it = l_it;
        ++next_l_it;
        while (!comp(*next_l_it, *pivot_it)) {
            Iterator next_r_it = r_it;
            --next_r_it;
            r_it = next_r_it;
            if (next_l_it == next_r_it) {
                next_l_it = l_it;
                break;
            }
            if (next_r_it == pivot_it) {
                pivot_it = next_l_it;
            } else if (next_l_it == pivot_it) {
                pivot_it = next_r_it;
            }
            std::swap(*next_l_it, *next_r_it);
        }
        l_it = next_l_it;
    }

    Iterator new_pivot_place_it = l_it;
    ++new_pivot_place_it;
    std::swap(*new_pivot_place_it, *pivot_it);
    return l_it;
}

template <size_t Cutoff, class Iterator,
          class Comparator =
              std::less<typename std::iterator_traits<Iterator>::value_type>>
void HybridSort(Iterator begin, Iterator end, Comparator comp = Comparator()) {
    static std::ranlux24 quicksort_random;

    size_t n = static_cast<size_t>(end - begin);
    if (n <= Cutoff) {
        HeapSort(begin, end, comp);
        return;
    }

    size_t pivot_index = quicksort_random() % n;
    Iterator last_l_p = HSortPartitionImpl(
        begin, end, begin + static_cast<std::ptrdiff_t>(pivot_index), comp);
    Iterator pivot_it = last_l_p;
    ++pivot_it;

    HybridSort<Cutoff>(begin, pivot_it, comp);

    Iterator first_not_equal = pivot_it;
    while (first_not_equal != end && *first_not_equal == *pivot_it) {
        ++first_not_equal;
    }
    for (Iterator r = first_not_equal; r != end; ++r) {
        if (*r == *pivot_it) {
            std::swap(*first_not_equal, *r);
            ++first_not_equal;
        }
    }

    HybridSort<Cutoff>(first_not_equal, end, comp);
}
