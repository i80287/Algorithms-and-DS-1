// #define NDEBUG 1

#ifdef NDEBUG
#ifdef _FORTIFY_SOURCE
#undef _FORTIFY_SOURCE
#endif
#ifdef _GLIBCXX_DEBUG
#undef _GLIBCXX_DEBUG
#endif
#ifdef _GLIBCXX_ASSERTIONS
#undef _GLIBCXX_ASSERTIONS
#endif
#ifdef assert
#undef assert
#define assert(expression) ((void)0)
#endif
#define DBG_EXPR(expr)
#else
#define DBG_EXPR(expr) expr
#endif

#define ATTRIBUTE_ASSUME(expr) __attribute__((assume(expr)))

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

static size_t create_count = 0;

template <class KeyType, class ValueType>
struct Node2 {
    KeyType key_;
    ValueType value_;
    Node2 *next_ = nullptr;

    constexpr Node2(KeyType key, ValueType value) noexcept
        : key_(key), value_(value) {}
};

template <class KeyType, class ValueType, class Func = std::hash<KeyType>>
class HashTable2 {
    using NodeType = Node2<KeyType, ValueType>;

public:
    HashTable2();

    HashTable2(Func);

    HashTable2(size_t, double, Func = std::hash<KeyType>());

    ~HashTable2();

    void insert(KeyType, ValueType);

    ValueType *find(KeyType);

    void erase(KeyType);

    Node2<KeyType, ValueType> &operator[](uint64_t);

    Node2<KeyType, ValueType> at(uint64_t);

    size_t size() const { return size_; }

    size_t capacity() const { return buckets_.size(); }

private:
    void resize();

    constexpr bool should_resize() const noexcept {
        uint32_t cur_sz = uint32_t(size_);
        uint32_t cur_cap = uint32_t(buckets_.size());
        return double(cur_sz) > max_occupancy_rate_ * double(cur_cap);
    }

    static constexpr size_t kDefaultSize = 100;
    static constexpr double kDefaultOccupancyRate = 0.5;

    std::vector<NodeType *> buckets_;
    size_t size_ = 0;
    const double max_occupancy_rate_ = kDefaultOccupancyRate;
    Func hasher_;
};

template <class KeyType, class ValueType, class Func>
HashTable2<KeyType, ValueType, Func>::HashTable2()
    : buckets_(kDefaultSize), hasher_() {}

template <class KeyType, class ValueType, class Func>
HashTable2<KeyType, ValueType, Func>::HashTable2(Func hasher)
    : buckets_(1), hasher_(std::move(hasher)) {}

template <class KeyType, class ValueType, class Func>
HashTable2<KeyType, ValueType, Func>::HashTable2(size_t size,
                                               double max_occupancy_rate,
                                               Func hasher)
    : buckets_(size | (size == 0)),
      max_occupancy_rate_(max_occupancy_rate > 0 && max_occupancy_rate <= 1
                              ? max_occupancy_rate
                              : kDefaultOccupancyRate),
      hasher_(std::move(hasher)) {}

template <class KeyType, class ValueType, class Func>
HashTable2<KeyType, ValueType, Func>::~HashTable2() {
    for (NodeType *node : buckets_) {
        while (node != nullptr) {
            NodeType *next = node->next_;
            delete node;
            node = next;
        }
    }

    if constexpr (std::is_same_v<KeyType, int> &&
                  std::is_same_v<ValueType, int>)
        if (create_count > size_t(4e6)) [[unlikely]]
            abort();
}

template <class KeyType, class ValueType, class Func>
void HashTable2<KeyType, ValueType, Func>::insert(KeyType key, ValueType value) {
        size_t hash = hasher_(key);
    assert(buckets_.size());
    size_t index = hash % buckets_.size();
    NodeType *prev_node = nullptr;
    NodeType *current_node = buckets_[index];

    size_t cls_cnt = 0;
    while (current_node != nullptr) {
        if (current_node->key_ == key) {
            current_node->value_ = std::move(value);
            return;
        }

        prev_node = current_node;
        current_node = current_node->next_;
        cls_cnt++;
    }

    NodeType *new_node = new NodeType(key, value);
    if constexpr (std::is_same_v<KeyType, int> &&
                  std::is_same_v<ValueType, int>)
        create_count++;

    size_++;
    if (prev_node == nullptr) {
        assert(buckets_[index] == nullptr);
        buckets_[index] = new_node;
    } else {
        assert(prev_node->next_ == nullptr);
        prev_node->next_ = new_node;
    }

    if (should_resize()) {
        resize();
    }
}

template <class KeyType, class ValueType, class Func>
ValueType *HashTable2<KeyType, ValueType, Func>::find(KeyType key) {
    size_t hash = hasher_(key);
    assert(buckets_.size());
    for (NodeType *current_node = buckets_[hash % buckets_.size()];
         current_node != nullptr; current_node = current_node->next_) {
        if (current_node->key_ == key) {
            return &current_node->value_;
        }
    }

    return nullptr;
}

