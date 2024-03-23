// #define NDEBUG 1

#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#define USE_PREV_FIELD 1

template <class KeyType, class ValueType>
struct Node {
    KeyType key_;
    ValueType value_;
    Node *next_ = nullptr;
#if defined(USE_PREV_FIELD) && USE_PREV_FIELD
    Node *prev_ = nullptr;
#endif
    size_t key_hash_ = size_t(-1);

    static constexpr bool kHasKeyHash = true;

    template <class K, class V>
    constexpr Node(K &&key, V &&value) noexcept
        : key_(std::forward<K>(key)), value_(std::forward<V>(value)) {}
};

template <class ValueType>
struct Node<std::string, ValueType> {
    std::string key_;
    ValueType value_;
    Node *next_ = nullptr;
    size_t key_hash_ = size_t(-1);

    static constexpr bool kHasKeyHash = true;

    constexpr Node(const std::string &key, const ValueType &value) noexcept
        : key_(key), value_(value) {}

    constexpr Node(const std::string &key, ValueType &&value) noexcept
        : key_(key), value_(std::move(value)) {}

    constexpr Node(std::string &&key, const ValueType &value) noexcept
        : key_(std::move(key)), value_(value), next_(nullptr) {}

    constexpr Node(std::string &&key, ValueType &&value) noexcept
        : key_(std::move(key)), value_(std::move(value)) {}
};

template <class ValueType>
struct Node<int32_t, ValueType> {
    int key_;
    ValueType value_;
    Node *next_ = nullptr;
#if defined(USE_PREV_FIELD) && USE_PREV_FIELD
    Node *prev_ = nullptr;
#endif

    static constexpr bool kHasKeyHash = false;

    constexpr Node(int key, const ValueType &value) noexcept
        : key_(key), value_(value) {}

    constexpr Node(int key, ValueType &&value) noexcept
        : key_(key), value_(std::move(value)) {}
};

template <class ValueType>
struct Node<uint32_t, ValueType> {
    uint32_t key_;
    ValueType value_;
    Node *next_ = nullptr;
#if defined(USE_PREV_FIELD) && USE_PREV_FIELD
    Node *prev_ = nullptr;
#endif

    static constexpr bool kHasKeyHash = false;

    constexpr Node(uint32_t key, const ValueType &value) noexcept
        : key_(key), value_(value) {}

    constexpr Node(uint32_t key, ValueType &&value) noexcept
        : key_(key), value_(std::move(value)) {}
};

template <class KeyType, class ValueType>
class inttrie {
public:
    using TrieKeyType = uint32_t;
    using NodeType = Node<KeyType, ValueType>;

    static constexpr size_t kBits = 4;
    static constexpr size_t kSize = 1u << kBits;
    static constexpr uint32_t kMask = (1u << kBits) - 1;
    static constexpr size_t kDefaultCapacity = 4096;
    static constexpr size_t kMaxNumLen = sizeof(TrieKeyType) * CHAR_BIT / kBits;

    static_assert(std::is_unsigned_v<TrieKeyType>);
    static_assert(kMaxNumLen <= kDefaultCapacity);

    struct trienode {
        uint32_t table[kSize];
        NodeType *value;

        constexpr bool terminal() const noexcept { return value != nullptr; }
    };

    inttrie() noexcept
        : nodes_(static_cast<trienode *>(
              calloc(kDefaultCapacity, sizeof(trienode)))) {
        assert(nodes_);
    }

    ~inttrie() {
        if (nodes_) {
            free(nodes_);
        }
    }

    template <class V>
    NodeType *assign_or_allocate_new(TrieKeyType n, V &&m) noexcept {
        assert(size_ <= capacity_);
        KeyType initial_n = KeyType(n);
        size_t current_node_index = 0;

        for (; n != 0; n >>= kBits) {
            assert(current_node_index < size_);
            size_t next_node_index =
                nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                break;
            }
            current_node_index = next_node_index;
        }

        assert(current_node_index < size_);
        assert(n == 0 || nodes_[current_node_index].table[n & kMask] == 0);

        if (size_t old_capacity = capacity_; size_ + kMaxNumLen > old_capacity)
            [[unlikely]] {
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

        for (; n != 0; n >>= kBits) {
            assert(current_node_index < size_ + kMaxNumLen);
            nodes_[current_node_index].table[n & kMask] =
                uint32_t(new_node_index);
            current_node_index = new_node_index;
            new_node_index++;
        }

        size_ = new_node_index;

        assert(n == 0);
        assert(current_node_index < size_);
        NodeType *current_node = nodes_[current_node_index].value;
        if (current_node == nullptr) {
            return nodes_[current_node_index].value =
                       new NodeType(initial_n, std::forward<V>(m));
        } else {
            current_node->value_ = std::forward<V>(m);
            return nullptr;
        }
    }

