#include <optional>

template <typename Container>
std::optional<typename Container::value_type> algorithm1(const Container& A) {
    size_t n = A.size();
    size_t c = 0;
    ssize_t ind = -1;
    for (size_t i = 0; i < n; i++) {
        size_t c1 = 0;
        for (size_t j = 0; j < n; j++) {
            if (A[i] == A[j]) {
                c1 = c1 + 1;
            }
        }

        if (c1 > c) {
            c = c1;
            ind = static_cast<ssize_t>(i);
        }
    }

    if (c > n / 2) {
        return A[ind];
    }

    return std::nullopt;
}

#include <array>
#include <cstdint>
#include <iostream>

int main() {
    std::optional<int> a = algorithm1(std::array<int, 6>{ 1, 2, 1, 2, 2, 2 });
    if (a) {
        std::cout << a.value() << '\n';
    }
}
