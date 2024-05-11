#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace std;

#if defined __VSCODE_LOCALGHOST__ && __cplusplus >= 202100L && 1
#define COMPILE_TIME_TESTING
#endif

using index_t                    = size_t;
using code_t                     = uint32_t;
constexpr index_t kNullNodeIndex = 0;
constexpr index_t kRootNodeIndex = kNullNodeIndex + 1;

struct edges_map final {
    constexpr index_t operator[](size_t chr) const noexcept {
        return chr - shift < container.size() ? container[chr - shift]
                                              : kNullNodeIndex;
    }

    constexpr index_t& operator[](size_t chr) {
        if (chr < shift) {
            size_t old_size     = container.size();
            const size_t growth = shift - chr;
            container.resize(old_size + growth);
#if defined __cpp_lib_is_constant_evaluated && \
    __cpp_lib_is_constant_evaluated >= 201811L
            if (is_constant_evaluated()) {
                for (size_t i = old_size - 1; int64_t(i) >= 0; i--) {
                    container[i + growth] = container[i];
                }
                for (size_t i = 0; i < growth; i++) {
                    container[i] = 0;
                }
            } else
#endif
            {
                std::copy_n(container.begin(), old_size,
                            container.begin() + int64_t(growth));
                std::fill_n(container.begin(), growth, index_t(0));
            }
            shift = chr;
        } else if (chr - shift >= container.size()) {
            if (container.empty()) {
                shift = chr;
            }
            container.resize(chr - shift + 1);
        }

        return container[chr - shift];
    }

    vector<index_t> container;
    size_t shift = 0;
};

struct node_t final {
    edges_map edges{};
    code_t code{};

    constexpr index_t operator[](size_t index) const noexcept {
        return edges[index];
    }

    constexpr index_t& operator[](size_t index) {
        return edges[index];
    }
};

// clang-format off
#ifdef COMPILE_TIME_TESTING
constexpr
#endif
static string decode(const vector<uint32_t>& codes) {
    // clang-format on
    vector<node_t> nodes;
    vector<std::string> code_literals;
    code_literals.reserve(size_t(2e4));
    nodes.reserve(size_t(1e3));
    nodes.resize(kRootNodeIndex + 1);
    assert(kNullNodeIndex < nodes.size());
    assert(kRootNodeIndex < nodes.size());

    uint32_t free_code      = 0;
    index_t free_node_index = kRootNodeIndex + 1;
    for (uint32_t c = 0; c < 128; c++) {
        const char str[] = {char(c), '\0'};
        code_literals.emplace_back(string_view(str, 1));
        nodes[kRootNodeIndex][c]  = free_node_index++;
        nodes.emplace_back().code = free_code++;
    }

    assert(code_literals.at('a') == "a");
    assert(code_literals.at('z') == "z");

    index_t current_node_index = kRootNodeIndex;
    string s;
    vector<string_view> hist;
    string_view prev_code_literal;
    string tmp;
    s.reserve(size_t(1.5e4));
    hist.reserve(size_t(2e3));
    tmp.reserve(100);
    for (uint32_t code : codes) {
        const bool code_exists = code < code_literals.size();
        if (!code_exists) {
            tmp = prev_code_literal;
            tmp += prev_code_literal.front();
        }
        const string_view current_code_literal =
            code_exists ? code_literals[code] : tmp;

        for (size_t i = 0; i < current_code_literal.size(); i++) {
            uint8_t chr = uint8_t(current_code_literal[i]);
            assert(current_node_index < nodes.size());
            index_t next_node_index =
                as_const(nodes[current_node_index])[chr];
            if (next_node_index == kNullNodeIndex) {
                nodes.emplace_back().code      = free_code;
                nodes[current_node_index][chr] = free_node_index;

                assert(free_code == code_literals.size());
                s                        = prev_code_literal;
                const size_t read_length = i + 1;
                s += current_code_literal.substr(0, read_length);
                code_literals.push_back(s);
                assert(code_literals.at(free_code) == s);

                free_code++;
                free_node_index++;
                next_node_index = kRootNodeIndex + 1 + chr;
            }

            current_node_index = next_node_index;
        }

        if (!code_exists) {
            assert(tmp == code_literals.back());
            prev_code_literal = code_literals.back();
        } else {
            prev_code_literal = current_code_literal;
        }

        assert(!prev_code_literal.empty());
        hist.emplace_back(prev_code_literal);
    }

    s.clear();
    for (auto p : hist) {
        s += p;
    }
    return s;
}