    NodeType *get(TrieKeyType n) const noexcept {
        assert(size_ <= capacity_);
        size_t current_node_index = 0;

        for (; n != 0; n >>= kBits) {
            assert(current_node_index < size_);
            size_t next_node_index =
                nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return nullptr;
            }
            current_node_index = next_node_index;
        }

        assert(current_node_index < size_);
        return nodes_[current_node_index].value;
    }

    NodeType *erase(TrieKeyType n) noexcept {
        assert(size_ <= capacity_);
        size_t current_node_index = 0;

        for (; n != 0; n >>= kBits) {
            assert(current_node_index < size_);
            size_t next_node_index =
                nodes_[current_node_index].table[n & kMask];
            if (next_node_index == 0) {
                return nullptr;
            }
            current_node_index = next_node_index;
        }

        assert(current_node_index < size_);
        NodeType *old_value = nodes_[current_node_index].value;
        nodes_[current_node_index].value = nullptr;
        return old_value;
    }

    trienode *nodes_ = nullptr;
    size_t size_ = 1;
    size_t capacity_ = kDefaultCapacity;
};

template <class KeyType, class ValueType, class Func = std::hash<KeyType>>
class HashTable {
    // These asserts works
    static_assert(std::is_standard_layout_v<KeyType> && sizeof(KeyType) <= 32);
    static_assert(std::is_standard_layout_v<ValueType> &&
                  sizeof(KeyType) <= 32);
    static_assert(std::is_same_v<KeyType, std::string> ||
                  sizeof(KeyType) == 4);

    // This doesnt
    // static_assert(std::is_scalar_v<KeyType> || std::is_same_v<KeyType,
    // std::string> || std::is_same_v<KeyType, std::string_view>);

    static constexpr bool kUseTrie = !std::is_same_v<KeyType, std::string>;
    static constexpr bool kReinterpretKey = !std::is_arithmetic_v<KeyType>;
    static_assert(!kUseTrie || sizeof(KeyType) == 4);
public:
    using NodeType = Node<KeyType, ValueType>;

    HashTable() : buckets_(kDefaultSize) {}

    HashTable(Func hasher) : buckets_(1), hasher_(std::move(hasher)) {}

    HashTable(size_t size, double max_load_factor,
              Func hasher = std::hash<KeyType>())
        : buckets_(size | (size == 0)),
          max_load_factor_(max_load_factor > 0 && max_load_factor <= 1
                               ? max_load_factor
                               : kDefaultLoadFactor),
          hasher_(std::move(hasher)) {}

    ~HashTable() {
        if constexpr (kUseTrie) {
            for (auto [node, _] : buckets_) {
                while (node != nullptr) {
                    NodeType *next = node->next_;
                    delete node;
                    node = next;
                }
            }
        } else {
            for (NodeType *node : buckets_) {
                while (node != nullptr) {
                    NodeType *next = node->next_;
                    delete node;
                    node = next;
                }
            }
        }
    }

    template <class K, class V>
    void insert(K &&, V &&);

    ValueType *find(const KeyType &);

    void erase(const KeyType &);

    Node<KeyType, ValueType> &operator[](uint64_t hash) {
        NodeType *node;
        if constexpr (kUseTrie) {
            node = buckets_.at(hash).first;
        } else {
            node = buckets_.at(hash);
        }
        return node != nullptr ? *node : throw std::runtime_error("");
    }

    Node<KeyType, ValueType> at(uint64_t hash) {
        return this->operator[](hash);
    }

    size_t size() const { return size_; }

    size_t capacity() const { return buckets_.size(); }

private:
    void resize();

    constexpr bool should_resize() const noexcept {
        double lf =
            double(uint32_t(size_) + 1) / double(uint32_t(buckets_.size()));
        return lf > max_load_factor_;
    }

    static constexpr size_t kDefaultSize = 100;
    static constexpr double kDefaultLoadFactor = 0.5;

    std::conditional_t<kUseTrie, std::vector<std::pair<NodeType *, NodeType *>>,
                       std::vector<NodeType *>>
        buckets_;
    size_t size_ = 0;
    const double max_load_factor_ = kDefaultLoadFactor;
    Func hasher_;
    std::conditional_t<kUseTrie,
                       inttrie<KeyType, ValueType>, int>
        trie_;
};

