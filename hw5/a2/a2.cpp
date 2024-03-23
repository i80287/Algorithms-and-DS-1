#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <vector>

#include "UnorderedSet.hpp"

// namespace measure_tools {

// struct Measurement {
//     size_t collisions_count = 0;
//     size_t clusters_count   = 0;
//     size_t n                = 0;

//     constexpr Measurement(size_t a_collisions_count, size_t a_clusters_count,
//                           size_t a_n) noexcept
//         : collisions_count(a_collisions_count),
//           clusters_count(a_clusters_count),
//           n(a_n) {}
// };

// struct HashTableOperationsResult {
//     std::vector<Measurement> insert_tests;
//     std::vector<Measurement> find_tests;
//     std::vector<Measurement> delete_tests;
// };

// struct Result {
//     HashTableOperationsResult subsequent_operations_tests;
//     HashTableOperationsResult random_operations_tests;
// };

// namespace impl {

// using std::int32_t;
// using std::size_t;
// using std::uint32_t;
// using std::uint_fast32_t;

// inline constexpr size_t kTestsPerGroup     = 2e5;
// inline constexpr uint_fast32_t kRandomSeed = 5872;

// static HashTableOperationsResult measure_subsequent_operations() {
//     std::vector<Measurement> subsequent_insert_tests;
//     std::vector<Measurement> subsequent_find_tests;
//     std::vector<Measurement> subsequent_delete_tests;
//     subsequent_insert_tests.reserve(kTestsPerGroup);
//     subsequent_find_tests.reserve(kTestsPerGroup);
//     subsequent_delete_tests.reserve(kTestsPerGroup);

//     UnorderedSet<int32_t> htable;
//     constexpr int32_t kInitialKey = -int32_t(kTestsPerGroup / 2);
//     int32_t key                   = kInitialKey;
//     for (size_t iter = kTestsPerGroup; iter > 0; iter--) {
//         htable.insert(key);
//         key++;
//         subsequent_insert_tests.emplace_back(htable.last_collisions_count(),
//                                              htable.clusters_count(), htable.size());
//     }
//     key = kInitialKey;
//     for (size_t iter = kTestsPerGroup; iter > 0; iter--) {
//         bool res = htable.contains(key);
//         assert(res);
//         key++;
//         subsequent_insert_tests.emplace_back(htable.last_collisions_count(),
//                                              htable.clusters_count(), htable.size());
//     }
//     key = kInitialKey;
//     for (size_t iter = kTestsPerGroup; iter > 0; iter--) {
//         htable.erase(key);
//         key++;
//         subsequent_insert_tests.emplace_back(htable.last_collisions_count(),
//                                              htable.clusters_count(), htable.size());
//     }
//     assert(htable.empty());
//     return {
//         .insert_tests = std::move(subsequent_insert_tests),
//         .find_tests   = std::move(subsequent_find_tests),
//         .delete_tests = std::move(subsequent_delete_tests),
//     };
// }

// static HashTableOperationsResult measure_random_operations() {
//     std::vector<Measurement> random_insert_tests;
//     std::vector<Measurement> random_find_tests;
//     std::vector<Measurement> random_delete_tests;
//     std::vector<int32_t> random_keys(kTestsPerGroup);
//     random_insert_tests.reserve(kTestsPerGroup);
//     random_find_tests.reserve(kTestsPerGroup);
//     random_delete_tests.reserve(kTestsPerGroup);

//     UnorderedSet<int32_t> htable;
//     {
//         std::mt19937 rnd{kRandomSeed};
//         for (size_t iter = 0; iter < kTestsPerGroup; iter++) {
//             int32_t key       = int32_t(rnd());
//             random_keys[iter] = key;
//             htable.insert(key);
//             key++;
//             random_insert_tests.emplace_back(htable.last_collisions_count(),
//                                              htable.clusters_count(), htable.size());
//         }
//     }
//     for (size_t iter = 0; iter < kTestsPerGroup; iter++) {
//         int32_t key = random_keys[iter];
//         bool res    = htable.contains(key);
//         assert(res);
//         random_insert_tests.emplace_back(htable.last_collisions_count(),
//                                          htable.clusters_count(), htable.size());
//     }
//     for (size_t iter = 0; iter < kTestsPerGroup; iter++) {
//         int32_t key = random_keys[iter];
//         htable.erase(key);
//         random_insert_tests.emplace_back(htable.last_collisions_count(),
//                                          htable.clusters_count(), htable.size());
//     }
//     return {
//         .insert_tests = std::move(random_insert_tests),
//         .find_tests   = std::move(random_find_tests),
//         .delete_tests = std::move(random_delete_tests),
//     };
// }

