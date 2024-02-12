#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <class NodeT, class K, class V>
static bool list_has_kv(NodeT node, K key, V value) {
    while (node != nullptr) {
        if (node->key_ == key) {
            return node->value_ == value;
        }
        node = node->next_;
    }
    return false;
}

template <
    template <class KeyType, class ValueType, class Func = std::hash<KeyType>>
    class HashTableType>
void single_test() {
    {
        HashTableType<int, int> table(1, 0.5);
        for (int i = 0; i <= 5; i++) table.insert(i, i);
        assert(table.size() == 6);
    }
    {
        HashTableType<int, int> table(10, 0.1);

        constexpr int k = 1e3;
        for (int i = k; i >= 0; i--) {
            table.insert(i, k - i);
        }
        assert(table.size() == size_t(k + 1));
        for (int i = k; i >= 0; i--) {
            [[maybe_unused]] const int *p = table.find(i);
            assert(p && *p == k - i);
        }
        assert(table.size() == size_t(k + 1));
        for (int i = 3 * k; i > k; i--) {
            assert(table.find(i) == nullptr);
        }
        assert(table.size() == size_t(k + 1));
        for (int i = -1; i >= -k; i--) {
            assert(table.find(i) == nullptr);
        }
        assert(table.size() == size_t(k + 1));
    }
    {
        HashTableType<int, int> table(200, 0.2);

        constexpr int k = 1e4;
        for (int i = k; i >= 0; i--) {
            table.insert(i, k - i);
        }
        assert(table.size() == size_t(k + 1));
        for (int i = k; i >= 0; i--) {
            [[maybe_unused]] const int *p = table.find(i);
            assert(p && *p == k - i);
        }
        assert(table.size() == size_t(k + 1));
        for (int i = 3 * k; i > k; i--) {
            assert(table.find(i) == nullptr);
        }
        assert(table.size() == size_t(k + 1));
        for (int i = -1; i >= -k; i--) {
            assert(table.find(i) == nullptr);
        }
        assert(table.size() == size_t(k + 1));
        for (int i = 0; i <= k; i++) {
            table.erase(i);
        }
        assert(table.size() == 0);
    }
    {
        HashTableType<std::string, std::string> table(200, 0.2);
        table.insert("abc", "def");
        assert(table.find("abc"));
        assert(table.size() == 1);
        assert(
            table.at(std::hash<std::string>{}("abc") % table.capacity()).key_ ==
            "abc");
        assert(table[std::hash<std::string>{}("abc") % table.capacity()].key_ ==
               "abc");

        {
            std::string s = "{}";
            s.reserve(s.size() + 'z' - '0');
            for (uint32_t i = '0'; i <= 'z'; i++) {
                s.push_back(char(i));
                size_t prev_size = s.size();
                table.insert(s, s);
                assert(prev_size == s.size());
            }
        }
        {
            std::string s = "{}";
            s.reserve(s.size() + 'z' - '0');
            for (uint32_t i = '0'; i <= 'z'; i++) {
                s.push_back(char(i));
                size_t prev_size = s.size();
                auto res = table.find(s);
                assert(prev_size == s.size());
                assert(res && *res == s);
            }
        }
    }
    {
        HashTableType<int, std::string> table(200, 0.2);
        table.insert(1, "def");
        assert(table.find(1));
        assert(table.size() == 1);
        assert(table.at(std::hash<int>{}(1) % table.capacity()).key_ == 1);
        assert(table[std::hash<int>{}(1) % table.capacity()].key_ == 1);

        {
            std::string s = "{}";
            s.reserve(s.size() + 'z' - '0' + 1);
            for (uint32_t i = '0'; i <= 'z'; i++) {
                s.push_back(char(i));
                size_t prev_size = s.size();
                table.insert(int32_t(i), s);
                assert(prev_size == s.size());
            }
            s.push_back(char('z' + 1));
            table.insert('z' + 1, std::move(s));
            assert(s.size() == 0);
        }
        {
            std::string s = "{}";
            s.reserve(s.size() + 'z' - '0' + 1);
            for (uint32_t i = '0'; i <= 'z' + 1; i++) {
                s.push_back(char(i));
                auto res = table.find(int32_t(i));
                assert(res && *res == s);
            }
        }
    }
    {
        HashTableType<std::string, std::string> table;
        table.insert("abc", "def");
        assert(table.find("abc"));
        assert(table.size() == 1);
        assert(
            table.at(std::hash<std::string>{}("abc") % table.capacity()).key_ ==
            "abc");
        assert(table[std::hash<std::string>{}("abc") % table.capacity()].key_ ==
               "abc");
    }
    {
        HashTableType<int, std::string> table;
        table.insert(1, "def");
        assert(table.find(1));
        assert(table.size() == 1);
        assert(table.at(std::hash<int>{}(1) % table.capacity()).key_ == 1);
        assert(table[std::hash<int>{}(1) % table.capacity()].key_ == 1);
    }
    {
#if defined(__clang__)
        constexpr int64_t k = 5e5;
#else
        constexpr int64_t k = 1e4;
#endif
        HashTableType<int32_t, int64_t> table;
        for (int32_t i = -k; i <= k; i++) {
            table.insert(i, i);
        }
        assert(table.size() == 2 * k + 1);
        for (int32_t i = -k; i <= k; i++) {
            auto node = table.at(std::hash<int32_t>{}(i) % table.capacity());
            assert(list_has_kv(&node, i, i));
        }
        assert(table.size() == 2 * k + 1);
        for (int32_t i = -k; i <= k; i++) {
            const int64_t *value = table.find(i);
            assert(value != nullptr && *value == i);
        }
        assert(table.size() == 2 * k + 1);
        for (int32_t i = k; i >= -k; i--) {
            const int64_t *value = table.find(i);
            assert(value != nullptr && *value == i);
        }
        assert(table.size() == 2 * k + 1);
        for (int32_t i = k; i >= -k; i--) {
            table.erase(i);
        }
        assert(table.size() == 0);
    }
    {
#if defined(__clang__)
        constexpr int64_t k = 5e5;
#else
        constexpr int64_t k = 1e4;
#endif
        HashTableType<int32_t, int64_t> table;
        for (int64_t i = -k; i <= k; i++) {
            table.insert(INT32_MAX, i);
        }
        for (int64_t i = -k; i <= k; i++) {
            table.insert(INT32_MIN, i);
        }
        assert(table.size() == 2);

        for (int32_t i = -k; i <= k; i++) {
            table.insert(i, i);
        }
        assert(table.size() == 2 * k + 1 + 2);
        for (int32_t i = k; i >= -k; i--) {
            table.erase(i);
        }
        assert(table.size() == 2);
        table.erase(INT32_MAX);
        table.erase(INT32_MIN);
        assert(table.size() == 0);

        assert(k > 0);
        int32_t key = 0;
        for (int64_t iter = 5e2; iter > 0; iter--) {
            table.insert(key, iter);
            key += int32_t(table.capacity());
        }
        assert(table.size() == size_t(5e2));
    }
    {
        HashTableType<int, int> table;
        constexpr int k = 1e5;
        for (int i = k; i >= 0; i--) {
            table.insert(i, k - i);
        }
        assert(table.size() == size_t(k + 1));
        for (int i = k; i >= 0; i--) {
            [[maybe_unused]] const int *p = table.find(i);
            assert(p && *p == k - i);
        }
        for (int i = k; i >= 0; i--) {
            auto node = table.at(std::hash<int64_t>{}(i) % table.capacity());
            assert(list_has_kv(&node, i, k - i));
        }
        assert(table.size() == size_t(k + 1));
        for (int i = 3 * k; i > k; i--) {
            assert(table.find(i) == nullptr);
        }
        assert(table.size() == size_t(k + 1));
        for (int i = -1; i >= -k; i--) {
            assert(table.find(i) == nullptr);
        }
        assert(table.size() == size_t(k + 1));
        for (int i = 0; i <= k; i++) {
            table.erase(i);
        }
        assert(table.size() == 0);
    }
}

// #include "HashTable.cpp"
// #include "ht1.cpp"
// #include "ht2.cpp"
#include "ht3.cpp"

#ifdef NDEBUG
#error "e"
#endif

int main() {
    single_test<HashTable>();
    // single_test<HashTable2>();
    // multi_test();
}

// void multi_test() {
//     HashTable<int, int> table1;
//     HashTable2<int, int> table2;
//     std::unordered_map<int, int> table3;
//     std::mt19937 rnd;
//     for (int i = 1e5; i > 0; i--) {
//         int k = int(rnd());
//         int v = int(rnd());
//         table1.insert(k, v);
//         table2.insert(k, v);
//         table3.insert_or_assign(k, v);
//     }

//     for (auto [k, v] : table3) {
//         const int32_t *v1 = table1.find(k);
//         assert(v1 && *v1 == v);
//         const int32_t *v2 = table2.find(k);
//         assert(v2 && *v2 == v);
//     }
// }