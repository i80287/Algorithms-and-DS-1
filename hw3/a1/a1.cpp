#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numbers>
#include <random>
#include <vector>

namespace measure_tools {

using f128 = long double;

struct Point {
    f128 x = 0;
    f128 y = 0;
};

inline constexpr std::uint_fast32_t kRndSeed = 29;
inline constexpr f128 kSquareMinCoord = -1;
inline constexpr f128 kSquareMaxCoord = 1;
inline constexpr f128 kCircleRadius = 1;
inline constexpr uint32_t kMinPoints = 100;
inline constexpr uint32_t kMaxPoints = 5000;
inline constexpr uint32_t kPointsStep = 100;

static void GeneratePointsInSquare(std::vector<Point>& points, size_t size,
                                   std::mt19937& rnd) {
    std::uniform_real_distribution<f128> dist(kSquareMinCoord, kSquareMaxCoord);
    points.resize(size);
    for (Point& point : points) {
        point.x = dist(rnd);
        point.y = dist(rnd);
    }
}

static size_t CountPointsInCircle(const std::vector<Point>& points) noexcept {
    size_t count = 0;
    for (const Point& point : points) {
        count += point.x * point.x + point.y * point.y <= kCircleRadius;
    }

    return count;
}

static f128 CalcApproxPi(uint32_t n, std::vector<Point>& points,
                         std::mt19937& rnd) {
    GeneratePointsInSquare(points, n, rnd);
    size_t m = CountPointsInCircle(points);
    return f128(4 * m) / n;
}

static int32_t WriteResultsToFiles(const char pi_value_fname[],
                                   const char perc_diff_fname[]) {
    std::ofstream approx_values_stream(pi_value_fname);
    if (!approx_values_stream) {
        std::cerr << "Was not able to create/open file " << pi_value_fname;
        return EXIT_FAILURE;
    }

    std::ofstream perc_diffs_stream(perc_diff_fname);
    if (!perc_diffs_stream) {
        std::cerr << "Was not able to create/open file " << perc_diff_fname;
        return EXIT_FAILURE;
    }

    std::mt19937 rnd{kRndSeed};
    std::vector<Point> points;
    points.reserve(kMaxPoints);
    for (uint32_t n = kMinPoints; n <= kMaxPoints; n += kPointsStep) {
        f128 approx_pi = CalcApproxPi(n, points, rnd);
        approx_values_stream << n << ' ' << approx_pi << '\n';
        perc_diffs_stream << n << ' ' << std::abs(1 - approx_pi / std::numbers::pi_v<f128>) * 100 << '\n';
    }

    return EXIT_SUCCESS;
}

static int32_t PlotGraph(const char data_fname[], const char title[],
                         const char ylabel[], const char color[],
                         bool plot_pi_line) {
    FILE* gnupipe = popen("gnuplot -persistent", "w");
    if (!gnupipe) {
        std::cerr << "Data presentation error: could not start drawing app";
        return EXIT_FAILURE;
    }

    fprintf(gnupipe,
            "set title \"%s\"\n"
            "set xlabel \"N\"\n"
            "set xtics 200\n"
            "set ylabel \"%s\"\n"
            "plot '%s' title \"%s\" w linespoints lt rgb \"%s\" %s\n",
            title, ylabel, data_fname, title, color,
            plot_pi_line ? ", pi title \"{/Symbol p}\" w lines lt rgb \"blue\""
                         : "");

    // Dont call pclose() on gnupipe!
    return EXIT_SUCCESS;
}

static int32_t PlotGraphs(const char pi_value_fname[],
                          const char rel_diff_fname[]) {
    return PlotGraph(pi_value_fname, "y = {/Symbol p}(N)", "{/Symbol p}(N)",
                     "red", true) |
           PlotGraph(rel_diff_fname,
                     "y = |1 - {/Symbol p}(N) / {/Symbol p}| * 100%",
                     "|1 - {/Symbol p}(N) / {/Symbol p}| * 100%", "dark-green",
                     false);
}

}  // namespace measure_tools

int main() {
    constexpr const char kPiValFname[] = "pi_values_tests.data";
    constexpr const char kPercentageDiffFname[] = "percntg_diff_tests.data";
    int32_t write_res =
        measure_tools::WriteResultsToFiles(kPiValFname, kPercentageDiffFname);
    if (write_res != EXIT_SUCCESS) {
        return write_res;
    }
    return measure_tools::PlotGraphs(kPiValFname, kPercentageDiffFname);
}