// }  // namespace impl

// Result measure_hashtable_metrics() {
//     return {
//         .subsequent_operations_tests = std::move(impl::measure_subsequent_operations()),
//         .random_operations_tests     = std::move(impl::measure_random_operations()),
//     };
// }

// }  // namespace measure_tools

// namespace data_tools {

// namespace filenames {

// namespace subsequent_ops {

// namespace collisions {
// inline constexpr std::string_view kInsertOpsFilename = "subseq_inserts_collisions.txt";
// inline constexpr std::string_view kFindOpsFilename   = "subseq_finds_collisions.txt";
// inline constexpr std::string_view kDeleteOpsFilename = "subseq_deletes_collisions.txt";
// }  // namespace collisions

// namespace clusters {
// inline constexpr std::string_view kInsertOpsFilename = "subseq_inserts_clusters.txt";
// inline constexpr std::string_view kFindOpsFilename   = "subseq_finds_clusters.txt";
// inline constexpr std::string_view kDeleteOpsFilename = "subseq_deletes_clusters.txt";
// }  // namespace clusters

// }  // namespace subsequent_ops

// namespace random_ops {

// namespace collisions {
// inline constexpr std::string_view kInsertOpsFilename = "random_inserts_collisions.txt";
// inline constexpr std::string_view kFindOpsFilename   = "random_finds_collisions.txt";
// inline constexpr std::string_view kDeleteOpsFilename = "random_deletes_collisions.txt";
// }  // namespace collisions

// namespace clusters {
// inline constexpr std::string_view kInsertOpsFilename = "random_inserts_clusters.txt";
// inline constexpr std::string_view kFindOpsFilename   = "random_finds_clusters.txt";
// inline constexpr std::string_view kDeleteOpsFilename = "random_deletes_clusters.txt";
// }  // namespace clusters

// }  // namespace random_ops

// }  // namespace filenames

// namespace impl {

// int32_t write_measuremets_to_files(const std::vector<measure_tools::Measurement>& data,
//                                    std::string_view collisions_fname,
//                                    std::string_view clusters_fname) {
//     std::ofstream colls_fs(collisions_fname.data());
//     if (!colls_fs) {
//         std::cerr << "Was not able to create/open file " << collisions_fname << '\n';
//         return EXIT_FAILURE;
//     }
//     std::ofstream clusters_fs(clusters_fname.data());
//     if (!clusters_fs) {
//         std::cerr << "Was not able to create/open file " << clusters_fname << '\n';
//         return EXIT_FAILURE;
//     }

//     for (auto [collisions_count, clusters_count, test_n] : data) {
//         colls_fs << test_n << ' ' << collisions_count << '\n';
//         clusters_fs << test_n << ' ' << clusters_count << '\n';
//     }

//     return colls_fs.fail() || clusters_fs.fail() ? EXIT_FAILURE : EXIT_SUCCESS;
// }

// }  // namespace impl

// int32_t write_measuremets_to_files(const measure_tools::Result& data) {
//     int32_t res = EXIT_SUCCESS;

//     res |= impl::write_measuremets_to_files(
//         data.subsequent_operations_tests.insert_tests,
//         filenames::subsequent_ops::collisions::kInsertOpsFilename,
//         filenames::subsequent_ops::clusters::kInsertOpsFilename);

//     res |= impl::write_measuremets_to_files(
//         data.subsequent_operations_tests.find_tests,
//         filenames::subsequent_ops::collisions::kFindOpsFilename,
//         filenames::subsequent_ops::clusters::kFindOpsFilename);

//     res |= impl::write_measuremets_to_files(
//         data.subsequent_operations_tests.delete_tests,
//         filenames::subsequent_ops::collisions::kDeleteOpsFilename,
//         filenames::subsequent_ops::clusters::kDeleteOpsFilename);

//     res |= impl::write_measuremets_to_files(
//         data.random_operations_tests.insert_tests,
//         filenames::random_ops::collisions::kInsertOpsFilename,
//         filenames::random_ops::clusters::kInsertOpsFilename);

//     res |= impl::write_measuremets_to_files(
//         data.random_operations_tests.find_tests,
//         filenames::random_ops::collisions::kInsertOpsFilename,
//         filenames::random_ops::clusters::kInsertOpsFilename);

//     res |= impl::write_measuremets_to_files(
//         data.random_operations_tests.delete_tests,
//         filenames::random_ops::collisions::kInsertOpsFilename,
//         filenames::random_ops::clusters::kInsertOpsFilename);

//     return res;
// }

// }  // namespace data_tools

// namespace graphics_tools {

// namespace impl {