template <class KeyType, class ValueType, class Func>
template <class K, class V>
void HashTable<KeyType, ValueType, Func>::insert(K &&key, V &&value) {
    if (should_resize()) [[unlikely]] {
        resize();
    }

    assert(buckets_.size());
    size_t hash = hasher_(key);
    size_t index = hash % buckets_.size();

    if constexpr (kUseTrie) {
        uint32_t trie_key = 0;
        if constexpr (kReinterpretKey) {
            trie_key = *((uint32_t *)&key);
        } else {
            trie_key = uint32_t(key);
        }

        NodeType *new_node = trie_.assign_or_allocate_new(trie_key, std::forward<V>(value));
        if (new_node != nullptr) {
            size_++;
            if (buckets_[index].first == nullptr) {
                assert(buckets_[index].second == nullptr);
                buckets_[index].first = new_node;
            } else {
                assert(buckets_[index].second != nullptr);
            }
            NodeType *list_tail = buckets_[index].second;
            if (list_tail != nullptr) {
                list_tail->next_ = new_node;
            }

            new_node->prev_ = list_tail;
            buckets_[index].second = new_node;
        }
    } else {
        NodeType *prev_node = nullptr;
        NodeType *current_node = buckets_[index];
        size_t collisions_count = 0;
        while (current_node != nullptr) {
            if (current_node->key_ == key) {
                current_node->value_ = std::forward<V>(value);
                return;
            }

            prev_node = current_node;
            current_node = current_node->next_;
            collisions_count++;
        }

        NodeType *new_node =
            new NodeType(std::forward<K>(key), std::forward<V>(value));
        if constexpr (NodeType::kHasKeyHash) {
            new_node->key_hash_ = hash;
        }
        size_++;
        if (prev_node == nullptr) {
            assert(buckets_[index] == nullptr);
            buckets_[index] = new_node;
        } else {
            assert(buckets_[index] != nullptr);
            assert(prev_node->next_ == nullptr);
            prev_node->next_ = new_node;
        }
    }
}

template <class KeyType, class ValueType, class Func>
ValueType *HashTable<KeyType, ValueType, Func>::find(const KeyType &key) {
    assert(buckets_.size());
    if constexpr (kUseTrie) {
        uint32_t trie_key = 0;
        if constexpr (kReinterpretKey) {
            trie_key = *((uint32_t *)&key);
        } else {
            trie_key = uint32_t(key);
        }


        NodeType *node_ptr = trie_.get(trie_key);
        return node_ptr != nullptr ? &node_ptr->value_ : nullptr;
    } else {
        for (NodeType *current_node = buckets_[hasher_(key) % buckets_.size()];
             current_node != nullptr; current_node = current_node->next_) {
            if (current_node->key_ == key) {
                return &current_node->value_;
            }
        }

        return nullptr;
    }
}

template <class KeyType, class ValueType, class Func>
void HashTable<KeyType, ValueType, Func>::erase(const KeyType &key) {
    assert(buckets_.size());
    size_t index = hasher_(key) % buckets_.size();

    if constexpr (kUseTrie) {
        uint32_t trie_key = 0;
        if constexpr (kReinterpretKey) {
            trie_key = *((uint32_t *)&key);
        } else {
            trie_key = uint32_t(key);
        }

        NodeType *old_node = trie_.erase(trie_key);
        if (old_node == nullptr) {
            return;
        }

        NodeType *prev_node = old_node->prev_;
        NodeType *next_node = old_node->next_;

        if (prev_node != nullptr) {
            prev_node->next_ = next_node;
        } else {
            assert(buckets_[index].first == old_node);
            buckets_[index].first = next_node;
        }
        if (next_node != nullptr) {
            next_node->prev_ = prev_node;
        } else {
            assert(buckets_[index].second == old_node);
            buckets_[index].second = prev_node;
        }

        delete old_node;
        size_--;
    } else {
        NodeType *current_node = buckets_[index];
        NodeType *prev_node = nullptr;
        while (current_node != nullptr) {
            if (current_node->key_ == key) {
                NodeType *next_node = current_node->next_;

                if (prev_node != nullptr) {
                    prev_node->next_ = next_node;
                } else {
                    assert(buckets_[index] == current_node);
                    buckets_[index] = next_node;
                }

                delete current_node;
                size_--;
                return;
            }

            prev_node = current_node;
            current_node = current_node->next_;
        }
    }
}

