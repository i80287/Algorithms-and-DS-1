#include "findkey.hpp"

#include <iostream>
#include <exception>
#include <stdexcept>
#include <set>
#include <unordered_set>

namespace tests {

template <typename T>
void CheckElementsInMatrix(size_t& test_cnt, const matrix_t<T>& matrix) {
    for (const auto& matrix_row : matrix) {
        for (const auto& elem : matrix_row) {
            auto [row, col] = FindKey(matrix, elem);
            if (row == static_cast<size_t>(-1) || matrix.at(row).at(col) != elem) {
                std::cerr << "Test " << test_cnt << " failed at element " << elem << '\n';
            }
            else {
                std::cout << "Test " << test_cnt << " passed\n";
            }

            test_cnt++;
        }
    }
}

void CheckElementsNotInMatrix(size_t& test_cnt, const matrix_t<int64_t>& matrix) {
    std::set<int64_t> numbers_in_matrix;
    for (const auto& row : matrix) {
        numbers_in_matrix.insert(row.begin(), row.end());
    }

    int64_t min_element = *numbers_in_matrix.begin();
    int64_t max_element = *(--numbers_in_matrix.end());

    constexpr int32_t elements_count = 16;

    for (int64_t elem = min_element - 1; elem != min_element - elements_count; elem--) {
        auto [row, col] = FindKey(matrix, elem); // we should not find elem now
        if (row != static_cast<size_t>(-1) || col != static_cast<size_t>(-1)) {
            std::cerr << "Test " << test_cnt << " failed at element " << elem << '\n';
        }
        else {
            std::cout << "Test " << test_cnt << " passed\n";
        }

        test_cnt++;
    }

    for (int64_t elem = max_element + 1; elem != max_element + elements_count; elem++) {
        auto [row, col] = FindKey(matrix, elem); // we should not find elem now
        if (row != static_cast<size_t>(-1) || col != static_cast<size_t>(-1)) {
            std::cerr << "Test " << test_cnt << " failed at element " << elem << '\n';
        }
        else {
            std::cout << "Test " << test_cnt << " passed\n";
        }

        test_cnt++;
    }
}

template <typename T>
void CheckMatrix(const matrix_t<T>& matrix) {
    if (matrix.empty()) {
        throw std::runtime_error{"empty matrix"};
    }

    size_t n = matrix.size();
    size_t m = matrix[0].size();
    std::unordered_set<T> elements;
    elements.reserve(n * m);

    for (size_t i = 0; i + 1 < n; i++) {
        for (size_t j = 0; j + 1 < m; j++) {
            if (!(matrix[i].at(j) < matrix[i].at(j + 1)) || !(matrix[i + 1].at(j) < matrix[i][j])) {
                throw std::runtime_error{"matrix elements ordering error"};
            }
        }
    }

    for (const auto& matrix_row : matrix) {
        elements.insert(matrix_row.begin(), matrix_row.end());
    }

    if (elements.size() != n * m) {
        throw std::runtime_error{"matrix must contains only unique elements"};
    }
}

void ShortTest1(size_t& test_cnt) {
    const matrix_t<int64_t> matrix = {
        { 11, 12, 18 },
        {  8,  9, 10 },
        {  5,  6,  7 },
    };
    CheckMatrix(matrix);

    CheckElementsInMatrix(test_cnt, matrix);
    CheckElementsNotInMatrix(test_cnt, matrix);
}

void ShortTest2(size_t& test_cnt) {
    const matrix_t<int64_t> matrix = {
        {  213,  334,  345, 3435, 4584, 5669 },
        {  124,  258,  339, 1024, 2048, 4096 },
        {   93,  101,  225,  489,  490,  581 },
        {   47,   94,  122,  336,  488,  502 },
        {    2,   48,   99,  229,  487,  499 },
        {   -8,   -1,   23,   45,  103,  255 },
    };
    CheckMatrix(matrix);

    CheckElementsInMatrix(test_cnt, matrix);
    CheckElementsNotInMatrix(test_cnt, matrix);
}

} // namespace tests

int main() {
    size_t test_cnt = 1;
    tests::ShortTest1(test_cnt);
    tests::ShortTest2(test_cnt);
}
