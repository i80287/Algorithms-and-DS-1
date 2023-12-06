#include <cstdint>
#include <iostream>
#if __cplusplus >= 202002L
#include <type_traits>
#endif
#include <utility>

template <class Iterator>
#if __cplusplus >= 202002L
requires std::is_arithmetic_v<typename std::iterator_traits<Iterator>::value_type>
constexpr
#endif
static inline void buildHeap(Iterator begin, size_t length) noexcept {
    for (size_t i = length / 2; i != 0; ) {
        size_t parent_index = --i;
        auto elem = begin[parent_index];

        size_t son_index = parent_index * 2 | 1;
        auto son = begin[son_index];
        if (son_index + 1 < length && son < begin[son_index + 1]) {
            son = begin[++son_index];
        }

        while (elem < son) {
            begin[parent_index] = son;
            begin[son_index] = elem;
            
            parent_index = son_index;
            son_index = parent_index * 2 | 1;
            if (son_index >= length) {
                break;
            }

            son = begin[son_index];
            if (son_index + 1 < length && son < begin[son_index + 1]) {
                son = begin[++son_index];
            }
        }
    }
}

template <class Iterator>
#if __cplusplus >= 202002L
requires std::is_arithmetic_v<typename std::iterator_traits<Iterator>::value_type>
constexpr
#endif
static inline void heapify(Iterator begin, size_t heap_size, size_t start_index) noexcept {
    size_t parent_index = start_index;
    size_t left_son_index = start_index * 2 | 1;
    size_t right_son_index = left_son_index + 1;
    if (left_son_index >= heap_size) {
        return;
    }

    auto sifting_elem = *(begin + parent_index);
    size_t son_index = left_son_index;
    auto son = *(begin + son_index);
    if (right_son_index < heap_size && son < *(begin + right_son_index)) {
        son_index = right_son_index;
        son = *(begin + son_index);
    }

    while (sifting_elem < son) {            
        *(begin + parent_index) = son;
        *(begin + son_index) = sifting_elem;

        parent_index = son_index;
        son_index = son_index * 2 | 1;
        if (son_index >= heap_size) {
            break;
        }

        son = *(begin + son_index);
        if (son_index + 1 != heap_size && son < *(begin + (son_index + 1))) {
            son = *(begin + ++son_index);
        }
    }
}

template <class Iterator>
#if __cplusplus >= 202002L
requires std::is_arithmetic_v<typename std::iterator_traits<Iterator>::value_type>
constexpr
#endif
void heapSort(Iterator begin, size_t length) noexcept {
    if (length == 0) {
        return;
    }

    buildHeap(begin, length);

    for (size_t heap_last_index = length - 1; heap_last_index != 0; heap_last_index--) {
        // Pop max elem from the top of the pyramid and add to the end.
        std::swap(*(begin), *(begin + heap_last_index));
        // Back pyramide (heap) to the balance state.
        heapify(begin, heap_last_index, 0);
    }
}

template <class Iterator>
#if __cplusplus >= 202002L
requires std::is_arithmetic_v<typename std::iterator_traits<Iterator>::value_type>
constexpr
#endif
void heapSort(Iterator begin, Iterator end) noexcept {
    heapSort(begin, static_cast<size_t>(end - begin));
}

int main() {
    using std::cin, std::cout;
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);
    uint32_t n = 0;
    cin >> n;
    int32_t* array = static_cast<int32_t*>(operator new(n * sizeof(int32_t)));
    for (int32_t* iter = array, *iter_end = iter + n; iter != iter_end; ++iter) {
        cin >> *iter;
    }
    heapSort(array, static_cast<size_t>(n));
    for (int32_t* iter = array, *iter_end = iter + n; iter != iter_end; ++iter) {
        cout << *iter << ' ';
    }
    operator delete(array);
}
