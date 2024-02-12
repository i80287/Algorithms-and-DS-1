#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace collisions_gen {

namespace impl {

using uint128_t = __uint128_t;

inline constexpr uint32_t kPrime = 31;
inline constexpr uint32_t kMaxN = 2000;
inline constexpr uint32_t kDefaultStartN = 1;

/// @brief f: { 0, ..., 2^32 - 1 } -> { 0, 1 }
/// @param n
/// @return n |-> n ∈ [1; 26]
constexpr bool between_1_and_26(uint32_t n) noexcept { return n - 1 <= 25; }

constexpr bool check_n_in_base_p(uint128_t n) noexcept {
    do {
        uint32_t r = uint32_t(n % kPrime);
        n /= kPrime;
        if (!between_1_and_26(r)) {
            return false;
        }
    } while (n);
    return true;
}

/// @brief Finds integer n, such that all digits
///        of n * 2^64 in pase kPrime ∈ [1; 26]
/// @tparam Start
/// @return n
template <uint32_t StartN = kDefaultStartN>
constexpr uint32_t find_zero_mod_num() noexcept {
    for (uint32_t n = StartN;; n++) {
        if (check_n_in_base_p(uint128_t(n) << 64)) {
            return n;
        }
    }
}

static std::vector<uint32_t> to_polynomial(uint32_t m) {
    uint128_t n = uint128_t(m) << 64;
    std::vector<uint32_t> poly;
    poly.reserve(40);
    do {
        uint32_t r = uint32_t(n % kPrime);
        n /= kPrime;
        assert(between_1_and_26(r));
        poly.push_back(r);
    } while (n);
    return poly;
}

template <uint32_t StartN = kDefaultStartN>
static std::vector<uint32_t> find_zero_rem_polynomial() {
    constexpr uint32_t n = find_zero_mod_num<StartN>();
    /**
     * n < 2^32 => n * 2^64 < 2^96 => size of answer <= 
     * <= ⌈ log_kPrime(2^32) ⌉ = 20
     */
    return to_polynomial(n);
}

static std::string polynomial_to_string(const std::vector<uint32_t>& poly) {
    std::string s(poly.size(), '\0');
    for (size_t i = 0; i < poly.size(); i++) {
        s[i] = char(poly[i] - 1 + 'a');
    }
    return s;
}

}  // namespace impl

// #pragma GCC optimize("O3,unroll-loops")
// #pragma GCC target("lzcnt,bmi,bmi2,avx,avx2")
// #include <bits/stdc++.h>
// using namespace std;
// #define int long long
constexpr long long polynomial_hash(std::string_view s) noexcept {
    constexpr long long p = impl::kPrime;
    long long hash = 0;
    long long p_pow = 1;
    for (size_t i = 0; i < s.length(); ++i) {
        hash += (s[i] - 'a' + 1) * p_pow;
        p_pow *= p;
    }

    return hash;
}

/// @brief Functions that generates @a size strings with the same
///        hash as @a str has. Hash functions is @ref polynomial_hash.
/// @tparam Seed optional argument that may change generated set of string.
/// @param str Initial string. If empty, @a size strings
///            with hash = 0 will be generated.
/// @param size Number of strings that should be generated.
/// @return vector of @a size pairwise different strings,
///         each one has the same hash as @a str has.
template <uint32_t Seed = impl::kDefaultStartN>
std::vector<std::string> generate_strings_with_same_hash(std::string_view str,
                                                         uint32_t size) {
    auto poly = impl::find_zero_rem_polynomial<Seed>();
    assert(poly.size() <= 20);
    std::string zero_hash_string = impl::polynomial_to_string(poly);
    assert(polynomial_hash(zero_hash_string) == 0);
    assert(zero_hash_string.size() == poly.size());

    std::string accum_string;
    accum_string.reserve(str.size() + zero_hash_string.size() * size);
    accum_string = str;

    std::vector<std::string> res;
    res.reserve(size);
    long long str_hash = polynomial_hash(str);
    for (uint32_t i = 0; i < size; i++) {
        accum_string += zero_hash_string;
        assert(polynomial_hash(accum_string) == str_hash);
        res.push_back(accum_string);
    }

    return res;
}

}  // namespace collisions_gen
