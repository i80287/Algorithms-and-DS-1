#define NDEBUG 1

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

template <class KeyType, class ValueType>
struct Node {
    KeyType key_;
    ValueType value_;
    Node *next_ = nullptr;
    size_t key_hash_ = 0;

    constexpr Node() noexcept = default;

    template <class K, class V>
    constexpr Node(K &&key, V &&value) noexcept
        : key_(std::forward<K>(key)),
          value_(std::forward<V>(value)),
          next_(nullptr) {}
};

template <class KeyType, class ValueType, class Func = std::hash<KeyType>>
class HashTable {
public:
    static_assert(std::is_standard_layout_v<KeyType> && sizeof(KeyType) <= 32);
    static_assert(std::is_standard_layout_v<ValueType> &&
                  sizeof(KeyType) <= 32);
    static_assert(std::is_same_v<KeyType, std::string> || sizeof(KeyType) == 4);

    using NodeType = Node<KeyType, ValueType>;

    HashTable() : buckets_(kDefaultSize), has_node_(kDefaultSize) {}

    HashTable(Func hasher)
        : buckets_(1), has_node_(1), hasher_(std::move(hasher)) {}

    HashTable(size_t size, double max_load_factor,
              Func hasher = std::hash<KeyType>())
        : buckets_(size | (size == 0)),
          has_node_(size | (size == 0)),
          max_load_factor_(max_load_factor > 0 && max_load_factor <= 1
                               ? max_load_factor
                               : kDefaultLoadFactor),
          hasher_(std::move(hasher)) {
    }

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

    template <class K, class V>
    void insert(K &&, V &&);

    ValueType *find(const KeyType &);

    void erase(const KeyType &);

    Node<KeyType, ValueType> &operator[](uint64_t);

    Node<KeyType, ValueType> at(uint64_t hash) {
        return this->operator[](hash);
    }

    size_t size() const { return size_; }

    size_t capacity() const { return buckets_.size(); }

private:
    void resize();

    bool should_resize() const noexcept {
        double lf =
            double(uint32_t(size_) + 1) / double(uint32_t(buckets_.size()));
        return lf > max_load_factor_;
    }

    static constexpr size_t kDefaultSize = 100;
    static constexpr double kDefaultLoadFactor = 0.5;

    std::vector<NodeType> buckets_;
    std::vector<uint8_t> has_node_;
    size_t size_ = 0;
    const double max_load_factor_ = kDefaultLoadFactor;
    Func hasher_;
};

template <class KeyType, class ValueType, class Func>
template <class K, class V>
void HashTable<KeyType, ValueType, Func>::insert(K &&key, V &&value) {
    assert(buckets_.size() == has_node_.size());
    assert(buckets_.size());
    if (should_resize()) {
        resize();
    }
    assert(buckets_.size() == has_node_.size());
    assert(buckets_.size());

    size_++;

    size_t hash = hasher_(key);
    size_t index = hash % buckets_.size();
    NodeType &first_node = buckets_[index];
    if (!has_node_[index]) {
        has_node_[index] = true;
        first_node.key_ = std::forward<K>(key);
        first_node.value_ = std::forward<V>(value);
        assert(first_node.next_ == nullptr);
        first_node.next_ = nullptr;
        first_node.key_hash_ = hash;
        return;
    }

    NodeType *current_node = &first_node;
    NodeType *prev_node = current_node;
    do {
        if (current_node->key_ == key) {
            current_node->value_ = std::forward<V>(value);
            --size_;
            return;
        }

        prev_node = current_node;
        current_node = current_node->next_;
    } while (current_node != nullptr);
    assert(prev_node->next_ == nullptr);
    NodeType *new_node =
        new NodeType(std::forward<K>(key), std::forward<V>(value));
    new_node->key_hash_ = hash;
    prev_node->next_ = new_node;
}

template <class KeyType, class ValueType, class Func>
ValueType *HashTable<KeyType, ValueType, Func>::find(const KeyType &key) {
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
void HashTable<KeyType, ValueType, Func>::erase(const KeyType &key) {
    assert(buckets_.size() == has_node_.size());
    assert(buckets_.size());
    size_t index = hasher_(key) % buckets_.size();

    if (!has_node_[index]) {
        assert(buckets_[index].next_ == nullptr);
        return;
    }

    NodeType &index_node = buckets_[index];
    NodeType *current_node = index_node.next_;
    if (index_node.key_ == key) {
        size_--;
        has_node_[index] = current_node != nullptr;
        if (current_node != nullptr) {
            // index_node = std::move(*current_node);
            index_node.key_ = std::move(current_node->key_);
            index_node.value_ = std::move(current_node->value_);
            index_node.next_ = current_node->next_;
            index_node.key_hash_ = current_node->key_hash_;
        }
    }

    NodeType *prev_node = &index_node;
    while (current_node != nullptr) {
        if (current_node->key_ == key) {
            assert(current_node != current_node->next_);
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
void HashTable<KeyType, ValueType, Func>::resize() {
    size_t bs = buckets_.size();
    assert(bs == has_node_.size());
    assert(bs);
    size_t new_size = bs * 2;
    if (new_size + bs > (200u << 20) / sizeof(NodeType)) [[unlikely]] {
        return;
    }

    std::vector<NodeType> new_buckets(new_size);
    std::vector<uint8_t> new_has_node(new_size);

    size_t i = 0;
    for (NodeType start_node : this->buckets_) {
        if (!has_node_[i++]) {
            assert(buckets_[i - 1].next_ == nullptr);
            continue;
        }

        size_t hash = start_node.key_hash_;
        assert(hash % buckets_.size() == i - 1);
        size_t index = hash % new_size;

        if (!new_has_node[index]) {
            new_has_node[index] = true;
            NodeType &new_node_init = new_buckets[index];
            new_node_init.key_ = std::move(start_node.key_);
            new_node_init.value_ = std::move(start_node.value_);
            new_node_init.next_ = nullptr;
            new_node_init.key_hash_ = hash;
        } else {
            NodeType *current_new_node = &new_buckets[index];
            while (current_new_node->next_ != nullptr) {
                assert(current_new_node != current_new_node->next_);
                assert(current_new_node != current_new_node->next_->next_);
                current_new_node = current_new_node->next_;
            }
            NodeType *new_node = new NodeType(std::move(start_node.key_),
                                              std::move(start_node.value_));
            new_node->key_hash_ = hash;
            current_new_node->next_ = new_node;
        }

        NodeType *current_node = start_node.next_;
        while (current_node != nullptr) {
            NodeType *const next_node = current_node->next_;
            current_node->next_ = nullptr;

            hash = current_node->key_hash_;
            assert(hash % buckets_.size() == i - 1);
            index = hash % new_size;
            if (!new_has_node[index]) {
                new_has_node[index] = true;
                NodeType &new_node_init = new_buckets[index];
                new_node_init.key_ = std::move(current_node->key_);
                new_node_init.value_ = std::move(current_node->value_);
                new_node_init.next_ = nullptr;
                new_node_init.key_hash_ = hash;
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
