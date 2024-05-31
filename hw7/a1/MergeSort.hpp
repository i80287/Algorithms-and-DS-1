#pragma once

#include <algorithm>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <utility>

namespace algorithms {
namespace detail {
namespace merge_sort {
namespace {

template <class Iterator, class Comparator>
inline constexpr bool kIsNothrowComparable =
    noexcept(std::declval<Comparator>()(*std::declval<Iterator>(), *std::declval<Iterator>()));

template <class Iterator, class Comparator>
inline constexpr bool kIsNoexceptMergable =
    noexcept(
        std::is_nothrow_move_assignable_v<typename std::iterator_traits<Iterator>::value_type>) &&
    noexcept(std::is_nothrow_move_constructible_v<
             typename std::iterator_traits<Iterator>::value_type>) &&
    kIsNothrowComparable<Iterator, Comparator>;

// clang-format off
template <class Iterator, class Comparator>
constexpr void merge_impl(
    Iterator iter_left_begin,
    Iterator iter_right_begin,
    Iterator iter_right_end,
    Comparator comp,
    std::vector<typename std::iterator_traits<Iterator>::value_type>& buffer) noexcept(kIsNoexceptMergable<Iterator, Comparator>) {
    // clang-format on
    const Iterator original_begin = iter_left_begin;
    const Iterator iter_left_end  = iter_right_begin;
    while (iter_left_begin != iter_left_end && iter_right_begin != iter_right_end) {
        if (comp(*iter_left_begin, *iter_right_begin)) {
            buffer.emplace_back(std::move(*iter_left_begin));
            ++iter_left_begin;
        } else {
            buffer.emplace_back(std::move(*iter_right_begin));
            ++iter_right_begin;
        }
    }

    std::move(iter_left_begin, iter_left_end, std::back_inserter(buffer));
    std::move(iter_right_begin, iter_right_end, std::back_inserter(buffer));
    std::move(buffer.begin(), buffer.end(), original_begin);
    buffer.clear();
}

// clang-format off
template <class Iterator, class Comparator>
constexpr void merge_sort_impl(
    Iterator iter_left_begin,
    Iterator iter_right_end,
    Comparator comp,
    std::vector<typename std::iterator_traits<Iterator>::value_type>& buffer) noexcept(kIsNoexceptMergable<Iterator, Comparator>) {
    // clang-format on
    const auto size = std::distance(iter_left_begin, iter_right_end);
    if (size < 2) {
        return;
    }
    Iterator iter_right_begin = iter_left_begin + size / 2;
    merge_sort_impl(iter_left_begin, iter_right_begin, comp, buffer);
    merge_sort_impl(iter_right_begin, iter_right_end, comp, buffer);
    merge_impl(iter_left_begin, iter_right_begin, iter_right_end, std::move(comp), buffer);
}

// clang-format off
template <class Iterator, class Comparator>
constexpr void merge_sort_with_empty_comparator_impl(
    Iterator iter_left_begin,
    Iterator iter_right_end,
    std::vector<typename std::iterator_traits<Iterator>::value_type>& buffer) noexcept(kIsNoexceptMergable<Iterator, Comparator>) {
    // clang-format on
    const auto size = std::distance(iter_left_begin, iter_right_end);
    if (size < 2) {
        return;
    }
    Iterator iter_right_begin = iter_left_begin + size / 2;
    merge_sort_with_empty_comparator_impl<Iterator, Comparator>(iter_left_begin, iter_right_begin,
                                                                buffer);
    merge_sort_with_empty_comparator_impl<Iterator, Comparator>(iter_right_begin, iter_right_end,
                                                                buffer);
    merge_impl(iter_left_begin, iter_right_begin, iter_right_end, Comparator{}, buffer);
}

}  // namespace
}  // namespace merge_sort
}  // namespace detail

struct MergeSortNiebloid final {
    static constexpr auto kAlgorithmName = niebloid_tools::get_algorithm_name<MergeSortNiebloid>();

    template <std::bidirectional_iterator Iterator, class Comparator = std::ranges::less>
    static constexpr void merge_sort(Iterator begin, Iterator end, Comparator comp = {}) {
        const auto size = static_cast<std::size_t>(std::distance(begin, end));
        std::vector<typename std::iterator_traits<Iterator>::value_type> buffer;
        buffer.reserve(size);
        if (size >= 2) {
            if constexpr (sizeof(Comparator) == 1 && std::is_nothrow_constructible_v<Comparator>) {
                detail::merge_sort::merge_sort_with_empty_comparator_impl<Iterator, Comparator>(
                    begin, end, buffer);
            } else {
                detail::merge_sort::merge_sort_impl(begin, end, std::move(comp), buffer);
            }
        }
    }

    template <StringsRange Range, class Comparator = std::ranges::less>
    static constexpr void merge_sort(Range&& range, Comparator comp = {}) {
        merge_sort(std::ranges::begin(range), std::ranges::end(range), std::move(comp));
    }

    template <std::bidirectional_iterator Iterator, class Comparator = std::ranges::less>
    constexpr void operator()(Iterator begin, Iterator end, Comparator comp = {}) const {
        merge_sort(begin, end, std::move(comp));
    }

    template <StringsRange Range, class Comparator = std::ranges::less>
    constexpr void operator()(Range&& range, Comparator comp = {}) const {
        merge_sort(std::ranges::begin(range), std::ranges::end(range), std::move(comp));
    }
};

inline constexpr MergeSortNiebloid merge_sort{};

}  // namespace algorithms
