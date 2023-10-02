#include <algorithm>
#include <vector>
#include <iostream>

size_t CountArrayRotations(std::vector<int>& array) {
    return 0;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    size_t size;
    std::cin >> size;
    std::vector<int> array;
    for (size_t i = 0; i < size; ++i) {
        std::cin >> array[i];
    }

    std::cout << CountArrayRotations(array);
    return 0;
}