#pragma once

#include <cassert>
#include <cstdint>
#include <vector>

using std::int32_t;
using std::size_t;
using std::uint32_t;

template <class Key, size_t MaxProbeRatio = 2>
class UnorderedSet {
    static_assert(MaxProbeRatio >= 1);

public:
    template <class K = Key>
    void insert(K&& key);

    bool contains(const Key& key) const noexcept;

    bool erase(const Key& key) noexcept;

    void clear() noexcept;

    constexpr size_t last_collisions_count() const noexcept {
        return last_operation_collisions_count_;
    }

    constexpr size_t clusters_count() const noexcept {
        // TODO: O(1) upd in other methods
        return 0;
        // size_t clusters_cnt = 0;
        // return clusters_cnt;
        // std::vector<uint8_t> visited(taken_.size());
        // for (size_t hash_value_mod_m = 0, m = taken_.size(); hash_value_mod_m < m;
        //      hash_value_mod_m++) {
        //     if (visited[hash_value_mod_m]) {
        //         continue;
        //     }
        //     visited[hash_value_mod_m] = true;
        //     if (!taken_[hash_value_mod_m]) {
        //         continue;
        //     }
        //     size_t bin = hash_value_mod_m;
        //     size_t i   = 1;
        //     do {
        //         // bin = (hash_value_mod_m + i * i * i / 3 + i * i / 2 * i / 6) & (m - 1);
        //         bin          = (bin + i * i) & (m - 1);
        //         visited[bin] = true;
        //         i++;
        //     } while (taken_[bin]);
        //     clusters_cnt++;
        // }

        // return clusters_cnt;
    }

    constexpr size_t size() const noexcept { return buckets_.actual_occupied(); }

    constexpr size_t capacity() const noexcept { return buckets_.size(); }

    constexpr bool empty() const noexcept { return size() == 0; }

private:
    void resize_with_rehash();

    enum class BucketState : uint8_t {
        kNull = 0,
        kOccupied,
        kErased,
    };

    class BucketsManager {
    public:
        static constexpr size_t kInitialSize = 16;

        static_assert(kInitialSize > 0 && (kInitialSize & (kInitialSize - 1)) == 0);

        BucketsManager() : buckets_state_(kInitialSize, BucketState::kNull) {
            buckets_ = static_cast<Key*>(operator new(sizeof(Key) * kInitialSize));
            check_size_correctness();
        }

        ~BucketsManager() {
            check_size_correctness();
            for (Key *iter = buckets_, *iter_end = iter + buckets_state_.size();
                 iter != iter_end; ++iter) {
                iter->~Key();
            }
            operator delete(buckets_);
        }

        BucketsManager(const BucketsManager& other)
            : buckets_state_(other.buckets_state_), taken_buckets_(other.taken_buckets_) {
            size_t sz              = size();
            buckets_               = static_cast<Key*>(operator new(sizeof(Key) * sz));
            size_t first_to_create = 0;
            try {
                for (; first_to_create < sz; first_to_create++) {
                    if (other.bucket_state(first_to_create) == BucketState::kOccupied) {
                        construct_at(first_to_create, other.buckets_[first_to_create]);
                    }
                }
            } catch (...) {
                for (; first_to_create > 0; first_to_create--) {
                    if (bucket_state[first_to_create - 1] == BucketState::kOccupied) {
                        destroy_at(first_to_create - 1);
                    }
                }
                operator delete(buckets_);
                buckets_ = nullptr;
                throw;
            }

            check_size_correctness();
        }

        BucketsManager& operator=(const BucketsManager& other) {
            return *this = BucketsManager(other);
        }

        constexpr BucketsManager(BucketsManager&& other) noexcept
            : buckets_(std::move(other.buckets_)),
              buckets_state_(std::move(other.buckets_state_)),
              taken_buckets_(other.taken_buckets_) {
            other.buckets_       = nullptr;
            other.taken_buckets_ = 0;
        }

        constexpr BucketsManager& operator=(BucketsManager&& other) noexcept {
            auto tmp_buckets         = std::move(other.buckets_);
            auto tmp_buckets_state   = std::move(other.buckets_state_);
            size_t tmp_taken_buckets = other.taken_buckets_;

            other.buckets_       = nullptr;
            other.taken_buckets_ = 0;

            buckets_          = std::move(tmp_buckets);
            tmp_buckets_state = std::move(tmp_buckets_state);
            taken_buckets_    = tmp_taken_buckets;
            return *this;
        }

        const Key& operator[](size_t index) const noexcept {
            check_size_correctness();
            assert(index < size());
            assert(bucket_state(index) == BucketState::kOccupied);
            return buckets_[index];
        }

        Key& operator[](size_t index) noexcept {
            check_size_correctness();
            assert(index < size());
            assert(bucket_state(index) == BucketState::kOccupied);
            return buckets_[index];
        }

        constexpr size_t actual_occupied() const noexcept { return taken_buckets_; }

        constexpr size_t size() const noexcept { return buckets_state_.size(); }

        BucketState bucket_state(size_t index) const noexcept {
            check_size_correctness();
            assert(index < size());
            return buckets_state_[index];
        }

        template <class K = Key>
        void construct_at(size_t index,
                          K&& key) noexcept(noexcept(Key(std::forward<K>(key)))) {
            check_size_correctness();
            assert(index < size());
            assert(buckets_state_[index] != BucketState::kOccupied);
            new (&buckets_[index]) Key(std::forward<K>(key));
            buckets_state_[index] = BucketState::kOccupied;
            taken_buckets_++;
        }

