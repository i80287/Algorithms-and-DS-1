#include <cassert>
#if __cplusplus >= 202002L
#include <concepts>
#endif
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "HybridSort.hpp"
#include "MergeSort.hpp"

using std::size_t;
using std::uint32_t;
using std::vector;
using T = uint32_t;

inline constexpr T kMinValue = 0;
inline constexpr T kMaxValue = 3000;

inline constexpr size_t kMinN = 500;
inline constexpr size_t kMaxN = 20000;
inline constexpr size_t kStepN = 100;

inline constexpr size_t kRandomSwaps = kMaxN / 1000;

inline constexpr std::uint_fast32_t kRndSeed = 29;

inline constexpr size_t kEachMeasureLimit = 16;

inline constexpr size_t kFirstCutoff = 5;
inline constexpr size_t kSecondCutoff = 10;
inline constexpr size_t kThirdCutoff = 20;
inline constexpr size_t kFourthCutoff = 50;

inline constexpr std::string_view kRandomDataFileSuffix = "_rnd_data.txt";
inline constexpr std::string_view kReversedDataFileSuffix = "_rev_data.txt";
inline constexpr std::string_view kAlmostSortedDataFileSuffix = "_sorted_data.txt";

namespace measure_tools {

struct Measurement {
    uint64_t time_ = 0;
    size_t n_ = 0;

    constexpr Measurement(uint64_t t, uint64_t n) : time_(t), n_(n) {}
};

struct Result {
    vector<Measurement> rand_vec_res;
    vector<Measurement> rev_sorted_vec_res;
    vector<Measurement> almost_sorted_vec_res;
};

}  // namespace measure_tools

namespace data_tools {

void FillVector(vector<T>& rand_vec, std::mt19937_64& rnd) {
    std::uniform_int_distribution<T> dist(kMinValue, kMaxValue);
    rand_vec.resize(kMaxN);
    std::generate(rand_vec.begin(), rand_vec.end(),
                  [&dist, &rnd] { return dist(rnd); });
}

void SwapRandElements(vector<T>& rand_vec, size_t swaps) {
    std::ranlux24 rnd{kRndSeed};
    for (size_t limit = swaps; limit != 0; limit--) {
        size_t l = rnd() % rand_vec.size();
        size_t r = rnd() % rand_vec.size();
        std::swap(rand_vec[l], rand_vec[r]);
    }
}

void FillVectors(vector<T>& rand_vec, vector<T>& rev_sorted_vec,
                 vector<T>& almost_sorted_vec) {
    std::mt19937_64 rnd{kRndSeed};
    FillVector(rand_vec, rnd);
    FillVector(rev_sorted_vec, rnd);
    FillVector(almost_sorted_vec, rnd);
    assert(rev_sorted_vec.size() == kMaxN);
    std::sort(rev_sorted_vec.begin(), rev_sorted_vec.end(), std::greater<T>{});
    std::sort(almost_sorted_vec.begin(), almost_sorted_vec.end());
    SwapRandElements(almost_sorted_vec, kRandomSwaps);
}

int32_t WriteDataToFile(const vector<measure_tools::Measurement>& data,
                        std::string_view fname) {
    std::ofstream fs(fname.data());
    if (!fs) {
        std::cerr << "Was not able to create/open file " << fname << '\n';
        return EXIT_FAILURE;
    }

    for (auto [test_time, test_n] : data) {
        fs << test_n << ' ' << test_time << '\n';
    }

    return fs.fail() ? EXIT_FAILURE : EXIT_SUCCESS;
}

int32_t WriteDataToFiles(const measure_tools::Result& data,
                         std::string_view filename_prefix) {
    int32_t res = EXIT_SUCCESS;
    std::string fname(filename_prefix);

    fname += kRandomDataFileSuffix;
    res |= WriteDataToFile(data.rand_vec_res, fname);

    fname.resize(filename_prefix.size());

    fname += kReversedDataFileSuffix;
    res |= WriteDataToFile(data.rev_sorted_vec_res, fname);

    fname.resize(filename_prefix.size());

    fname += kAlmostSortedDataFileSuffix;
    res |= WriteDataToFile(data.almost_sorted_vec_res, fname);
    return res;
}

}  // namespace data_tools

