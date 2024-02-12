#include <cstdint>
#include <iostream>
#include <vector>
#include <utility>
#include <cassert>

using std::cin, std::cout, std::vector;

inline constexpr size_t kMaxHeight = 20;

struct tree {
    struct node {
        int32_t value = 0;
        uint32_t left_index = uint32_t(-1);
        uint32_t right_index = uint32_t(-1);

        constexpr bool has_left() const noexcept {
            return left_index != uint32_t(-1);
        }

        constexpr bool has_right() const noexcept {
            return right_index != uint32_t(-1);
        }

        constexpr bool is_leaf() const noexcept {
            return !has_left() & !has_right();
        }
    };

    vector<node> nodes;

    bool insert(int32_t value) {
        if (nodes.empty()) {
            nodes.emplace_back().value = value;
            return true;
        }

        size_t height = size_t(-1);
        size_t prev_node_index = 0;
        for (size_t current_node_index = 0; current_node_index < nodes.size(); ) {
            int32_t node_value = nodes[current_node_index].value;
            uint32_t right_index = nodes[current_node_index].right_index;
            uint32_t left_index = nodes[current_node_index].left_index;
            assert(node_value != value);
            prev_node_index = current_node_index;
            current_node_index = node_value < value ? right_index : left_index;
            height++;
        }

        if (height > kMaxHeight) {
            assert(height != size_t(-1));
            return false;
        }

        size_t new_node_index = nodes.size();
        nodes.emplace_back().value = value;
        if (nodes.at(prev_node_index).value < value) {
            assert(nodes[prev_node_index].right_index == uint32_t(-1));
            nodes[prev_node_index].right_index = uint32_t(new_node_index);
        }
        else {
            assert(nodes[prev_node_index].left_index == uint32_t(-1));
            nodes[prev_node_index].left_index = uint32_t(new_node_index);
        }

        return true;
    }

    bool is_avl() const {
        return nodes.empty() || height_impl(0).second;
    }

    std::pair<int32_t, bool> height_impl(uint32_t node_index) const {
        assert(node_index < nodes.size());
        int32_t height_l = -1;
        bool b_l = true;
        if (nodes[node_index].has_left()) {
            auto ans = height_impl(nodes[node_index].left_index);
            height_l = ans.first;
            b_l = ans.second;
        }

        int32_t height_r = -1;
        bool b_r = true;
        if (nodes[node_index].has_right()) {
            auto ans = height_impl(nodes[node_index].right_index);
            height_r = ans.first;
            b_r = ans.second;
        }

        int32_t h = std::max(height_l, height_r) + 1;
        bool b = b_l && b_r && std::abs(height_l - height_r) <= 1;
        return { h, b };
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);

    tree t;
    t.nodes.reserve(size_t(1) << 21);
    while (cin) {
        int32_t a = 0;
        cin >> a;
        if (a == 0) {
            break;
        }

        if (!t.insert(a)) {
            cout << "NO\n";
            return 0;
        }
    }

    cout << (t.is_avl() ? "YES\n" : "NO\n");
}