#ifdef COMPILE_TIME_TESTING
static_assert(decode({97, 98, 99, 99}) == "abcc");
static_assert(decode({97, 98, 97, 97}) == "abaa");
static_assert(decode({97, 97, 98}) == "aab");
static_assert(decode({97, 97}) == "aa");
static_assert(decode({97, 128}) == "aaa");
static_assert(decode({97, 98, 129}) == "abbb");
static_assert(decode({97, 98, 128}) == "abab");
static_assert(decode({98, 128, 98}) == "bbbb");
static_assert(decode({97, 98, 129, 98, 100, 132, 132}) == "abbbbddddd");
static_assert(decode({97, 97, 98, 98, 129, 99, 100, 101, 102}) ==
              "aabbabcdef");
static_assert(decode({48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 128, 130,
                      132, 134, 136}) == "01234567890123456789");
static_assert(decode({48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 128, 130,
                      132, 134, 136, 48}) == "012345678901234567890");
static_assert(decode({97, 98, 99, 128, 99}) == "abcabc");

static_assert(decode({97, 98, 99, 128, 130}) == "abcabca");
static_assert(decode({97, 98, 99, 128, 99, 98}) == "abcabcb");
static_assert(decode({97, 98, 99, 128, 130, 129}) == "abcabcabc");
static_assert(decode({48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 128, 130,
                      132, 134, 136, 138}) == "01234567890123456789012");
static_assert(decode({48,  49,  50,  51,  52,  53,  54,  55,
                      56,  57,  128, 130, 132, 134, 136, 138,
                      131, 133, 135, 137, 129, 144, 141, 57}) ==
              "0123456789012345678901234567890123456789");
static_assert(decode({48,  49,  50,  51,  52,  53,  54,  55,  56,
                      57,  128, 130, 132, 134, 136, 138, 131, 133,
                      135, 137, 129, 144, 141, 147, 139, 145, 136}) ==
              "01234567890123456789012345678901234567890123456789");
static_assert(decode({48,  49,  50,  51,  52,  53,  54,  55,  56,  57,
                      128, 130, 132, 134, 136, 138, 131, 133, 135, 137,
                      129, 144, 141, 147, 139, 145, 142, 148, 140, 146,
                      143, 156, 154, 152, 150, 158, 153, 151, 149, 146}) ==
              "01234567890123456789012345678901234567890123456789012345678"
              "90123456789012345678901234567890123456789");
static_assert(
    decode({76,  101, 109, 112, 101, 108, 45,  90,  105, 118, 45,  87,
            132, 99,  104, 32,  40,  76,  90,  87,  41,  32,  105, 115,
            32,  97,  32,  117, 110, 136, 101, 114, 115, 97,  108, 32,
            108, 111, 115, 115, 108, 101, 166, 32,  100, 97,  116, 153,
            99,  111, 130, 114, 169, 115, 105, 111, 110, 152, 108, 103,
            111, 114, 105, 116, 104, 109, 32,  99,  179, 173, 101, 100,
            32,  98,  121, 32,  65,  98,  114, 97,  104, 97,  193, 128,
            130, 132, 44,  32,  74,  97,  176, 98,  32,  135, 118, 214,
            97,  110, 199, 84,  158, 114, 202, 139, 108, 141, 46}) ==
    "Lempel-Ziv-Welch (LZW) is a universal lossless data compression "
    "algorithm created by Abraham Lempel, Jacob Ziv, and Terry "
    "Welch.");
