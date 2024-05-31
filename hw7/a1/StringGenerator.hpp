#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <random>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "config.hpp"

namespace gen_tools {
namespace detail {
namespace {

using namespace std::string_view_literals;

inline constexpr std::string_view kAlphabet =
    "!#%&()*-0123456789:;@ABCDEFGHIJKLMNOPQRSTUVWXYZ^"
    "abcdefghijklmnopqrstuvwxyz"sv;
inline constexpr std::size_t kDefaultSwapsRatio = 16;

inline constexpr std::size_t kDynamicExtent = static_cast<std::size_t>(-1);

}  // namespace
}  // namespace detail

template <std::size_t NumberOfStrings, std::size_t OneStringSize>
inline void append_shuffled_strings(StringsArrayType& arr, std::mt19937& rnd) {
    arr.reserve(NumberOfStrings);
    for (auto iter = NumberOfStrings; iter > 0; iter--) {
        std::ranges::generate(arr.emplace_back(OneStringSize, '\0'), [&rnd = rnd]() noexcept {
            return ::gen_tools::detail::kAlphabet[std::uniform_int_distribution<std::size_t>(
                0, ::gen_tools::detail::kAlphabet.size() - 1)(rnd)];
        });
    }
}

template <std::size_t NumberOfStrings = ::gen_tools::detail::kDynamicExtent,
          std::size_t SwapsRation     = ::gen_tools::detail::kDefaultSwapsRatio>
inline void swap_some_strings_inplace(StringsArrayType& arr, std::mt19937& rnd) {
    std::size_t size;
    if constexpr (NumberOfStrings == ::gen_tools::detail::kDynamicExtent) {
        size = arr.size();
    } else {
        assert(NumberOfStrings <= arr.size());
        size = NumberOfStrings;
    }
    const auto swaps = size / SwapsRation;
    for (auto i = swaps; i > 0; i--) {
        auto l = rnd() % size;
        auto r = rnd() % size;
        std::swap(arr[l], arr[r]);
    }
}

namespace detail {
namespace {

enum class StringsArrayTag {
    kShuffled,
    kSorted,
    kReversedSorted,
    kAlmostSorted,
};

template <std::size_t NumberOfStrings, std::size_t OneStringSize, StringsArrayTag... ArrayTypes>
class [[nodiscard]] StringGeneratorContainerImpl final {
    static constexpr bool kReturnStdArray = sizeof...(ArrayTypes) >= 2;

public:
    using GetReturnType =
        std::conditional_t<kReturnStdArray, std::array<StringsArrayType, sizeof...(ArrayTypes)>,
                           StringsArrayType>;

    constexpr StringGeneratorContainerImpl(std::mt19937& rnd) noexcept : rnd_(rnd) {}
    StringGeneratorContainerImpl(const StringGeneratorContainerImpl&)                     = delete;
    StringGeneratorContainerImpl(StringGeneratorContainerImpl&&)                          = delete;
    StringGeneratorContainerImpl& operator=(const StringGeneratorContainerImpl&) noexcept = delete;
    StringGeneratorContainerImpl& operator=(StringGeneratorContainerImpl&&) noexcept      = delete;

    auto add_shuffled_strings() && {
        return StringGeneratorContainerImpl<NumberOfStrings, OneStringSize, ArrayTypes...,
                                            StringsArrayTag::kShuffled>(rnd_);
    }
    auto add_sorted_strings() && {
        return StringGeneratorContainerImpl<NumberOfStrings, OneStringSize, ArrayTypes...,
                                            StringsArrayTag::kSorted>(rnd_);
    }
    auto add_reversed_sorted_strings() && {
        return StringGeneratorContainerImpl<NumberOfStrings, OneStringSize, ArrayTypes...,
                                            StringsArrayTag::kReversedSorted>(rnd_);
    }
    auto add_almost_sorted_strings() && {
        return StringGeneratorContainerImpl<NumberOfStrings, OneStringSize, ArrayTypes...,
                                            StringsArrayTag::kAlmostSorted>(rnd_);
    }
    [[nodiscard]] constexpr auto get() && noexcept {
        static_assert(sizeof...(ArrayTypes) >= 1);
        GetReturnType res;
        if constexpr (kReturnStdArray) {
            fill_array<0, ArrayTypes...>(res);
        } else {
            static_assert(sizeof...(ArrayTypes) == 1);
            fill_impl<ArrayTypes...>(res);
        }
        return res;
    }

private:
    template <StringsArrayTag FillStrategy>
    void fill_impl(StringsArrayType& strings_array) {
        append_shuffled_strings<NumberOfStrings, OneStringSize>(strings_array, rnd_);
        if constexpr (FillStrategy == StringsArrayTag::kShuffled) {
        } else if constexpr (FillStrategy == StringsArrayTag::kSorted) {
            std::ranges::sort(strings_array);
        } else if constexpr (FillStrategy == StringsArrayTag::kReversedSorted) {
            std::ranges::sort(strings_array, std::ranges::greater{});
        } else {
            static_assert(FillStrategy == StringsArrayTag::kAlmostSorted);
            std::ranges::sort(strings_array);
            swap_some_strings_inplace(strings_array, rnd_);
        }
    }

    template <std::size_t FillIndex, StringsArrayTag FillStrategy,
              StringsArrayTag... NextFillStrategies>
    void fill_array(GetReturnType& arr) {
        fill_impl<FillStrategy>(std::get<FillIndex>(arr));
        if constexpr (sizeof...(NextFillStrategies) > 0) {
            fill_array<FillIndex + 1, NextFillStrategies...>(arr);
        }
    }

    std::mt19937& rnd_;
};

template <std::size_t NumberOfStrings, std::size_t OneStringSize>
    requires(NumberOfStrings > 0 && OneStringSize > 0)
class StringGeneratorImpl final {
    using Container = StringGeneratorContainerImpl<NumberOfStrings, OneStringSize>;

public:
    using RandomGenerator = std::mt19937;

    StringGeneratorImpl() noexcept = default;
    StringGeneratorImpl(std::uint_fast32_t seed) noexcept : rnd_(seed) {}
    StringGeneratorImpl(const StringGeneratorImpl&)           = delete;
    StringGeneratorImpl(StringGeneratorImpl&&)                = delete;
    StringGeneratorImpl operator=(const StringGeneratorImpl&) = delete;
    StringGeneratorImpl operator=(StringGeneratorImpl&&)      = delete;

    auto add_shuffled_strings() {
        return Container(rnd_).add_shuffled_strings();
    }
    auto add_sorted_strings() {
        return Container(rnd_).add_sorted_strings();
    }
    auto add_reversed_sorted_strings() {
        return Container(rnd_).add_reversed_sorted_strings();
    }
    auto add_almost_sorted_strings() {
        return Container(rnd_).add_almost_sorted_strings();
    }
    RandomGenerator& get_generator() {
        return rnd_;
    }

private:
    RandomGenerator rnd_;
};

}  // namespace
};  // namespace detail

template <std::size_t NumberOfStrings, std::size_t OneStringSize>
using StringGenerator = detail::StringGeneratorImpl<NumberOfStrings, OneStringSize>;

};  // namespace gen_tools
