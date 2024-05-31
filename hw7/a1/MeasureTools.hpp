#pragma once

#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <ranges>
#include <type_traits>
#include <utility>

#include "config.hpp"

namespace measuretools {

using TimeType = std::chrono::microseconds;

struct MeasureParams final {
    std::size_t MinArraySize;
    std::size_t MaxArraySize;
    std::size_t ArraySizeStep;

    constexpr std::size_t number_of_iterations() const noexcept {
        return 1 + (MaxArraySize - MinArraySize) / ArraySizeStep;
    }
};

struct TimeMeasureResult final {
    std::size_t subarray_size;
    TimeType time_passed;
};

struct ComparisonsMeasureResult final {
    std::size_t subarray_size;
    std::size_t comparisons;
};

namespace detail {

template <class Function, class Iterator = typename StringsArrayType::iterator>
concept ConsumerFunction = std::invocable<Function, Iterator, Iterator> &&
                           requires(Function fn, Iterator begin, Iterator end) {
                               { std::invoke(fn, begin, end) };
                           };

}  // namespace detail

template <MeasureParams Params, detail::ConsumerFunction Function>
    requires(Params.MinArraySize <= Params.MaxArraySize)
[[nodiscard]] inline auto measure_call_time_over_subranges(const StringsArrayType& strings,
                                                           Function&& consumer_function) {
    namespace chrono = std::chrono;

    assert(Params.MaxArraySize <= strings.size());
    std::array<TimeMeasureResult, Params.number_of_iterations()> measures{};

    const auto size_of_each_string = strings.front().size();
    [[maybe_unused]] const bool all_have_same_size =
        std::ranges::all_of(strings, [=](const UsedStringType& s) constexpr noexcept {
            return s.size() == size_of_each_string;
        });
    assert(all_have_same_size);

    StringsArrayType tmp_storage(strings.size(), UsedStringType(size_of_each_string, '\0'));

    for (std::size_t subarray_size = Params.MinArraySize, i = 0;
         subarray_size <= Params.MaxArraySize; subarray_size += Params.ArraySizeStep, i++) {
        constexpr std::size_t kRepeats = 4;
        TimeType total_time_passed{0};

        for (auto j = kRepeats; j > 0; j--) {
            std::ranges::for_each(
                std::views::zip(strings | std::views::take(subarray_size), tmp_storage),
                [](const std::pair<const UsedStringType&, UsedStringType&>& pair) {
                    std::ranges::copy(pair.first, pair.second.begin());
                });
            const auto start = chrono::high_resolution_clock::now();
            std::invoke(consumer_function, tmp_storage.begin(),
                        tmp_storage.begin() + static_cast<std::ptrdiff_t>(subarray_size));
            const auto end         = chrono::high_resolution_clock::now();
            const auto time_passed = chrono::duration_cast<TimeType>(end - start);
            total_time_passed += time_passed;
        }

        measures[i] = {
            .subarray_size = subarray_size,
            .time_passed   = total_time_passed / kRepeats,
        };
    }

    return measures;
}

template <MeasureParams Params, detail::ConsumerFunction Function>
    requires(Params.MinArraySize <= Params.MaxArraySize)
[[nodiscard]] inline auto measure_comparions_over_subranges(const StringsArrayType& strings,
                                                            Function&& consumer_function) {
    namespace chrono = std::chrono;

    assert(Params.MaxArraySize <= strings.size());
    std::array<ComparisonsMeasureResult, Params.number_of_iterations()> measures{};

    const auto size_of_each_string = strings.front().size();
    [[maybe_unused]] const bool all_have_same_size =
        std::ranges::all_of(strings, [=](const UsedStringType& s) constexpr noexcept {
            return s.size() == size_of_each_string;
        });
    assert(all_have_same_size);

    StringsArrayType tmp_storage(strings.size(), UsedStringType(size_of_each_string, '\0'));

    for (std::size_t subarray_size = Params.MinArraySize, i = 0;
         subarray_size <= Params.MaxArraySize; subarray_size += Params.ArraySizeStep, i++) {
        std::ranges::for_each(
            std::views::zip(strings | std::views::take(subarray_size), tmp_storage),
            [](const std::pair<const UsedStringType&, UsedStringType&>& pair) {
                std::ranges::copy(pair.first, pair.second.begin());
            });
        string_char_comparisons_count = 0;
        std::invoke(consumer_function, tmp_storage.begin(),
                    tmp_storage.begin() + static_cast<std::ptrdiff_t>(subarray_size));
        measures[i] = {
            .subarray_size = subarray_size,
            .comparisons   = string_char_comparisons_count,
        };
    }

    return measures;
}

}  // namespace measuretools