static_assert(
    decode({73,  110, 32,  116, 104, 101, 32,  121, 101, 97,  114, 32,
            49,  56,  55,  56,  32,  73,  130, 111, 111, 107, 32,  109,
            121, 32,  100, 101, 103, 114, 101, 133, 111, 102, 32,  68,
            111, 99,  116, 111, 138, 160, 32,  77,  101, 100, 105, 99,
            105, 110, 159, 161, 131, 133, 85,  110, 105, 118, 101, 114,
            115, 105, 116, 152, 169, 76,  111, 110, 100, 194, 44,  32,
            97,  195, 32,  112, 114, 164, 158, 154, 100, 146, 32,  78,
            101, 116, 108, 101, 152, 166, 32,  103, 111, 130, 104, 204,
            117, 103, 104, 221, 133, 99,  111, 117, 187, 133, 203, 101,
            115, 99,  114, 105, 98,  172, 32,  102, 167, 32,  115, 231,
            103, 101, 194, 115, 32,  176, 227, 199, 114, 151, 46}) ==
    "In the year 1878 I took my degree of Doctor of Medicine of "
    "the University of London, and proceeded to Netley to go "
    "through the course prescribed for surgeons in the army.");
static_assert(decode({}) == "");
static_assert(decode({97, 128, 97}) == "aaaa");
static_assert(decode({97, 128, 128}) == "aaaaa");
static_assert(decode({97, 128, 129}) == "aaaaaa");
static_assert(decode({97, 128, 129, 128}) == "aaaaaaaa");
static_assert(decode({97, 128, 129, 129}) == "aaaaaaaaa");
static_assert(decode({97, 128, 129, 130}) == "aaaaaaaaaa");
static_assert(decode({97, 128, 129, 130, 131, 97}) == "aaaaaaaaaaaaaaaa");
static_assert(decode({97, 128, 129, 130, 131, 132, 133, 130}) ==
              "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
static_assert(
    decode({97, 128, 129, 130, 131, 132, 133, 134, 135, 136, 135}) ==
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "a");
static_assert(decode({97, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137,
                      138, 139, 140, 141, 142, 143, 133}) ==
              "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
              "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
              "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
static_assert(decode({97, 98, 99, 100, 101, 102, 103, 104, 105, 128, 130,
                      132, 134, 136, 129, 131, 133, 135}) ==
              "abcdefghiabcdefghiabcdefghi");
static_assert(decode({97,  98,  99,  100, 101, 102, 103,
                      104, 105, 128, 130, 132, 134, 136,
                      129, 131, 133, 135, 137, 143, 140}) ==
              "abcdefghiabcdefghiabcdefghiabcdefghi");
static_assert(decode({97,  98,  99,  100, 101, 102, 103, 104,
                      105, 128, 130, 132, 134, 136, 129, 131,
                      133, 135, 137, 143, 140, 97}) ==
              "abcdefghiabcdefghiabcdefghiabcdefghia");
static_assert(decode({97,  98,  99,  100, 101, 102, 103, 104,
                      105, 128, 130, 132, 134, 136, 129, 131,
                      133, 135, 137, 143, 140, 128}) ==
              "abcdefghiabcdefghiabcdefghiabcdefghiab");
static_assert(decode({97,  98,  99,  100, 101, 102, 103, 104,
                      105, 128, 130, 132, 134, 136, 129, 131,
                      133, 135, 137, 143, 140, 137}) ==
              "abcdefghiabcdefghiabcdefghiabcdefghiabc");
static_assert(decode({97, 98, 99, 128, 130, 129, 131, 131}) ==
              "abcabcabcabcabc");
static_assert(decode({97, 98, 99, 128, 130, 129, 131, 134}) ==
              "abcabcabcabcabca");
static_assert(decode({97, 98, 99, 128, 130, 129, 131, 131, 98}) ==
              "abcabcabcabcabcb");
static_assert(decode({97, 98, 99, 128, 130, 129, 131, 134, 98}) ==
              "abcabcabcabcabcab");
static_assert((decode({97, 98, 99, 128, 130, 129, 131, 134, 129}) ==
               "abcabcabcabcabcabc"));
static_assert((decode({97,  98,  99,  100, 101, 102, 103, 104,
                       105, 128, 130, 132, 134, 136, 129, 131,
                       133, 135, 137, 143, 140, 146}) ==
               "abcdefghiabcdefghiabcdefghiabcdefghiabcd"));
static_assert((decode({97,  98,  99,  100, 101, 102, 103, 104,
                       105, 128, 130, 132, 134, 136, 129, 131,
                       133, 135, 137, 143, 140, 146, 139, 135}) ==
               "abcdefghiabcdefghiabcdefghiabcdefghiabcdefghi"));
static_assert((decode({97,  98,  99,  100, 101, 102, 103, 104, 105,
                       128, 130, 132, 134, 136, 129, 131, 133, 135,
                       137, 143, 140, 146, 139, 145, 142, 150, 105}) ==
               "abcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghi"));
static_assert(
    (decode({97,  98,  99,  100, 101, 102, 103, 104, 105, 128, 130, 132,
             134, 136, 129, 131, 133, 135, 137, 143, 140, 146, 139, 145,
             142, 150, 141, 138, 144, 154, 147, 151, 155, 148, 152, 156,
             149, 163, 162, 161, 160, 159, 158, 157, 153, 164, 140}) ==
     "abcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcde"
     "fghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghiabcdefghia"
     "bcdefghiabcdefghiabcdefghi"));
static_assert(
    (decode({97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108,
             109, 110, 128, 130, 132, 134, 136, 138, 140, 142, 131, 133,
             135, 137, 139, 141, 129, 150, 145, 153, 148, 156, 144, 152,
             147, 155, 143, 151, 146, 154, 149, 162, 168, 160, 166, 158,
             164, 48,  170, 167, 159, 165, 157, 163, 169, 161, 179, 164,
             178, 175, 184, 174, 183, 173, 182, 172, 181, 171, 180, 188,
             192, 110, 177, 185, 189, 193, 197, 187, 203, 200, 199, 195,
             210, 198, 208, 211, 214, 213, 191, 195, 202, 218, 217, 194,
             222, 206, 190, 223, 207, 221, 228, 227, 226, 225, 205, 186,
             154, 220, 35,  97,  134, 115, 100, 107, 106, 104, 108, 97,
             119, 106, 64,  35,  74,  72,  97,  106, 100, 104, 98,  115,
             103, 97,  115, 104, 106, 134, 260, 263, 259, 254, 104, 107,
             268, 244, 271, 260, 244, 115, 107, 260, 243, 104, 259, 106,
             97,  271}) ==
     "abcdefghijklmnabcdefghijklmnabcdefghijklmnabcdefghijklmnabcdefghi"
     "jklm"
     "nabcdefghijklmnabcdefghijklmnabcdefghijklmn0abcdefghijklmnabcdefg"
     "hijk"
     "lmnabcdefghijklmnabcdefghijklmnabcdefghijklmnabcdefghijklmnabcdef"
     "ghij"
     "klmnabcdefghijklmn0abcdefghijklmnabcdefghijklmnabcdefghijklmnabcd"
     "efgh"
     "ijklmnabcdefghijklmnabcdefghijklmnabcdefghijklmnabcdefghijklmn0ab"
     "cdef"
     "ghijklmnabcdefghijklmnabcdefghijklmnabcdefghijklmnabcdefghijklmna"
     "bcde"
     "fghijklmnabcdefghijklmnabcdefghijklmn0abc#aghsdkjhlawj@#"
     "JHajdhbsgashjghgahjsgajhksgajhksgajhskgakjhsgjaks"));
static_assert((decode({99, 99, 98, 98, 129, 97, 97, 131, 128, 98, 134, 129,
                       99, 133, 133, 140}) == "ccbbcbaabcccbabcbcaaaaca"));
static_assert((decode({99,  99, 98,  98,  129, 97,  97,  131,
                       128, 98, 134, 129, 99,  133, 133, 140,
                       130, 97, 128, 128, 145, 133, 98}) ==
               "ccbbcbaabcccbabcbcaaaacabbaccccacaab"));
static_assert((decode({99,  99,  98,  98,  129, 97,  97,  131, 128,
                       98,  134, 129, 99,  133, 133, 140, 130, 97,
                       128, 128, 145, 133, 131, 144, 140}) ==
               "ccbbcbaabcccbabcbcaaaacabbaccccacaabcbbaca"));
static_assert((decode({99,  99,  98,  98,  129, 97,  97,  131, 128,
                       98,  134, 129, 99,  133, 133, 140, 130, 97,
                       128, 128, 145, 133, 131, 144, 140, 140, 145}) ==
               "ccbbcbaabcccbabcbcaaaacabbaccccacaabcbbacacaac"));
static_assert((decode({99,  99,  98,  98,  129, 97,  97,  131, 128,
                       98,  134, 129, 99,  133, 133, 140, 130, 97,
                       128, 128, 145, 133, 131, 144, 140, 140, 148}) ==
               "ccbbcbaabcccbabcbcaaaacabbaccccacaabcbbacacaaca"));
static_assert((decode({99,  99,  98,  98,  129, 97,  97,  131, 128, 98,
                       134, 129, 99,  133, 133, 140, 130, 97,  128, 128,
                       145, 133, 131, 144, 140, 140, 148, 98}) ==
               "ccbbcbaabcccbabcbcaaaacabbaccccacaabcbbacacaacab"));
static_assert((decode({99,  99,  98,  98,  129, 97,  97,  131, 128, 98,
                       134, 129, 99,  133, 133, 140, 130, 97,  128, 128,
                       145, 133, 131, 144, 140, 140, 148, 131}) ==
               "ccbbcbaabcccbabcbcaaaacabbaccccacaabcbbacacaacabc"));
static_assert(
    (decode({99,  99,  98,  98,  129, 97,  97,  131, 128, 98,  134, 129,
             99,  133, 133, 140, 130, 97,  128, 128, 145, 133, 131, 144,
             140, 140, 148, 150, 136, 149, 143, 130, 147, 134, 150, 131,
             154, 153, 148, 143, 145, 135, 137, 168, 139, 142}) ==
     "ccbbcbaabcccbabcbcaaaacabbaccccacaabcbbacacaacabcbccbaabcab"
     "bbccaabbcbbcacabcaaacacabacbccbaacbcbcaac"));
static_assert(
    decode({66,  73, 68, 69, 65,  74,  70,  68,  71, 75, 71, 72,
            73,  72, 75, 72, 68,  67,  134, 128, 72, 71, 68, 73,
            74,  72, 72, 74, 145, 69,  70,  132, 65, 71, 66, 70,
            142, 66, 68, 65, 75,  133, 73,  70,  73, 67, 65, 154,
            161, 75, 73, 65, 163, 67,  71,  71,  160}) ==
    "BIDEAJFDGKGHIHKHDCFDBIHGDIJHHJCFEFAJAGBFKHBDAKJFIFICAHJGBKIAFKCGG"
    "AG");
static_assert(
    decode({74,  70,  67,  65,  69,  74, 67, 75, 128, 75, 73,  73,
            67,  74,  133, 73,  65,  66, 65, 74, 71,  69, 71,  74,
            69,  72,  71,  66,  68,  68, 72, 75, 69,  69, 144, 67,
            160, 129, 145, 65,  75,  72, 73, 69, 145, 66, 143, 68,
            65,  68,  75,  134, 137, 71, 70, 70, 75,  66, 66}) ==
    "JFCAEJCKJFKIICJJCIABAJGEGJEHGBDDHKEEABCEEFCBAAKHIEBABIADADKCKKIGF"
    "FKBB");
static_assert(
    decode({122, 84,  88,  78,  67,  78,  73,  84,  94,  65,  115, 91,
            101, 77,  68,  74,  92,  76,  114, 97,  96,  86,  88,  81,
            66,  124, 81,  85,  124, 114, 67,  117, 90,  70,  97,  103,
            121, 66,  68,  91,  118, 113, 103, 78,  79,  68,  86,  94,
            76,  66,  114, 110, 85,  88,  94,  122, 105, 159, 71,  107,
            84,  122, 112, 89,  120, 106, 92,  67,  125, 69,  114, 68,
            106, 124, 122, 88,  69,  109, 91,  83,  91,  121, 103, 123,
            70,  106, 73,  100, 110, 115, 108, 116, 110, 90,  107, 94,
            118, 90,  126, 125, 82,  87,  118, 80,  103, 105, 109, 89,
            68,  88,  126, 110, 100, 70,  118, 88,  105, 105, 95,  113,
            85,  106, 70,  123, 84,  77,  85,  78,  77,  117, 72,  72,
            123, 80,  83,  74,  69,  80,  88,  79,  103, 86,  123, 78,
            85,  119, 91,  74,  68,  68,  103, 114, 103, 91,  110, 98,
            109, 70,  121, 75,  69,  125, 80,  102, 67,  69,  75,  82,
            88,  89,  66,  97,  110, 95,  72,  89,  96,  126, 124, 89,
            117, 118, 115, 101, 88,  86,  118, 294, 122, 106, 80,  126,
            74,  104, 98,  115, 85,  75,  124, 118, 65,  85,  92,  97,
            71,  92,  78,  87,  96,  158, 115, 71,  96,  100, 97,  124,
            124, 115, 83,  78,  102, 68,  113, 341, 124, 79,  71,  84,
            81,  86,  104, 69,  68,  84,  118, 73,  123, 65,  121, 82}) ==
    "zTXNCNIT^As[eMDJ\\Lra`VXQB|QU|rCuZFagyBD[vqgNODV^LBrnUX^"
    "ziuZGkTzpYxj\\C}ErDj|zXEm[S[yg{FjIdnsltnZk^vZ~}RWvPgimYDX~ndFvXii_"
    "qUjF{TMUNMuHH{PSJEPXOgV{NUw[JDDgrg[nbmFyKE}PfCEKRXYBan_HY`~|"
    "YuvseXVvKRzjP~JhbsUK|vAU\\aG\\NW`CusG`da||sSNfDqda|OGTQVhEDTvI{AyR");
static_assert(decode({100, 99, 101, 100, 100, 97,  131, 97,  98,
                      100, 98, 130, 101, 99,  141, 128, 100, 101,
                      97,  99, 137, 133, 98,  98,  129, 99,  149,
                      99,  98, 133, 140, 101, 98}) ==
              "dceddaddabdbedecccdcdeacdbadbbcecadbcbadeceb");
static_assert(decode({98,  97,  100, 99,  97,  101, 101, 98,  135, 98,
                      100, 100, 97,  129, 99,  130, 129, 135, 100, 98,
                      99,  98,  134, 99,  134, 140, 146, 133, 151, 100}) ==
              "badcaeebbbbddaadcdcadbbdbcbebcebaadbeeced");
static_assert(decode({99,  101, 99,  100, 100, 99,  98,  97,  97,
                      131, 101, 135, 98,  138, 98,  99,  136, 97,
                      98,  146, 133, 99,  133, 101, 147, 149, 133}) ==
              "cecddcbaaddeaabeabcadabbbcbccbebbccccb");
static_assert(decode({98, 100, 100, 99, 98, 98, 99, 99, 130, 100, 97, 137,
                      136, 100, 98, 138, 142, 101}) ==
              "bddcbbccdcdadadcddbadbae");
static_assert(decode({97, 101, 100, 100, 99, 100, 97, 100, 98, 130, 97,
                      136}) == "aeddcdadbddabd");
static_assert(decode({98, 101, 98, 100, 98, 98, 99, 133}) == "bebdbbcbc");
#endif

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t codes_size;
    cin >> codes_size;
    vector<uint32_t> codes(codes_size);
    for (uint32_t& code : codes) {
        cin >> code;
    }
    cout << decode(codes);
}
