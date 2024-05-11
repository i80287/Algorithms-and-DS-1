#pragma once

#include <cstdint>
#include <functional>
#include <iterator>
#include <random>
#include <ranges>

namespace algorithms {

namespace detail {

template <class Iterator, class Comparator>
constexpr Iterator partition(Iterator begin, Iterator end, Iterator pivot_it, Comparator comp) {
    // For ADL (more efficient swap may be provided for the given value type)
    using std::swap;

    Iterator l_it = begin;
    while (l_it != end && !comp(*l_it, *pivot_it)) {
        ++l_it;
    }
    if (l_it == end) [[unlikely]] {
        if (pivot_it != begin) {
            swap(*begin, *pivot_it);
        }
        return begin;
    }

    if (begin == pivot_it) {
        pivot_it = l_it;
    } else if (l_it == pivot_it) {
        pivot_it = begin;
    }
    swap(*begin, *l_it);
    l_it = begin;

    Iterator r_it = begin;
    do {
        ++r_it;
    } while (r_it != end && comp(*r_it, *pivot_it));
    Iterator last = end;
    --last;
    if (r_it == end) {
        assert(begin != end);
        swap(*pivot_it, *last);
        return last;
    }
    if (last == pivot_it) {
        pivot_it = r_it;
    } else if (r_it == pivot_it) {
        pivot_it = last;
    }
    swap(*last, *r_it);
    r_it = last;
    while (std::distance(l_it, r_it) >= 2) {
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
            swap(*next_l_it, *next_r_it);
        }
        l_it = next_l_it;
    }

    Iterator new_pivot_place_it = l_it;
    ++new_pivot_place_it;
    swap(*new_pivot_place_it, *pivot_it);
    return new_pivot_place_it;
}

template <class Iterator, class Comparator>
constexpr Iterator select_pivot(Iterator begin, Iterator end, Comparator comp) {
    // length should be >= 2 by the invariant of quick_sort_impl.
    // Thus we can derefer (begin + length - 1) safely.
    const auto length     = std::distance(begin, end);
    const auto half_index = length / 2;
    --end;
    const auto mid_iter = begin + half_index;

    if (comp(*begin, *mid_iter)) {
        if (comp(*mid_iter, *end)) {
            return mid_iter;
        }

        if (comp(*begin, *end)) {
            return end;
        }

        return begin;
    }

    if (comp(*end, *mid_iter)) {
        return mid_iter;
    }

    if (comp(*end, *begin)) {
        return end;
    }

    return begin;
}

template <class Iterator, class Comparator>
constexpr void quick_sort_impl(Iterator begin, Iterator end, Comparator comp) {
    // For ADL (more efficient swap may be provided for the given value type)
    using std::swap;

    const Iterator selected_pivot_iter = ::algorithms::detail::select_pivot(begin, end, comp);
    const Iterator new_pivot_iter =
        ::algorithms::detail::partition(begin, end, selected_pivot_iter, comp);

    if (std::distance(begin, new_pivot_iter) > 1) {
        ::algorithms::detail::quick_sort_impl(begin, new_pivot_iter, comp);
    }

    Iterator first_not_equal = new_pivot_iter;
    while (first_not_equal != end && *first_not_equal == *new_pivot_iter) {
        ++first_not_equal;
    }
    for (Iterator r = first_not_equal; r != end; ++r) {
        if (*r == *new_pivot_iter) {
            swap(*first_not_equal, *r);
            ++first_not_equal;
        }
    }

    if (std::distance(first_not_equal, end) > 1) {
        ::algorithms::detail::quick_sort_impl(first_not_equal, end, comp);
    }
}

}  // namespace detail

template <std::bidirectional_iterator Iterator, class Comparator = std::ranges::less>
constexpr void quick_sort(Iterator begin, Iterator end, Comparator comp = {}) {
    if (std::distance(begin, end) > 1) {
        ::algorithms::detail::quick_sort_impl(begin, end, std::move(comp));
    }
}

template <std::ranges::bidirectional_range Range, class Comparator = std::ranges::less>
constexpr void quick_sort(Range&& range, Comparator comp = {}) {
    quick_sort(std::ranges::begin(range), std::ranges::end(range), std::move(comp));
}

}  // namespace algorithms
