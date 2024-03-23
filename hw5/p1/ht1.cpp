// #define NDEBUG 1

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

template <class KeyType, class ValueType>
struct Node {
    KeyType key_{};
    ValueType value_{};
    Node *next_ = nullptr;

    constexpr Node() noexcept = default;

    constexpr Node(KeyType key, ValueType value) noexcept
        : key_(key), value_(value) {}
};

template <class KeyType, class ValueType, class Func = std::hash<KeyType>>
class HashTable {
public:
    using NodeType = Node<KeyType, ValueType>;

    HashTable() : buckets_(kDefaultSize), has_node_(kDefaultSize) {}

    HashTable(Func hasher)
        : buckets_(size_t(1)),
          has_node_(size_t(1)),
          hasher_(hasher) {}

    HashTable(size_t size, double max_occupancy_rate,
              Func hasher = std::hash<KeyType>())
        : buckets_(size_t(size | (size == 0))),
          has_node_(size_t(size | (size == 0))),
          max_occupancy_rate_(max_occupancy_rate > 0 && max_occupancy_rate <= 1
                                  ? max_occupancy_rate
                                  : kDefaultOccupancyRate),
          hasher_(hasher) {}

    ~HashTable() {
        size_t i = 0;
        for (NodeType s_node : buckets_) {
            if (!has_node_[i++]) {
                assert(buckets_[i - 1].next_ == nullptr);
                continue;
            }
            NodeType *node = s_node.next_;
            while (node != nullptr) {
                NodeType *next = node->next_;
                assert(node != next);
                delete node;
                node = next;
            }
        }
    }

    void insert(KeyType, ValueType);

    ValueType *find(KeyType);

    void erase(KeyType);

    Node<KeyType, ValueType> &operator[](uint64_t);

    Node<KeyType, ValueType> at(uint64_t);

    size_t size() const { return size_; }

    size_t capacity() const { return buckets_.size(); }

private:
    void resize();

    bool should_resize() const noexcept {
        uint32_t next_sz = uint32_t(size_ + 1);
        uint32_t cur_cap = uint32_t(buckets_.size());
        assert(max_occupancy_rate_ > 0 && max_occupancy_rate_ <= 1);
        return double(next_sz) / double(cur_cap) > max_occupancy_rate_;
    }

    static constexpr size_t kDefaultSize = 100;
    static constexpr double kDefaultOccupancyRate = 0.5;

    std::vector<NodeType> buckets_;
    std::vector<bool> has_node_;
    size_t size_ = 0;
    const double max_occupancy_rate_ = kDefaultOccupancyRate;
    Func hasher_;
};

template <class KeyType, class ValueType, class Func>
void HashTable<KeyType, ValueType, Func>::insert(KeyType key, ValueType value) {
    assert(buckets_.size() == has_node_.size());
    assert(buckets_.size());
    if (should_resize()) {
        resize();
    }
    assert(buckets_.size() == has_node_.size());
    assert(buckets_.size());

    size_++;

    size_t index = hasher_(key) % buckets_.size();
    NodeType& first_node = buckets_[index];
    if (!has_node_[index]) {
        has_node_[index] = true;
        first_node.key_ = std::move(key);
        first_node.value_ = std::move(value);
        assert(first_node.next_ == nullptr);
        first_node.next_ = nullptr;
        return;
    }

    NodeType *current_node = &first_node;
    NodeType *prev_node = current_node;
    do {
        if (current_node->key_ == key) {
            current_node->value_ = std::move(value);
            --size_;
            return;
        }

        prev_node = current_node;
        current_node = current_node->next_;
    } while (current_node != nullptr);
    assert(prev_node->next_ == nullptr);
    prev_node->next_ = new NodeType(key, value);
}

template <class KeyType, class ValueType, class Func>
ValueType *HashTable<KeyType, ValueType, Func>::find(KeyType key) {
    assert(buckets_.size() == has_node_.size());
    assert(buckets_.size());
    size_t index = hasher_(key) % buckets_.size();
    if (!has_node_[index]) {
        assert(buckets_[index].next_ == nullptr);
        return nullptr;
    }

    NodeType *current_node = &buckets_[index];
    do {
        if (current_node->key_ == key) {
            return &current_node->value_;
        }
        current_node = current_node->next_;
    } while (current_node != nullptr);
    return nullptr;
}