template <class KeyType, class ValueType, class Func>
void HashTable2<KeyType, ValueType, Func>::erase(KeyType key) {
    size_t hash = hasher_(key);
    assert(buckets_.size());
    size_t index = hash % buckets_.size();

    NodeType *current_node = buckets_[index];
    NodeType *prev_node = nullptr;
    DBG_EXPR(size_t i = 0);
    while (current_node != nullptr) {
        if (current_node->key_ == key) {
            if (prev_node == nullptr) {
                buckets_[index] = current_node->next_;
            } else {
                prev_node->next_ = current_node->next_;
            }

            delete current_node;
            size_--;
            return;
        }

        prev_node = current_node;
        current_node = current_node->next_;
        DBG_EXPR(i++);
        assert(i < size_t(5e6));
    }
}

template <class KeyType, class ValueType, class Func>
typename HashTable2<KeyType, ValueType, Func>::NodeType &
HashTable2<KeyType, ValueType, Func>::operator[](uint64_t hash) {
    NodeType *node = buckets_.at(hash);
    return node != nullptr ? *node : throw std::runtime_error("");
}

template <class KeyType, class ValueType, class Func>
typename HashTable2<KeyType, ValueType, Func>::NodeType
HashTable2<KeyType, ValueType, Func>::at(uint64_t hash) {
    return this->operator[](hash);
}

#define REHASH_TYPE_NAIVE          1
#define REHASH_TYPE_STORE_LIST_END 2

#define REHASH_TYPE REHASH_TYPE_STORE_LIST_END

constexpr size_t kMaxSize = size_t(7e6);

#if REHASH_TYPE == REHASH_TYPE_STORE_LIST_END
static Node2<int, int> *last_node_in_list[kMaxSize];
#endif

template <class KeyType, class ValueType, class Func>
void HashTable2<KeyType, ValueType, Func>::resize() {
    assert(this->buckets_.size());
    size_t bs = this->buckets_.size();
    size_t new_size = bs * 2;
    if constexpr (std::is_same_v<KeyType, int> &&
                  std::is_same_v<ValueType, int>) {
        if (new_size > kMaxSize) [[unlikely]] {
            abort();
        }

        // if (new_size > size_t(4e5)) {
        //     return;
        // }

#if REHASH_TYPE == REHASH_TYPE_STORE_LIST_END
        memset(last_node_in_list, 0, new_size * sizeof(Node2<int, int> *));
#endif

        std::vector<Node2<int, int> *> new_buckets(new_size);

        DBG_EXPR(size_t j = 0);
        DBG_EXPR(size_t k = 0);
        for (Node2<int, int> *start_node : this->buckets_) {
            while (start_node != nullptr) {
                Node2<int, int> *const next_node = start_node->next_;
                assert(start_node != next_node);
                assert(next_node == nullptr || start_node != next_node->next_);

                size_t hash = this->hasher_(start_node->key_);
                assert(hash % buckets_.size() == k);
                size_t index = hash % new_size;

#if REHASH_TYPE == REHASH_TYPE_NAIVE
                Node2<int, int> *current_node = new_buckets[index];
                assert(current_node != start_node);
                if (current_node == nullptr) {
                    new_buckets[index] = start_node;
                } else {
                    while (current_node->next_ != nullptr) {
                        assert(current_node != current_node->next_);
                        assert(current_node->next_ == nullptr ||
                               current_node != current_node->next_->next_);
                        current_node = current_node->next_;
                    }
                    assert(current_node != start_node);
                    current_node->next_ = start_node;
                }
                start_node->next_ = nullptr;
#elif REHASH_TYPE == REHASH_TYPE_STORE_LIST_END
                Node2<int, int> *last_node = last_node_in_list[index];
                if (last_node == nullptr) {
                    assert(new_buckets[index] == nullptr);
                    new_buckets[index] = start_node;
                } else {
                    assert(new_buckets[index] != nullptr);
                    last_node->next_ = start_node;
                }
                last_node_in_list[index] = start_node;
                start_node->next_ = nullptr;
#else
#error "REHASH_TYPE"
#endif

                start_node = next_node;

                DBG_EXPR(j++);
                assert(j < size_t(5e6));
            }
            DBG_EXPR(k++);
        }
        assert(j < size_t(5e6));

        this->buckets_ = std::move(new_buckets);
    } else {
        std::vector<NodeType *> new_buckets(new_size);

        for (NodeType *start_node : buckets_) {
            while (start_node != nullptr) {
                NodeType *const next_node = start_node->next_;

                size_t hash = hasher_(start_node->key_);
                size_t index = hash % new_size;

                NodeType *old_head = new_buckets[index];
                new_buckets[index] = start_node;
                start_node->next_ = old_head;

                start_node = next_node;
            }
        }

        for (size_t i = 0; i < new_size; i++) {
            NodeType *curr_node = new_buckets[i];
            if (curr_node == nullptr) {
                continue;
            }
            NodeType *next_node = curr_node->next_;
            curr_node->next_ = nullptr;
            while (next_node != nullptr) {
                NodeType *next_next_node = next_node->next_;
                next_node->next_ = curr_node;
                curr_node = next_node;
                next_node = next_next_node;
            }

            new_buckets[i] = curr_node;
        }

        buckets_ = std::move(new_buckets);
    }
}
