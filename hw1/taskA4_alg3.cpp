#include <algorithm>
#include <optional>

template <typename Container>
std::optional<typename Container::value_type> algorithm3(Container& A) {
    size_t n = A.size();
    if (n == 1) {
        return A[0];
    }

    size_t c = 1;
    std::sort(A.begin(), A.end());
    for (size_t i = 1; i < n; i++) {
        if (A[i - 1] == A[i]) {
            c = c + 1;
        }
        else {
            if (c > n / 2) {
                return A[i - 1];
            }

            c = 1;
        }
    }

    if (c > (n >> 1)) {
        return A[n - 1];
    }

    return std::nullopt;
}

#include <array>
#include <cstdint>
#include <iostream>

int main() {
    std::array<int, 6> arr{ 1, 2, 1, 2, 2, 2 };
    std::optional<int> a = algorithm3(arr);
    if (a) {
        std::cout << a.value() << '\n';
    }
}
