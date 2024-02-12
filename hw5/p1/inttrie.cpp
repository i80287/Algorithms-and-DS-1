// #define NDEBUG 1

#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

template <bool unroll_loops = false>
class inttrie {
public:
    using value_t = void *;

private:
    static constexpr size_t kBits = 4;
    static constexpr size_t kSize = 1u << kBits;
    static constexpr uint32_t kMask = (1u << kBits) - 1;
    static constexpr size_t kDefaultCapacity = 32;
    static constexpr size_t kMaxNumLen = sizeof(uint32_t) * CHAR_BIT / kBits;

    struct trienode {
        uint32_t table[kSize];
        value_t value;

        constexpr bool terminal() const noexcept { return value != 0; }
    };

public:
    inttrie() noexcept
        : nodes_(static_cast<trienode *>(calloc(kDefaultCapacity, sizeof(trienode)))) {
        assert(nodes_);
    }

    ~inttrie() {
        if (nodes_) {
            free(nodes_);
        }
    }

    void assign(int n, value_t m) noexcept { assign(uint32_t(n), m); }

    value_t get(int n) const noexcept { return get(uint32_t(n)); }

    void erase(int n) noexcept { erase(uint32_t(n)); }

    constexpr size_t size() const noexcept { return actual_size_; }

private:
    void assign(uint32_t n, value_t m) noexcept {
        assert(size_ <= capacity_);
        size_t current_node_index = 0;

        if constexpr (unroll_loops) {
            size_t next_node_index;

            if (n == 0)
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                goto ASSIGN_FIRST_FOR_CYCLE_END_0123456789;
            }
            current_node_index = next_node_index;
            n >>= kBits;

        ASSIGN_FIRST_FOR_CYCLE_END_0123456789:
        } else {
            for (; n != 0; n >>= kBits) {
                assert(current_node_index < size_);
                size_t next_node_index =
                    nodes_[current_node_index].table[n & kMask];
                if (next_node_index == 0) {
                    break;
                }
                current_node_index = next_node_index;
            }
        }

        assert(current_node_index < size_);
        assert(n == 0 || nodes_[current_node_index].table[n & kMask] == 0);

        if (size_t old_capacity = capacity_;
            size_ + kMaxNumLen > old_capacity) {
            size_t new_capacity = old_capacity * 2;
            assert(new_capacity > size_ + kMaxNumLen);
            nodes_ = static_cast<trienode *>(
                realloc(nodes_, new_capacity * sizeof(trienode)));
            memset(&nodes_[old_capacity], 0,
                   (new_capacity - old_capacity) * sizeof(trienode));
            capacity_ = new_capacity;
            assert(nodes_);
        }

        size_t new_node_index = size_;

        if constexpr (unroll_loops) {
            if (n == 0)
                goto ASSIGN_SECOND_FOR_CYCLE_END_0123456789;
            nodes_[current_node_index].table[n & kMask] =
                uint32_t(new_node_index);
            current_node_index = new_node_index;
            new_node_index++;
            n >>= kBits;
            if (n == 0)
                goto ASSIGN_SECOND_FOR_CYCLE_END_0123456789;
            nodes_[current_node_index].table[n & kMask] =
                uint32_t(new_node_index);
            current_node_index = new_node_index;
            new_node_index++;
            n >>= kBits;
            if (n == 0)
                goto ASSIGN_SECOND_FOR_CYCLE_END_0123456789;
            nodes_[current_node_index].table[n & kMask] =
                uint32_t(new_node_index);
            current_node_index = new_node_index;
            new_node_index++;
            n >>= kBits;
            if (n == 0)
                goto ASSIGN_SECOND_FOR_CYCLE_END_0123456789;
            nodes_[current_node_index].table[n & kMask] =
                uint32_t(new_node_index);
            current_node_index = new_node_index;
            new_node_index++;
            n >>= kBits;
            if (n == 0)
                goto ASSIGN_SECOND_FOR_CYCLE_END_0123456789;
            nodes_[current_node_index].table[n & kMask] =
                uint32_t(new_node_index);
            current_node_index = new_node_index;
            new_node_index++;
            n >>= kBits;
            if (n == 0)
                goto ASSIGN_SECOND_FOR_CYCLE_END_0123456789;
            nodes_[current_node_index].table[n & kMask] =
                uint32_t(new_node_index);
            current_node_index = new_node_index;
            new_node_index++;
            n >>= kBits;
            if (n == 0)
                goto ASSIGN_SECOND_FOR_CYCLE_END_0123456789;
            nodes_[current_node_index].table[n & kMask] =
                uint32_t(new_node_index);
            current_node_index = new_node_index;
            new_node_index++;
            n >>= kBits;
            if (n == 0)
                goto ASSIGN_SECOND_FOR_CYCLE_END_0123456789;
            nodes_[current_node_index].table[n & kMask] =
                uint32_t(new_node_index);
            current_node_index = new_node_index;
            new_node_index++;
            n >>= kBits;

        ASSIGN_SECOND_FOR_CYCLE_END_0123456789:
        } else {
            for (; n != 0; n >>= kBits) {
                assert(current_node_index < size_ + kMaxNumLen);
                nodes_[current_node_index].table[n & kMask] =
                    uint32_t(new_node_index);
                current_node_index = new_node_index;
                new_node_index++;
            }
        }

