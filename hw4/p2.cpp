#include <cstdint>
#include <iostream>
#include <vector>
#include <utility>
#include <cassert>
#include <tuple>
#include <climits>

using std::cin, std::cout, std::vector;

inline constexpr uint32_t kNullIndex = uint32_t(-1);
inline constexpr int32_t kInvalidBHeight = -1;

struct tree {
    struct node {
        int32_t key = 0;
        bool red = false;
        uint32_t left_index = kNullIndex;
        uint32_t right_index = kNullIndex;

        constexpr bool has_left() const noexcept {
            return left_index != kNullIndex;
        }

        constexpr bool has_right() const noexcept {
            return right_index != kNullIndex;
        }

        constexpr bool is_leaf() const noexcept {
            return !has_left() && !has_right();
        }

        constexpr bool is_black() const noexcept {
            return !is_red();
        }

        constexpr bool is_red() const noexcept {
            return red;
        }
    };

    uint32_t root_index = 0;
    vector<node> nodes;

    bool is_rbtree() const {
        assert(!nodes.empty());
        if (nodes[root_index].is_red()) {
            return false;
        }

        auto [h, min, max] = impl(root_index);
        if (h == kInvalidBHeight) {
            assert(min == max && max == 0);
            return false;
        }

        const node& root = nodes[root_index];
        if (root.has_left()) {
            assert(min < root.key);
        }
        else {
            assert(min == root.key);
        }
        if (root.has_right()) {
            assert(root.key < max);
        }
        else {
            assert(root.key == max);
        }
        return true;
    }

    std::tuple<int32_t, int32_t, int32_t> impl(uint32_t node_index) const {
        assert(node_index < nodes.size());
        int32_t height_l = 1;
        int32_t height_r = 1;
        const node& cur_node = nodes[node_index];
        int32_t min_l = cur_node.key;
        int32_t max_r = cur_node.key;

        if (cur_node.has_left()) {
            if (nodes[cur_node.left_index].key >= cur_node.key ||
                (cur_node.is_red() && nodes[cur_node.left_index].is_red())) {
                return { kInvalidBHeight, 0, 0 };
            }

            auto [h, min, max] = impl(cur_node.left_index);
            assert(min <= max);
            if (h == kInvalidBHeight || max >= cur_node.key) {
                return { kInvalidBHeight, 0, 0 };
            }

            min_l = std::min(min_l, min);
            height_l = h;
        }

        if (cur_node.has_right()) {
            if (cur_node.key >= nodes[cur_node.right_index].key ||
                (cur_node.is_red() && nodes[cur_node.right_index].is_red())) {
                return { kInvalidBHeight, 0, 0 };
            }

            auto [h, min, max] = impl(nodes[node_index].right_index);
            assert(min <= max);
            if (h == kInvalidBHeight || cur_node.key >= min) {
                return { kInvalidBHeight, 0, 0 };
            }

            max_r = std::max(max_r, max);
            height_r = h;
        }

        assert(min_l <= max_r);
        if (height_l == height_r) {
            return { height_l + cur_node.is_black(), min_l, max_r };
        }

        return { kInvalidBHeight, 0, 0 };
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);

    uint32_t n = 0, root_index = 0;
    cin >> n;
    if (n == 0) {
        cout << "NO";
        return 0;
    }

    cin >> root_index;
    root_index--;
    tree t;
    t.root_index = root_index;
    t.nodes.resize(n);
    std::string buffer;
    buffer.reserve(8);
    do {
        uint32_t index = 0;
        int32_t key = 0;
        cin >> index >> key;
        index--;
        cin >> buffer;
        uint32_t left_index = buffer[0] != 'n' ? uint32_t(atoi(buffer.c_str())) - 1 : kNullIndex;
        cin >> buffer;
        uint32_t right_index = buffer[0] != 'n' ? uint32_t(atoi(buffer.c_str())) - 1 : kNullIndex;
        cin.ignore();
        char color_c = char(cin.get());
        t.nodes.at(index).key = key;
        t.nodes[index].red = color_c == 'R';
        t.nodes[index].left_index = left_index;
        t.nodes[index].right_index = right_index;
    } while (--n);
    cout << (t.is_rbtree() ? "YES" : "NO");
}