        void destroy_at(size_t index) noexcept {
            check_size_correctness();
            assert(taken_buckets_);
            assert(index < size());
            assert(buckets_state_[index] == BucketState::kOccupied);
            (buckets_ + index)->~Key();
            buckets_state_[index] = BucketState::kErased;
            taken_buckets_--;
        }

        void clear() noexcept {
            check_size_correctness();
            for (Key *iter = buckets_, *iter_end = iter + buckets_state_.size();
                 iter != iter_end; ++iter) {
                iter->~Key();
            }
            std::fill(buckets_state_.begin(), buckets_state_.end(), BucketState::kNull);
            taken_buckets_ = 0;
        }

        void check_size_correctness() const noexcept {
            size_t m = buckets_state_.size();
            assert((m & (m - 1)) == 0 && m != 0);
        }

    private:
        Key* buckets_ = nullptr;
        std::vector<BucketState> buckets_state_;
        size_t taken_buckets_ = 0;
    };

    BucketsManager buckets_;

    mutable size_t last_operation_collisions_count_ = 0;
};

template <class Key, size_t MaxProbeRatio>
template <class K>
void UnorderedSet<Key, MaxProbeRatio>::insert(K&& key) {
    buckets_.check_size_correctness();

    size_t m              = buckets_.size();
    const size_t hash     = std::hash<Key>{}(key);
    size_t bin            = hash & (m - 1);
    size_t collisions_cnt = 0;
    for (size_t i = 1; i <= m; i++) {
        assert(bin < m);
        if (buckets_.bucket_state(bin) != BucketState::kOccupied) {
            break;
        }
        bin = (bin + i * i) & (m - 1);
        collisions_cnt++;
    }

    if (collisions_cnt * MaxProbeRatio >= m) [[unlikely]] {
        resize_with_rehash();
        m   = buckets_.size();
        bin = hash & (m - 1);
        for (size_t i = 1; i <= m; i++) {
            assert(bin < m);
            if (buckets_.bucket_state(bin) != BucketState::kOccupied) {
                break;
            }
            bin = (bin + i * i) & (m - 1);
            collisions_cnt++;
        }
    }

    assert(buckets_.bucket_state(bin) != BucketState::kOccupied);
    buckets_.construct_at(bin, std::forward<K>(key));
    last_operation_collisions_count_ = collisions_cnt;
}

template <class Key, size_t MaxProbeRatio>
void UnorderedSet<Key, MaxProbeRatio>::resize_with_rehash() {
    buckets_.check_size_correctness();

    // size_t m      = buckets_.size();
    // size_t new_sz = m * 2;
    // assert(new_sz != 0);
    // std::vector<int32_t> new_buckets(new_sz);
    // std::vector<bool> new_taken(new_sz);
    // for (size_t i = 0; i < m; i++) {
    //     if (taken_[i]) {
    //         size_t bin = std::hash<Key>{}(buckets_[i]) & (new_sz - 1);
    //         for (size_t j = 1; j <= new_sz; j++) {
    //             assert(bin < new_sz);
    //             if (!new_taken[bin]) {
    //                 break;
    //             }
    //             bin = (bin + j * j) & (new_sz - 1);
    //         }
    //         assert(!new_taken[bin]);
    //         new_buckets[bin] = std::move(buckets_[i]);
    //         new_taken[bin]   = true;
    //     }
    // }

    // std::swap(buckets_, new_buckets);
    // std::swap(taken_, new_taken);
}

template <class Key, size_t MaxProbeRatio>
bool UnorderedSet<Key, MaxProbeRatio>::contains(const Key& key) const noexcept {
    buckets_.check_size_correctness();

    size_t m                = buckets_.size();
    size_t bin              = std::hash<Key>{}(key) & (m - 1);
    bool found              = false;
    size_t collisions_count = 0;
    for (size_t i = 1; i <= m; i++) {
        assert(bin < m);
        BucketState bucket_state = buckets_.bucket_state(bin);
        if (bucket_state == BucketState::kNull) {
            break;
        } else if (bucket_state == BucketState::kOccupied) {
            found = buckets_[bin] == key;
            if (found) {
                break;
            }
        }
        bin = (bin + i * i) & (m - 1);
        collisions_count++;
    }

    last_operation_collisions_count_ = collisions_count;
    return found;
}

template <class Key, size_t MaxProbeRatio>
bool UnorderedSet<Key, MaxProbeRatio>::erase(const Key& key) noexcept {
    printf("Removing %d\n", key);
    buckets_.check_size_correctness();

    const size_t m          = buckets_.size();
    size_t bin              = std::hash<Key>{}(key) & (m - 1);
    size_t i                = 1;
    bool found              = false;
    size_t collisions_count = 0;
    for (; i <= m; i++) {
        assert(bin < m);
        BucketState bucket_state = buckets_.bucket_state(bin);
        if (bucket_state == BucketState::kNull) {
            break;
        } else if (bucket_state == BucketState::kOccupied) {
            found = buckets_[bin] == key;
            if (found) {
                break;
            }
        }

        bin = (bin + i * i) & (m - 1);
        collisions_count++;
    }

    last_operation_collisions_count_ = collisions_count;

    if (found) [[likely]] {
        assert(buckets_.bucket_state(bin) == BucketState::kOccupied);
        assert(buckets_[bin] == key);
        buckets_.destroy_at(bin);
    } else [[unlikely]] {
        assert(buckets_.bucket_state(bin) == BucketState::kNull || i > m);
        assert(buckets_.bucket_state(bin) != BucketState::kOccupied ||
               buckets_[bin] != key);
    }

    return found;
}

template <class Key, size_t MaxProbeRatio>
void UnorderedSet<Key, MaxProbeRatio>::clear() noexcept {
    buckets_.clear();
}