namespace measure_tools {

namespace chrono = std::chrono;

template <class Iterator>
static void CheckSorted(Iterator begin, Iterator end) {
    Iterator it1 = begin;
    Iterator it2 = begin;
    ++it2;
    if (it1 == end) {
        return;
    }
    for (; it2 != end; ++it1, ++it2) {
        assert(!(*it1 > *it2));
    }
}

template <class Callable, class... Args>
#if __cplusplus >= 202002L
    requires std::invocable<Callable, Args...>
#endif
static uint64_t MeasureCall(Callable f, Args... args) {
    auto time_start = chrono::high_resolution_clock::now();
    f(std::forward<Args>(args)...);
    auto time_end = chrono::high_resolution_clock::now();
    return static_cast<uint64_t>(
        chrono::duration_cast<chrono::microseconds>(time_end - time_start)
            .count());
}

template <class SortingAlgo>
vector<Measurement> MeasureSortingAlgorithm(SortingAlgo f,
                                            const vector<T>& vec) {
    vector<T> buffer;
    buffer.reserve(kMaxN);
    vector<Measurement> measurements;
    constexpr size_t kTotalMeasurements = (kMaxN - kMinN) / kStepN + 1;
    measurements.reserve(kTotalMeasurements);
    for (size_t n = kMinN; n <= kMaxN; n += kStepN) {
        uint64_t time_for_n = 0;
        for (size_t i = kEachMeasureLimit; i != 0; i--) {
            buffer = vec;
            time_for_n +=
                MeasureCall(f, buffer.begin(),
                            buffer.begin() + static_cast<std::ptrdiff_t>(n));

            // Just prevent compiler from optimizing away sorting algorithm call.
            CheckSorted(buffer.begin(),
                        buffer.begin() + static_cast<std::ptrdiff_t>(n));
        }

        measurements.emplace_back(time_for_n / kEachMeasureLimit, n);
    }

    assert(measurements.size() == kTotalMeasurements);
    return measurements;
}

template <class SortingAlgo>
inline Result MeasureSortingAlgorithm(SortingAlgo f, const vector<T>& rand_vec,
                                      const vector<T>& rev_sorted_vec,
                                      const vector<T>& almost_sorted_vec) {
    return Result{measure_tools::MeasureSortingAlgorithm(f, rand_vec),
                  measure_tools::MeasureSortingAlgorithm(f, rev_sorted_vec),
                  measure_tools::MeasureSortingAlgorithm(f, almost_sorted_vec)};
}

inline std::vector<Result> MeasureSortingAlgorithms() {
    vector<T> rand_vec;
    vector<T> rev_sorted_vec;
    vector<T> almost_sorted_vec;
    data_tools::FillVectors(rand_vec, rev_sorted_vec, almost_sorted_vec);

    using Iter = vector<T>::iterator;
    return {MeasureSortingAlgorithm(MergeSort<Iter>, rand_vec, rev_sorted_vec,
                                    almost_sorted_vec),
            MeasureSortingAlgorithm(HybridSort<kFirstCutoff, Iter>, rand_vec,
                                    rev_sorted_vec, almost_sorted_vec),
            MeasureSortingAlgorithm(HybridSort<kSecondCutoff, Iter>, rand_vec,
                                    rev_sorted_vec, almost_sorted_vec),
            MeasureSortingAlgorithm(HybridSort<kThirdCutoff, Iter>, rand_vec,
                                    rev_sorted_vec, almost_sorted_vec),
            MeasureSortingAlgorithm(HybridSort<kFourthCutoff, Iter>, rand_vec,
                                    rev_sorted_vec, almost_sorted_vec)};
}

}  // namespace measure_tools

namespace graphics_tools {

using measure_tools::Result;

int32_t PlotData(const char filename[], const char title[],
                 const char color[]) {
    FILE* gnupipe = popen("gnuplot -persistent", "w");
    if (!gnupipe) {
        std::cerr << "Data presentation error: could not start drawing app";
        return EXIT_FAILURE;
    }

    fprintf(gnupipe,
            "set title \"%s\"\n"
            "set xlabel \"N\"\n"
            // "set xtics 200\n"
            "set ylabel \"time in milliseconds\"\n"
            "plot '%s' title \"%s\" w linespoints lt rgb \"%s\"\n",
            title, filename, title, color);

    // Dont call pclose() on gnupipe!
    return EXIT_SUCCESS;
}

int32_t PlotAllData(std::string_view filename_prefix,
                    std::string_view title_prefix, std::string_view color) {
    int32_t res = EXIT_SUCCESS;
    std::string filename(filename_prefix);
    std::string title(title_prefix);

    filename += kRandomDataFileSuffix;
    title += ", random array";
    res |= PlotData(filename.c_str(), title.c_str(), color.data());

    filename.resize(filename_prefix.size());
    title.resize(title_prefix.size());

    filename += kReversedDataFileSuffix;
    title += ", reversed sorted array";
    res |= PlotData(filename.c_str(), title.c_str(), color.data());

    filename.resize(filename_prefix.size());
    title.resize(title_prefix.size());

    filename += kAlmostSortedDataFileSuffix;
    title += ", almost sorted array";
    res |= PlotData(filename.c_str(), title.c_str(), color.data());
    return res;
}

}  // namespace graphics_tools

int main() {
    auto results = measure_tools::MeasureSortingAlgorithms();
    std::string_view kArgs[][3] = {
        {"merge_sort", "Merge sort without cutoff", "red"},
        {"hybrid_sort_5", "Hybrid Sort with cutoff 5", "dark-green"},
        {"hybrid_sort_10", "Hybrid Sort with cutoff 10", "red"},
        {"hybrid_sort_20", "Hybrid Sort with cutoff 20", "blue"},
        {"hybrid_sort_50", "Hybrid Sort with cutoff 50", "#0xFFAABB"},
    };
    assert(results.size() == std::size(kArgs));

    int32_t res = EXIT_SUCCESS;
    size_t i = 0;
    for (auto [prefix_name, title_prefix, color] : kArgs) {
        res |= data_tools::WriteDataToFiles(results[i], prefix_name);
        res |= graphics_tools::PlotAllData(prefix_name, title_prefix, color);
        i++;
    }
    return res;
}
