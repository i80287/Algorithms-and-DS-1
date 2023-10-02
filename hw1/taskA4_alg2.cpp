#include <optional>

template <typename Container>
std::optional<typename Container::value_type> algorithm2(const Container& A) {
    size_t n = A.size();
    size_t c = 1;
    size_t ind = 0;
    for (size_t i = 1; i < n; i++) {
        if (A[ind] == A[i]) {
            c = c + 1;
        }
        else {
            c = c - 1;
        }

        if (c == 0) {
            ind = i;
            c = 1;
        }
    }

    size_t A_ind_count = 0;
    for (size_t i = 0; i < n; i++) {
        A_ind_count += A[ind] == A[i];
    }

    if (A_ind_count > (n >> 1)) {
        return A[ind];
    }

    return std::nullopt;
}

#include <array>
#include <cstdint>
#include <iostream>

// int main() {
//     std::optional<int> a = algorithm2(std::array<int, 6>{ 1, 2, 1, 2, 2, 2 });
//     if (a) {
//         std::cout << a.value() << '\n';
//     }
// }
