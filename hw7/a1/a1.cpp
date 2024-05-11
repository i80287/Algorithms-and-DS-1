#include <cassert>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>

#include "MeasureTools.hpp"
#include "QuickSort.hpp"
#include "StringGenerator.hpp"

inline constexpr std::uint_fast32_t kStringGenSeed = 367218;
inline constexpr std::size_t kMinStringSize        = 10;
inline constexpr std::size_t kMaxStringSize        = 200;

inline constexpr std::size_t kMinArraySize  = 1000;
inline constexpr std::size_t kMaxArraySize  = 30000;
inline constexpr std::size_t kArraySizeStep = 1000;

static void check_sorting_algorithms() {
    auto reversed_strings =
        stringgen::StringGenerator<kMaxArraySize, 20>{kStringGenSeed}.add_shuffled_strings().get();

    std::ranges::sort(reversed_strings, std::ranges::greater{});
    algorithms::quick_sort(reversed_strings);
    assert(std::ranges::is_sorted(reversed_strings));

    std::ranges::sort(reversed_strings, std::ranges::greater{});
    algorithms::quick_sort(reversed_strings);
    assert(std::ranges::is_sorted(reversed_strings));
}

int main() {
    check_sorting_algorithms();

    const auto [shuffled_strings, rev_sorted_strings, almost_sorted_strings] =
        stringgen::StringGenerator<kMaxArraySize, kMaxStringSize>{kStringGenSeed}
            .add_shuffled_strings()
            .add_reversed_sorted_strings()
            .add_almost_sorted_strings()
            .get();

    using namespace measuretools;
    auto measure_results_2 =
        measuretools::measure_call_time_over_subranges<kMinArraySize, kMaxArraySize, kArraySizeStep>(
            shuffled_strings, [](StringsArrayType::iterator begin, StringsArrayType::iterator end) {
                std::ranges::sort(begin, end);
            });
    std::cout << std::left;
    for (auto [size, time] : measure_results_2) {
        std::cout << std::setw(10) << size << " | " << std::setw(10) << time << '\n';
    }
    auto measure_results_1 =
        measuretools::measure_call_time_over_subranges<kMinArraySize, kMaxArraySize, kArraySizeStep>(
            shuffled_strings, [](StringsArrayType::iterator begin, StringsArrayType::iterator end) {
                algorithms::quick_sort(begin, end);
                assert(std::ranges::is_sorted(begin, end));
            });
    std::cout << std::left;
    for (auto [size, time] : measure_results_1) {
        std::cout << std::setw(10) << size << " | " << std::setw(10) << time << '\n';
    }

    return 0;
}
