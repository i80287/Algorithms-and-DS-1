#pragma once

#include <concepts>
#include <iterator>
#include <ranges>
#include <string>
#include <type_traits>
#include <vector>

#include "NiebloidTools.hpp"

inline std::size_t string_char_comparisons_count = 0;

struct StringProxyCounter : public std::string {
    using Base = std::string;
    using Base::Base;

    [[nodiscard]] constexpr std::strong_ordering operator<=>(
        const StringProxyCounter& other) const noexcept {
        string_char_comparisons_count += std::min(size(), other.size());
        const std::string& s1 = *this;
        const std::string& s2 = other;
        return s1 <=> s2;
    }
};

using UsedStringType = std::string;
using StringsArrayType = std::vector<UsedStringType>;

inline constexpr bool kIsMeasuringTime = !std::is_same_v<UsedStringType, StringProxyCounter>;

template <class Iterator>
concept StringIterator =
    std::random_access_iterator<Iterator> &&
    (std::same_as<
         typename std::iterator_traits<Iterator>::value_type,
         std::basic_string<typename std::iterator_traits<Iterator>::value_type::value_type> > ||
     std::same_as<typename std::iterator_traits<Iterator>::value_type, StringProxyCounter>);

template <class Range>
concept StringsRange = std::ranges::bidirectional_range<Range> &&
                       StringIterator<decltype(std::ranges::begin(std::declval<Range>()))>;

template <StringIterator Iterator>
using IteratorStringType = typename std::iterator_traits<Iterator>::value_type;

template <StringIterator Iterator>
using StringIteratorUChar = std::make_unsigned_t<typename IteratorStringType<Iterator>::value_type>;