// constexpr size_t kMaxSize = size_t(128);
// static Node<int, int> *last_node_in_list[kMaxSize];

template <class KeyType, class ValueType, class Func>
void HashTable<KeyType, ValueType, Func>::resize() {
    size_t new_size = this->buckets_.size() * 2;
    assert(new_size);
    if constexpr (kUseTrie) {
        if (new_size >= 400) {
            return;
        }

        std::vector<std::pair<NodeType *, NodeType *>> new_buckets(new_size);
        for (auto [start_node, _] : this->buckets_) {
            while (start_node != nullptr) {
                NodeType *const next_node = start_node->next_;
                assert(start_node != next_node);
                assert(next_node == nullptr || start_node != next_node->next_);

                size_t index;
                if constexpr (NodeType::kHasKeyHash) {
                    index = start_node->key_hash_ % new_size;
                } else {
                    index = hasher_(start_node->key_) % new_size;
                }

                NodeType *last_node = new_buckets[index].second;
                if (last_node == nullptr) {
                    assert(new_buckets[index].first == nullptr);
                    assert(new_buckets[index].second == nullptr);
                    new_buckets[index].first = start_node;
                } else {
                    assert(new_buckets[index].first != nullptr);
                    assert(new_buckets[index].second != nullptr);
                    last_node->next_ = start_node;
                }

                if constexpr (kUseTrie) {
                    start_node->prev_ = new_buckets[index].second;
                }
                start_node->next_ = nullptr;
                new_buckets[index].second = start_node;

                start_node = next_node;
            }
        }

        this->buckets_ = std::move(new_buckets);
    } else {
        std::vector<NodeType *> new_buckets(new_size);
        for (NodeType *start_node : buckets_) {
            while (start_node != nullptr) {
                NodeType *const next_node = start_node->next_;

                size_t hash;
                if constexpr (NodeType::kHasKeyHash) {
                    hash = start_node->key_hash_;
                } else {
                    hash = hasher_(start_node->key_);
                }
                size_t index = hash % new_size;
                NodeType *current_node = new_buckets[index];
                if (current_node == nullptr) {
                    new_buckets[index] = start_node;
                } else {
                    while (current_node->next_ != nullptr) {
                        current_node = current_node->next_;
                    }
                    current_node->next_ = start_node;
                }
                start_node->next_ = nullptr;

                start_node = next_node;
            }
        }

        this->buckets_ = std::move(new_buckets);
    }

    // std::vector<NodeType *> new_buckets(new_size);
    // if constexpr (std::is_same_v<NodeType, Node<int, int>>) {
    // if (__builtin_expect(new_size > kMaxSize, false)) {
    //     abort();
    // }

    // memset(last_node_in_list, 0, new_size * sizeof(Node<int, int> *));

    // for (NodeType *start_node : this->buckets_) {
    //     while (start_node != nullptr) {
    //         Node<int, int> *const next_node = start_node->next_;
    //         assert(start_node != next_node);
    //         assert(next_node == nullptr || start_node !=
    //         next_node->next_);

    //         size_t index = hasher_(start_node->key_) % new_size;

    //         Node<int, int> *last_node = last_node_in_list[index];
    //         if (last_node == nullptr) {
    //             assert(new_buckets[index] == nullptr);
    //             new_buckets[index] = start_node;
    //         } else {
    //             assert(new_buckets[index] != nullptr);
    //             last_node->next_ = start_node;
    //         }
    //         last_node_in_list[index] = start_node;
    //         start_node->next_ = nullptr;

    //         start_node = next_node;
    //     }
    // }
    // } else {
    // for (NodeType *start_node : buckets_) {
    //     while (start_node != nullptr) {
    //         NodeType *const next_node = start_node->next_;

    //         size_t index = hasher_(start_node->key_) % new_size;

    //         NodeType *old_head = new_buckets[index];
    //         new_buckets[index] = start_node;
    //         start_node->next_ = old_head;

    //         start_node = next_node;
    //     }
    // }

    // for (size_t i = 0; i < new_size; i++) {
    //     NodeType *curr_node = new_buckets[i];
    //     if (curr_node == nullptr) {
    //         continue;
    //     }
    //     NodeType *next_node = curr_node->next_;
    //     curr_node->next_ = nullptr;
    //     while (next_node != nullptr) {
    //         NodeType *next_next_node = next_node->next_;
    //         next_node->next_ = curr_node;
    //         curr_node = next_node;
    //         next_node = next_next_node;
    //     }

    //     new_buckets[i] = curr_node;
    // }
    // }
}
