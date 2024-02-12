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

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ext/pb_ds/assoc_container.hpp>
#include <iostream>
#include <string>
#include <vector>

#if 0
template <class T>
using unordered_set = __gnu_pbds::gp_hash_table<T, __gnu_pbds::null_type>;
#else
#include <unordered_set>
template <class T>
using unordered_set = std::unordered_set<T>;
#endif

constexpr uint32_t log10_floor(uint32_t n) noexcept {
    constexpr uint8_t table1[33] = {
        10, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 4,
        4,  4, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0,
    };

    constexpr uint32_t table2[11] = {
        1,       10,       100,       1000,       10000, 100000,
        1000000, 10000000, 100000000, 1000000000, 0,
    };
    uint32_t digits = table1[__builtin_clz(n)];
    digits -= ((n - table2[digits]) >> 31);
    return digits;
}

class BloomFilter {
public:
    BloomFilter(size_t hashes, size_t bits);

    ~BloomFilter();

    void add(const std::string &obj);

    bool verify(const std::string &obj);

    double getFPRate() const;

    size_t numberOfHashFunctions() const;
    size_t numberOfBits() const;

private:
    std::vector<std::vector<bool>> matrix_;
    const uint32_t num_of_hf_ = 0;
    const uint32_t num_of_bits_ = 0;
    size_t count_of_fp = 0;
    size_t count_of_verify = 0;
    unordered_set<std::string> strings_in_filter;
};

BloomFilter::BloomFilter(size_t hashes, size_t bits)
    : matrix_(std::vector<std::vector<bool>>(hashes, std::vector<bool>(bits))),
      num_of_hf_(uint32_t(hashes)),
      num_of_bits_(uint32_t(bits) / uint32_t(hashes)) {
    assert(hashes != 0 && uint32_t(hashes) == hashes);
    assert(bits != 0 && uint32_t(bits) == bits);
    assert(uint32_t(bits) % uint32_t(hashes) == 0);
}

BloomFilter::~BloomFilter() {}

void BloomFilter::add(const std::string &obj) {
    strings_in_filter.insert(obj);

    std::string s;
    size_t real_str_size = obj.size();
    assert(num_of_hf_ != 0);
    s.reserve(real_str_size + (log10_floor(num_of_hf_) + 1));
    s = obj;

    matrix_[0][std::hash<std::string>{}(s) % num_of_bits_] = true;

    for (uint32_t i = 1; i < num_of_hf_; i++) {
        s.resize(real_str_size);
        s += std::to_string(i);
        size_t row_index = std::hash<std::string>{}(s) % num_of_bits_;
        matrix_[i][row_index] = true;
    }
}

bool BloomFilter::verify(const std::string &obj) {
    count_of_verify++;

    std::string s;
    size_t real_str_size = obj.size();
    assert(num_of_hf_ != 0);
    s.reserve(real_str_size + (log10_floor(num_of_hf_) + 1));
    s = obj;

    if (!matrix_[0][std::hash<std::string>{}(s) % num_of_bits_]) {
        assert(strings_in_filter.find(obj) == strings_in_filter.end());
        return false;
    }

    for (uint32_t i = 1; i < num_of_hf_; i++) {
        s.resize(real_str_size);
        s += std::to_string(i);
        size_t row_index = std::hash<std::string>{}(s) % num_of_bits_;
        if (!matrix_[i][row_index]) {
            assert(strings_in_filter.find(obj) == strings_in_filter.end());
            return false;
        }
    }

    count_of_fp += (strings_in_filter.find(obj) == strings_in_filter.end());
    return true;
}

double BloomFilter::getFPRate() const {
    return count_of_verify ? double(count_of_fp) / double(count_of_verify) : 0;
}

size_t BloomFilter::numberOfHashFunctions() const { return num_of_hf_; }

size_t BloomFilter::numberOfBits() const { return num_of_bits_ * num_of_hf_; }

int main() {
    constexpr size_t kHFNum = 4;
    constexpr size_t kBitsNum = 16;
    BloomFilter bf(kHFNum, kBitsNum);
    assert(bf.getFPRate() == 0);
    assert(bf.numberOfBits() == kBitsNum);
    assert(bf.numberOfHashFunctions() == kHFNum);

    bf.add("abc");
    bf.add("abd");
    bf.add("abe");
    assert(bf.verify("abc"));
    assert(bf.verify("abd"));
    assert(bf.verify("abe"));
    assert(bf.getFPRate() == 0);
}
