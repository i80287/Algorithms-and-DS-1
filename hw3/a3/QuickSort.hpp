#pragma once

#include <cstdint>
#include <functional>
#include <random>

template <class Iterator, class Comparator>
static inline Iterator QSortPartitionImpl(Iterator begin, Iterator end,
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

template <class Iterator,
          class Comparator =
              std::less<typename std::iterator_traits<Iterator>::value_type>>
void QuickSort(Iterator begin, Iterator end, Comparator comp = Comparator()) {
    static std::ranlux24 quicksort_random;

    size_t n = static_cast<size_t>(end - begin);
    if (n <= 1) {
        return;
    }

    size_t pivot_index = quicksort_random() % n;
    Iterator last_l_p = QSortPartitionImpl(
        begin, end, begin + static_cast<std::ptrdiff_t>(pivot_index), comp);
    Iterator pivot_it = last_l_p;
    ++pivot_it;

    QuickSort(begin, pivot_it, comp);

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

    QuickSort(first_not_equal, end, comp);
}
