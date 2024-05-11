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

struct MeasureResult final {
    std::size_t subarray_size;
    TimeType time_passed;
};

namespace detail {

template <class Function, class Iterator = StringsArrayType::iterator>
concept ConsumerFunction = std::invocable<Function, Iterator, Iterator> &&
                           requires(Function fn, Iterator begin, Iterator end) {
                               { std::invoke(fn, begin, end) } -> std::same_as<void>;
                           };

}  // namespace detail

template <std::size_t MinArraySize, std::size_t MaxArraySize, std::size_t ArraySizeStep,
          detail::ConsumerFunction Function>
    requires(MinArraySize <= MaxArraySize)
[[nodiscard]] inline auto measure_call_time_over_subranges(const StringsArrayType& strings,
                                                           Function&& consumer_function) {
    namespace chrono = std::chrono;

    assert(MaxArraySize <= strings.size());
    constexpr auto kNumberOfIterations = 1 + (MaxArraySize - MinArraySize) / ArraySizeStep;
    std::array<MeasureResult, kNumberOfIterations> measures{};

    const auto size_of_each_string = strings.front().size();
    [[maybe_unused]] const bool all_have_same_size =
        std::ranges::all_of(strings, [=](const std::string& s) constexpr noexcept {
            return s.size() == size_of_each_string;
        });
    assert(all_have_same_size);

    StringsArrayType tmp_storage(strings.size(), std::string(size_of_each_string, '\0'));

    using difference_type = std::iterator_traits<StringsArrayType::iterator>::difference_type;
    for (std::size_t subarray_size = MinArraySize, i = 0; subarray_size <= MaxArraySize;
         subarray_size += ArraySizeStep, i++) {
        std::ranges::for_each(
            std::views::zip(strings | std::views::take(subarray_size), tmp_storage),
            [](const std::pair<const std::string&, std::string&>& pair) {
                std::ranges::copy(pair.first, pair.second.begin());
            });

        const auto start = chrono::high_resolution_clock::now();
        std::invoke(consumer_function, tmp_storage.begin(),
                    tmp_storage.begin() + static_cast<difference_type>(subarray_size));
        const auto end         = chrono::high_resolution_clock::now();
        const auto time_passed = chrono::duration_cast<TimeType>(end - start);
        measures[i]            = {
                       .subarray_size = subarray_size,
                       .time_passed   = time_passed,
        };
    }

    return measures;
}

}  // namespace measuretools
