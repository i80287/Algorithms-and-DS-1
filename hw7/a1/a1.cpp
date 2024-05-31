#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>

#include "MeasureTools.hpp"
#include "MergeSort.hpp"
#include "MSDRadixSort.hpp"
#include "QuickSort.hpp"
#include "StringGenerator.hpp"
#include "StringMergeSort.hpp"
#include "StringQuickSort.hpp"

inline constexpr std::uint_fast32_t kStringGenSeed = 367218;
inline constexpr std::size_t kMinStringSize        = 10;
inline constexpr std::size_t kMaxStringSize        = 200;

inline constexpr std::size_t kMinArraySize  = 1000;
inline constexpr std::size_t kMaxArraySize  = 60000;
inline constexpr std::size_t kArraySizeStep = 1000;

template <class MeasureResultType, std::size_t N>
void save_measure_results(std::string_view algo_name, std::string_view array_type,
                          const std::array<MeasureResultType, N>& measure_results) {
    std::string fname(std::string_view(kIsMeasuringTime ? "time_" : "comp_"));
    fname += std::string(algo_name);
    fname += '_';
    fname += array_type;
    fname += ".csv";
    for (char& c : fname) {
        switch (c) {
            case '<':
                c = '[';
                break;
            case '>':
                c = ']';
                break;
        }
    }
    std::ofstream fout(fname);
    if (!fout) {
        std::cout << "bad " << fname << '\n';
        return;
    }

    if constexpr (kIsMeasuringTime) {
        fout << "N,Time\n";
        for (auto [size, time] : measure_results) {
            fout << size << ',' << time.count() << '\n';
        }
    } else {
        static_assert(std::is_same_v<MeasureResultType, measuretools::ComparisonsMeasureResult>);
        fout << "N,Comparisons\n";
        for (auto [size, comparisons] : measure_results) {
            fout << size << ',' << comparisons << '\n';
        }
    }
}

template <class = void> // In order to instantiate lazily
void measure_time_over(const StringsArrayType& strings, std::string_view array_type) {
    using namespace measuretools;
    constexpr MeasureParams kParams = {
        .MinArraySize  = kMinArraySize,
        .MaxArraySize  = kMaxArraySize,
        .ArraySizeStep = kArraySizeStep,
    };

    auto measure_and_save = [&](auto sorting_algo) {
        save_measure_results(
            sorting_algo.kAlgorithmName, array_type,
            measuretools::measure_call_time_over_subranges<kParams>(strings, sorting_algo));
    };
    measure_and_save(algorithms::merge_sort);
    measure_and_save(algorithms::quick_sort);
    measure_and_save(algorithms::string_merge_sort);
    measure_and_save(algorithms::string_quick_sort);
    measure_and_save(algorithms::msd_radix_sort<true>);
    measure_and_save(algorithms::msd_radix_sort<false>);
}

template <class = void> // In order to instantiate lazily
void measure_comparisons_over(const StringsArrayType& strings, std::string_view array_type) {
    using namespace measuretools;
    constexpr MeasureParams kParams = {
        .MinArraySize  = kMinArraySize,
        .MaxArraySize  = kMaxArraySize,
        .ArraySizeStep = kArraySizeStep,
    };

    auto measure_and_save = [&](auto sorting_algo) {
        save_measure_results(
            sorting_algo.kAlgorithmName, array_type,
            measuretools::measure_comparions_over_subranges<kParams>(strings, sorting_algo));
    };
    measure_and_save(algorithms::merge_sort);
    measure_and_save(algorithms::quick_sort);
    measure_and_save(algorithms::string_merge_sort);
    measure_and_save(algorithms::string_quick_sort);
    measure_and_save(algorithms::msd_radix_sort<true>);
    measure_and_save(algorithms::msd_radix_sort<false>);
}

int main() {
    const auto [shuffled_strings, rev_sorted_strings, almost_sorted_strings] =
        gen_tools::StringGenerator<kMaxArraySize, kMaxStringSize>{kStringGenSeed}
            .add_shuffled_strings()
            .add_reversed_sorted_strings()
            .add_almost_sorted_strings()
            .get();
    if constexpr (kIsMeasuringTime) {
        std::cout << "Starting to measure time...\n";
        measure_time_over(shuffled_strings, "shuffled_strings");
        measure_time_over(rev_sorted_strings, "reversed_sorted_strings");
        measure_time_over(almost_sorted_strings, "almost_sorted_strings");
        std::cout << "Ended measuring time\n";
    } else {
        std::cout << "Starting to measure comparisons count...\n";
        measure_comparisons_over(shuffled_strings, "shuffled_strings");
        measure_comparisons_over(rev_sorted_strings, "reversed_sorted_strings");
        measure_comparisons_over(almost_sorted_strings, "almost_sorted_strings");
        std::cout << "Ended measuring comparisons count\n";
    }
}
