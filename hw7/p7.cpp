#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <queue>
#include <string>
#include <string_view>

using namespace std;

struct node_t final {
    size_t c;
    size_t freq;
    size_t left;
    size_t right;
    constexpr node_t(size_t a_c, size_t a_freq, size_t a_left,
                     size_t a_right) noexcept
        : c(a_c), freq(a_freq), left(a_left), right(a_right) {}
};

static array<std::string, 256> char_code{};

static void fill_codes(size_t v, string& s, const vector<node_t>& nodes) {
    if (nodes[v].c) {
        char_code[nodes[v].c] = s;
        return;
    }

    s.push_back('0');
    fill_codes(nodes[v].left, s, nodes);
    s.back() = '1';
    fill_codes(nodes[v].right, s, nodes);
    s.pop_back();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    array<size_t, 256> freq{};
    for (char c : s) {
        freq[uint8_t(c)]++;
    }
    vector<node_t> nodes;
    nodes.reserve(1024);
    auto comp = [&](size_t lhs_index,
                    size_t rhs_index) constexpr noexcept {
        return nodes[lhs_index].freq > nodes[rhs_index].freq;
    };
    priority_queue<size_t, vector<size_t>, decltype(comp)> q(comp);
    size_t unique_cnt = 0;
    for (size_t c = 0, index = 0; c < 256; c++) {
        if (freq[c]) {
            unique_cnt++;
            nodes.emplace_back(c, freq[c], size_t(0), size_t(0));
            q.push(index++);
        }
    }
    assert(unique_cnt > 0);

    while (q.size() > 1) {
        size_t x = q.top();
        q.pop();
        size_t y = q.top();
        q.pop();
        nodes.emplace_back(size_t(0), nodes[x].freq + nodes[y].freq, x, y);
        q.push(nodes.size() - 1);
    }

    string ss;
    ss.reserve(s.size());
    fill_codes(q.top(), ss, nodes);

    
    for (size_t c = 0; c < 256; c++) {
        if (freq[c] && char_code[c].empty()) {
            assert(unique_cnt == 1);
            char_code[c] = "0";
        }
    }

    size_t total_size = 0;
    for (size_t c = 0; c < 256; c++) {
        total_size += char_code[c].size() * freq[c];
    }
    cout << unique_cnt << ' ' << total_size << '\n';

    for (size_t c = 0; c < 256; c++) {
        if (freq[c]) {
            cout << char(c) << ": " << char_code[c] << '\n';
        }
    }

    ss.clear();
    for (char c : s) {
        ss += char_code[uint8_t(c)];
    }
    cout << ss;
}