        size_ = new_node_index;

        assert(n == 0);
        assert(current_node_index < size_);
        actual_size_ += !nodes_[current_node_index].terminal();
        nodes_[current_node_index].value = m;
    }

    value_t get(uint32_t n) const noexcept {
        assert(size_ <= capacity_);
        size_t current_node_index = 0;

        if constexpr (unroll_loops) {
            size_t next_node_index;
            if (n == 0)
                goto GET_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return value_t(0);
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto GET_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return value_t(0);
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto GET_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return value_t(0);
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto GET_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return value_t(0);
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto GET_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return value_t(0);
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto GET_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return value_t(0);
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto GET_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return value_t(0);
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto GET_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return value_t(0);
            }
            current_node_index = next_node_index;
        GET_END_LABEL_0123456789:
        } else {
            for (; n != 0; n >>= kBits) {
                assert(current_node_index < size_);
                size_t next_node_index =
                    nodes_[current_node_index].table[n & kMask];
                if (next_node_index == 0) {
                    return value_t(0);
                }
                current_node_index = next_node_index;
            }
        }

        assert(current_node_index < size_);
        return nodes_[current_node_index].value;
    }

    void erase(uint32_t n) noexcept {
        assert(size_ <= capacity_);
        size_t current_node_index = 0;

        if constexpr (unroll_loops) {
            size_t next_node_index;
            if (n == 0)
                goto ERASE_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return;
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto ERASE_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return;
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto ERASE_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return;
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto ERASE_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return;
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto ERASE_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return;
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto ERASE_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return;
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto ERASE_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return;
            }
            current_node_index = next_node_index;
            n >>= kBits;
            if (n == 0)
                goto ERASE_END_LABEL_0123456789;
            next_node_index = nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return;
            }
            current_node_index = next_node_index;

        ERASE_END_LABEL_0123456789:
        } else {
            for (; n != 0; n >>= kBits) {
                assert(current_node_index < size_);
                size_t next_node_index =
                    nodes_[current_node_index].table[n & kMask];
                if (next_node_index == 0) {
                    return;
                }
                current_node_index = next_node_index;
            }
        }

        assert(current_node_index < size_);
        actual_size_ -= nodes_[current_node_index].terminal();
        nodes_[current_node_index].value = value_t(0);
    }

    trienode *nodes_ = nullptr;
    size_t size_ = 1;
    size_t capacity_ = kDefaultCapacity;
    size_t actual_size_ = 0;
};

#include <chrono>
#include <map>
#include <random>

template <class inttrie_type>
[[maybe_unused]] static void test_trie() {
    using value_t = inttrie_type::value_t;
    std::mt19937 rnd;

    std::map<int, value_t> map;
    inttrie_type trie;

    for (int i = 1e6; i > 0; i--) {
        int k = int(rnd());
        value_t v = value_t(uintmax_t(rnd()));
        v = value_t(uintmax_t(v) | uintmax_t(v == 0));
        map.insert_or_assign(k, v);
        trie.assign(k, v);
    }

    assert(trie.size() == map.size());

    for (auto [k, v] : map) {
        auto res = trie.get(k);
        assert(res == v);
    }

    assert(trie.size() == map.size());

    for (auto [k, v] : map) {
        trie.erase(k);
    }

    assert(trie.size() == 0);
}

[[maybe_unused]] static void measure_tries() {
    using value_t = inttrie<true>::value_t;

    constexpr size_t kSamples = 1e6;
    std::vector<std::pair<int, value_t>> samples(kSamples);
    std::mt19937 rnd;

    for (int test = 8; test > 0; test--) {
        for (auto &pair : samples) {
            pair.first = int(rnd());
            value_t v = value_t(uintmax_t(rnd()));
            v = value_t(uintmax_t(v) | uintmax_t(v == 0));
            pair.second = v;
        }

        inttrie trie;
        inttrie trie_unrolled;

        namespace chrono = std::chrono;

        auto start = chrono::high_resolution_clock::now();

        for (auto [k, v] : samples) {
            trie.assign(k, v);
            trie_unrolled.assign(k, v);
        }

        if (trie.size() != trie_unrolled.size()) [[unlikely]] {
            perror("1");
            return;
        }

        for (auto [k, v] : samples) {
            auto res1 = trie.get(k);
            auto res2 = trie_unrolled.get(k);
            if (res1 != v) [[unlikely]] {
                perror("2");
                return;
            }
            if (res2 != v) [[unlikely]] {
                perror("3");
                return;
            }
        }

        if (trie.size() != trie_unrolled.size()) [[unlikely]] {
            perror("4");
            return;
        }

        for (auto [k, v] : samples) {
            trie.erase(k);
        }

        if (trie.size() == 0) [[unlikely]] {
            perror("5");
            return;
        }
        if (trie_unrolled.size() == 0) [[unlikely]] {
            perror("6");
            return;
        }

        auto end = chrono::high_resolution_clock::now();
        printf(
            "%llu\n",
            chrono::duration_cast<chrono::milliseconds>(end - start).count());
    }
}

int main() {
    constexpr bool kTest = true;
    if constexpr (kTest) {
#ifdef NDEBUG
        static_assert(false);
#endif
        test_trie<inttrie<true>>();
        test_trie<inttrie<false>>();
    } else {
        measure_tries();
    }
}
