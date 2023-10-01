#ifndef _FINDKEY_HPP_
#define _FINDKEY_HPP_ 1

#include <cstdint>  // size_t
#include <concepts> // std::same_as<T>
#include <utility>  // std::pair<T, U>
#include <vector>   // std::vector<T, ...>

template <typename T>
using matrix_t = std::vector<std::vector<T>>;

/// @brief Searches for the key in the matrix (with special order of the elements).
/// @tparam T type of the element.
/// @param matrix matrix.
/// @param key key to find.
/// @return pair of indexes { row, column } if element is found. Otherwise, returns { (size_t)-1, (size_t)-1 }.
template <typename T>
requires requires (const T& a, const T& b) {
    { a < b } -> std::same_as<bool>;
    { a == b } -> std::same_as<bool>;
}
std::pair<size_t, size_t> FindKey(const matrix_t<T>& matrix, const T& key) {
    std::pair<size_t, size_t> ans{ static_cast<size_t>(-1), static_cast<size_t>(-1) };
    if (matrix.empty()) {
        return ans;
    }

    size_t n = matrix.size();
    size_t m = matrix[0].size();
    size_t i = 0;
    for (size_t j = 0; j < m; j++) {
        while (i < n && key < matrix[i].at(j)) {
            i++;
        }

        if (i == n) {
            return ans;
        }

        if (key == matrix[i][j]) {
            ans.first = i;
            ans.second = j;
            return ans;
        }
    }

    return ans;
}

#endif // !_FINDKEY_HPP_
