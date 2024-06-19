#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

#pragma GCC target("avx,fma,popcnt,tune=native")
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <map>
#include <numeric>
#include <valarray>

using namespace std;
using i64 = int64_t;
using u64 = uint64_t;
using v64 = vector<i64>;

class tree_t final {
public:
    struct node_t final {
        u64 parent_node_index = kNullNodeIndex;
        u64 left_node_index   = kNullNodeIndex;
        u64 right_node_index  = kNullNodeIndex;
        u64 node_index        = kNullNodeIndex;
        u64 value{};

        constexpr bool leaf() const noexcept {
            assert(node_index != kNullNodeIndex);
            return left_node_index == kNullNodeIndex && right_node_index == kNullNodeIndex;
        }
    };

    static tree_t from_istream(std::istream& in) {
        u64 n;
        cin >> n;
        tree_t t(n);
        for (u64 i = 1; i <= n; i++) {
            cin >> t.nodes_[i].value;
        }
        vector<adjacent_nodes_t> adj_list(n + 1);
        for (u64 i = n - 1; i64(i) > 0; i--) {
            u64 v, u;
            in >> v >> u;
            adj_list[v].push_back(u);
            adj_list[u].push_back(v);
        }
        assert(adj_list[kNullNodeIndex].size() == 0);
        assert(adj_list[kRootNodeIndex].size() <= 2);
        init_from_adj_list(t.nodes_, adj_list);

        return t;
    }

    u64 solve() const {
        vector<vector<state_t>> dp(nodes_.size());
        dp[kNullNodeIndex].resize(1);
        solve(kRootNodeIndex, dp, nodes_);
        u64 min_ops = 1ll << 62;
        for (state_t st : dp[kRootNodeIndex]) {
            min_ops = min(min_ops, st.ops);
        }
        return min_ops;
    }

    constexpr bool empty() const noexcept {
        return nodes_.size() == kRootNodeIndex;
    }
    constexpr node_t& root() noexcept {
        assert(!empty());
        return nodes_[kRootNodeIndex];
    }
    constexpr const node_t& root() const noexcept {
        assert(!empty());
        return nodes_[kRootNodeIndex];
    }

private:
    static constexpr u64 kNullNodeIndex = 0;
    static constexpr u64 kRootNodeIndex = 1;

    struct adjacent_nodes_t final {
        constexpr void push_back(u64 v) {
            if (size_ >= neighbours_.size()) {
                throw runtime_error(__PRETTY_FUNCTION__);
            }
            neighbours_[size_++] = v;
        }
        constexpr size_t size() const noexcept {
            if (size_ > neighbours_.size()) {
                __builtin_unreachable();
            }
            return size_;
        }
        constexpr u64 operator[](size_t index) const noexcept {
            assert(index < size());
            return neighbours_[index];
        }

        size_t size_ = 0;
        array<u64, 3> neighbours_{};
    };

    static void init_from_adj_list(vector<node_t>& nodes,
                                   const vector<adjacent_nodes_t>& adj_list) {
        init_from_adj_list(kNullNodeIndex, kRootNodeIndex, nodes, adj_list);
    }

    static void init_from_adj_list(u64 parent_node_index, u64 node_index, vector<node_t>& nodes,
                                   const vector<adjacent_nodes_t>& adj_list) {
        assert(node_index < adj_list.size());
        const adjacent_nodes_t& neighbours = adj_list[node_index];
        u64 children[2]                    = {kNullNodeIndex, kNullNodeIndex};
        u64 children_size                  = 0;
        for (size_t i = 0; i < neighbours.size(); i++) {
            if (neighbours[i] != parent_node_index) {
                assert(children_size < 2);
                children[children_size++] = neighbours[i];
            }
        }

        assert(node_index < nodes.size());
        node_t& node           = nodes[node_index];
        node.node_index        = node_index;
        node.parent_node_index = parent_node_index;
        if (children_size > 2) {
            __builtin_unreachable();
        }
        if (children_size == 0) {
            return;
        }
        node.left_node_index  = children[0];
        node.right_node_index = children[1];
        for (size_t i = 0; i < children_size; i++) {
            const auto child_index = children[i];
            init_from_adj_list(node_index, child_index, nodes, adj_list);
        }
    }

    struct state_t final {
        u64 value = 0;
        u64 ops   = 0;
    };

    static void solve(u64 node_index, vector<vector<state_t>>& dp, const vector<node_t>& nodes) {
        if (node_index == kNullNodeIndex) {
            return;
        }

        const node_t& node    = nodes[node_index];
        const auto node_value = node.value;

        if (node.leaf()) {
            dp[node.node_index].resize(2);
            dp[node.node_index][0] = state_t{
                .value = 0,
                .ops   = node_value,
            };
            dp[node.node_index][1] = state_t{
                .value = 1,
                .ops   = u64(abs(i64(node_value - 1))),
            };
            return;
        }

        solve(node.left_node_index, dp, nodes);
        solve(node.right_node_index, dp, nodes);

        const auto& left_node_valid_states  = dp[node.left_node_index];
        const auto& right_node_valid_states = dp[node.right_node_index];
        vector<state_t>& node_states        = dp[node.node_index];
        assert(node_states.empty());
        assert(!left_node_valid_states.empty());
        assert(!right_node_valid_states.empty());

        // ?=> unordered_map: {value} -> {set{ops}}

        node_states.resize(left_node_valid_states.size() * right_node_valid_states.size());
        size_t i = 0;
        for (state_t l_st : left_node_valid_states) {
            for (state_t r_st : right_node_valid_states) {
                auto sum         = l_st.value + r_st.value;
                auto cost        = u64(abs(i64(node_value - sum))) + l_st.ops + r_st.ops;
                node_states[i++] = state_t{
                    .value = sum,
                    .ops   = cost,
                };
            }
        }

        constexpr auto st_cmp = [](const state_t& s1, const state_t& s2) constexpr noexcept {
            return s1.ops < s2.ops;
        };
        if (node_states.size() > 64) {
            partial_sort(node_states.begin(), node_states.begin() + 64, node_states.end(), st_cmp);
            node_states.resize(64);
            node_states.shrink_to_fit();
        } else {
            sort(node_states.begin(), node_states.end(), st_cmp);
        }
    }

    tree_t(u64 n) : nodes_(n + 1) {}
    vector<node_t> nodes_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << tree_t::from_istream(cin).solve();
}