template <class KeyType, class ValueType, class Func>
void HashTable<KeyType, ValueType, Func>::erase(KeyType key) {
    assert(buckets_.size() == has_node_.size());
    assert(buckets_.size());
    size_t index = hasher_(key) % buckets_.size();

    if (!has_node_[index]) {
        assert(buckets_[index].next_ == nullptr);
        return;
    }

    NodeType index_node = buckets_[index];
    NodeType *current_node = index_node.next_;
    if (index_node.key_ == key) {
        size_--;
        has_node_[index] = current_node != nullptr;
        if (current_node != nullptr) {
            buckets_[index] = std::move(*current_node);
        }
    }

    NodeType *prev_node = &index_node;
    while (current_node != nullptr) {
        if (current_node->key_ == key) {
            prev_node->next_ = current_node->next_;
            delete current_node;
            size_--;
            return;
        }

        prev_node = current_node;
        current_node = current_node->next_;
    }
}

template <class KeyType, class ValueType, class Func>
typename HashTable<KeyType, ValueType, Func>::NodeType &
HashTable<KeyType, ValueType, Func>::operator[](uint64_t hash) {
    assert(buckets_.size() == has_node_.size());
    if (!has_node_.at(hash)) {
        assert(buckets_[hash].next_ == nullptr);
        throw std::runtime_error("");
    }
    return buckets_[hash];
}

template <class KeyType, class ValueType, class Func>
typename HashTable<KeyType, ValueType, Func>::NodeType
HashTable<KeyType, ValueType, Func>::at(uint64_t hash) {
    assert(buckets_.size() == has_node_.size());
    if (!has_node_.at(hash)) {
        assert(buckets_[hash].next_ == nullptr);
        throw std::runtime_error("");
    }
    return buckets_[hash];
}

template <class KeyType, class ValueType, class Func>
void HashTable<KeyType, ValueType, Func>::resize() {
    size_t bs = buckets_.size();
    assert(bs == has_node_.size());
    assert(bs);

    if (size_ > (100 << 7) + 1) {
        abort();
    }

    size_t new_size = bs * 2;

    std::vector<NodeType> new_buckets(new_size);
    std::vector<bool> new_has_node(new_size);

    size_t i = 0;
    for (NodeType start_node : this->buckets_) {
        if (!has_node_[i++]) {
            assert(buckets_[i - 1].next_ == nullptr);
            continue;
        }

        size_t hash = this->hasher_(start_node.key_);
        assert(hash % buckets_.size() == i - 1);
        size_t index = hash % new_size;

        if (!new_has_node[index]) {
            new_has_node[index] = true;
            new_buckets[index].key_ = std::move(start_node.key_);
            new_buckets[index].value_ = std::move(start_node.value_);
            new_buckets[index].next_ = nullptr;
        } else {
            NodeType *current_new_node = &new_buckets[index];
            while (current_new_node->next_ != nullptr) {
                assert(current_new_node != current_new_node->next_);
                assert(current_new_node != current_new_node->next_->next_);
                current_new_node = current_new_node->next_;
            }
            current_new_node->next_ =
                new NodeType(start_node.key_, start_node.value_);
        }

        NodeType *current_node = start_node.next_;
        while (current_node != nullptr) {
            NodeType *const next_node = current_node->next_;
            current_node->next_ = nullptr;

            hash = this->hasher_(current_node->key_);
            assert(hash % buckets_.size() == i - 1);
            index = hash % new_size;
            if (!new_has_node[index]) {
                new_has_node[index] = true;
                new_buckets[index].key_ = std::move(current_node->key_);
                new_buckets[index].value_ = std::move(current_node->value_);
                new_buckets[index].next_ = nullptr;
                delete current_node;
            } else {
                NodeType *current_new_node = &new_buckets[index];
                while (current_new_node->next_ != nullptr) {
                    assert(current_new_node != current_new_node->next_);
                    assert(current_new_node != current_new_node->next_->next_);
                    current_new_node = current_new_node->next_;
                }
                assert(current_new_node != &start_node);
                current_new_node->next_ = current_node;
            }

            current_node = next_node;
        }
    }

    this->buckets_ = std::move(new_buckets);
    this->has_node_ = std::move(new_has_node);
}
