#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <string_view>

#if defined __VSCODE_LOCALGHOST__
#include "clipboardxx.hpp"
#endif

using namespace std;

using index_t                    = size_t;
using code_t                     = uint32_t;
constexpr index_t kNullNodeIndex = 0;
constexpr index_t kRootNodeIndex = kNullNodeIndex + 1;

struct node_t final {
    std::map<uint8_t, index_t> edges{};
    code_t code{};
};

static void dfs(index_t u, string& ss, const vector<node_t>& nodes) {
    if (nodes[u].code >= 128) {
        cout << ss << " -> " << nodes[u].code << '\n';
    }
    for (auto [chr, v] : nodes[u].edges) {
        ss.push_back(char(chr));
        dfs(v, ss, nodes);
        ss.pop_back();
    }
}

static vector<code_t> encode(string_view s) {
    assert(!s.empty());
    vector<node_t> nodes;
    code_t free_code        = 0;
    index_t free_node_index = kRootNodeIndex + 1;
    {
        nodes.reserve(1024);
        nodes.resize(kRootNodeIndex + 1);
        assert(kNullNodeIndex < nodes.size());
        assert(kRootNodeIndex < nodes.size());
        auto& root_edges = nodes[kRootNodeIndex].edges;
        for (uint8_t c = 0; c < 128; c++) {
            auto [_, inserted] = root_edges.emplace(c, free_node_index);
            assert(inserted);
            free_node_index++;
            nodes.emplace_back().code = free_code++;
        }
    }

    index_t current_node_index = kRootNodeIndex;
    vector<code_t> codes;
    codes.reserve(s.size() / 3);
    for (char c : s) {
        uint8_t chr = uint8_t(c);
        assert(current_node_index < nodes.size());
        auto [iter, inserted] = nodes[current_node_index]
            .edges
            .try_emplace(chr, free_node_index);
        if (!inserted) {
            current_node_index = iter->second;
            continue;
        }

        nodes.emplace_back().code = free_code++;
        free_node_index++;
        codes.push_back(nodes[current_node_index].code);
        current_node_index = kRootNodeIndex + 1 + chr;
    }

    assert(current_node_index < nodes.size());
    codes.push_back(nodes[current_node_index].code);

    // { string ss; dfs(kRootNodeIndex, ss, nodes); }

    return codes;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    s.reserve(size_t(2e4));
    s = "abcd";
    // std::getline(std::cin, s);
    const auto codes = encode(s);
    cout << codes.size() << '\n';
    for (auto code : codes) {
        cout << code << ' ';
    }

    string ans = "static_assert(decode({";
    ans.reserve(size_t(1e3));
    size_t i = 0;
    for (auto code : codes) {
        ans += to_string(code);
        if (++i != codes.size()) {
            ans += ", ";
        }
    }
    ans += "}) == \"";
    ans += s;
    ans += "\");";
    clipboardxx::clipboard() << ans;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1000ms);
}