// int32_t plot_data(const char data_filename[], const char title[], const char y_label[], const char color[]) {
//     std::FILE* gnupipe = popen("gnuplot -persistent", "w");
//     if (!gnupipe) {
//         std::cerr << "Data presentation error: could not start drawing app";
//         return EXIT_FAILURE;
//     }

//     std::fprintf(gnupipe,
//                  "set title \"%s\"\n"
//                  "set xlabel \"N\"\n"
//                  "set ylabel \"%s\"\n"
//                  "plot '%s' title \"%s\" w linespoints lt rgb \"%s\"\n",
//                  title, y_label, data_filename, title, color);

//     // Dont call pclose() on gnupipe!
//     return EXIT_SUCCESS;
// }

// int32_t plot_all_data(std::string_view filename_prefix, std::string_view title_prefix,
//                       std::string_view color) {
//     int32_t res = EXIT_SUCCESS;
//     std::string data_filename(filename_prefix);
//     std::string title(title_prefix);

//     // data_filename += kRandomDataFileSuffix;
//     title += ", random array";
//     res |= plot_data(data_filename.c_str(), title.c_str(), color.data());

//     data_filename.resize(filename_prefix.size());
//     title.resize(title_prefix.size());

//     // data_filename += kReversedDataFileSuffix;
//     title += ", reversed sorted array";
//     res |= plot_data(data_filename.c_str(), title.c_str(), color.data());

//     data_filename.resize(filename_prefix.size());
//     title.resize(title_prefix.size());

//     // data_filename += kAlmostSortedDataFileSuffix;
//     title += ", almost sorted array";
//     res |= plot_data(data_filename.c_str(), title.c_str(), color.data());
//     return res;
// }

// }  // namespace impl

// void plot_saved_data() {

// }

// }  // namespace graphics_tools

void tests();

int main() {
    // int writing_res = data_tools::write_measuremets_to_files(
    //     measure_tools::measure_hashtable_metrics());
    // if (writing_res == EXIT_FAILURE) {
    //     return EXIT_FAILURE;
    // }

    // graphics_tools::plot_saved_data();
    // tests();
    UnorderedSet<int> hs;
    constexpr int K = 74;
    int vals[K] = {};
    for (uint32_t k = 0; k < K; k++) {
        vals[k] = k % 2 ? int(k * 2) : -int(k * 2 + 1);
    }
    for (int k : vals) {
        hs.insert(k);
    }
    for (int k : vals) {
        assert(hs.contains(k));
    }
    size_t first_not_removed = 0;
    for (int k : vals) {
        for (size_t i = first_not_removed; i < K; i++) {
            int k1 = vals[i];
            bool res = hs.contains(k1);
            assert(res);
        }
        bool res = hs.erase(k);
        printf("removed %d\n", k);
        first_not_removed++;
        assert(res);
    }
}

void tests() try {
    UnorderedSet<int32_t> table;
    assert(table.size() == 0);

    for (int32_t k = -100; k <= 100; k++) {
        assert(!table.contains(k));
    }

    assert(table.size() == 0);

    for (int32_t k = -100; k <= 100; k++) {
        table.insert(k);
    }

    assert(table.size() == 201);

    assert(table.contains(-100));
    for (int32_t k = -100; k <= 100; k++) {
        assert(table.contains(k));
    }

    assert(table.size() == 201);

    for (int32_t k = -100; k <= 100; k += 2) {
        assert(table.erase(k));
    }

    assert(table.size() == 100);

    for (int32_t k = -100; k <= 100; k++) {
        bool has_key = table.contains(k);
        assert(k % 2 == 0 ? !has_key : has_key);
    }

    assert(table.size() == 100);
    table.clear();
    assert(table.size() == 0);

    constexpr size_t kTests = size_t(5e5);

    std::vector<int32_t> inserted_values;
    inserted_values.reserve(kTests + 1);
    {
        std::mt19937 rnd;
        for (int i = int(kTests); i >= 0; i--) {
            int32_t k = int32_t(rnd());
            table.insert(k);
            inserted_values.push_back(k);
        }
    }
    assert(table.size() == inserted_values.size());

    for (int32_t k : inserted_values) {
        bool res = table.contains(k);
        assert(res);
    }

    for (int32_t k : inserted_values) {
        bool res = table.contains(k);
        assert(res);
    }

    for (int32_t k : inserted_values) {
        assert(table.contains(k));
        bool res = table.erase(k);
        if (!res) {
            assert(table.contains(k));
            printf("%d\n", k);
        }
        assert(res);
    }
#ifdef NDEBUG
#error "NDEBUG in tests"
#endif
    puts("Tests passed");
} catch (...) {
    puts("EXCEPTION!!!");
}
