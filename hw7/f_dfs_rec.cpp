#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

inline constexpr size_t kAlphabetLength       = 2;
inline constexpr size_t kNullNodeIndex        = 0;
inline constexpr size_t kFakePrerootNodeIndex = kNullNodeIndex + 1;
inline constexpr size_t kRootNodeIndex        = kFakePrerootNodeIndex + 1;
inline constexpr size_t kDefaultNodesCount    = 3;

namespace {

struct node_t {
    array<size_t, kAlphabetLength> edges{};
    size_t compressed_suffix_link{};
    bool is_terminal{};

    [[nodiscard]] constexpr size_t operator[](
        size_t index) const noexcept {
        return edges[index];
    }

    [[nodiscard]] constexpr size_t& operator[](size_t index) noexcept {
        return edges[index];
    }

    constexpr bool term_or_has_csl() const {
        return is_terminal || compressed_suffix_link != kRootNodeIndex;
    }
};

}  // namespace

static vector<node_t> nodes;

static void add(string_view s) {
    auto iter                 = s.begin();
    const auto iter_end       = s.end();
    size_t current_node_index = kRootNodeIndex;
    for (; iter != iter_end; ++iter) {
        size_t index = size_t(uint8_t(*iter)) - '0';
        assert(index <= 1);
        assert(current_node_index < nodes.size());
        size_t next_node_index = nodes[current_node_index][index];
        if (next_node_index != kNullNodeIndex) {
            current_node_index = next_node_index;
        } else {
            break;
        }
    }

    auto lasted_max_length = static_cast<size_t>(iter_end - iter);
    nodes.reserve(nodes.size() + lasted_max_length);
    for (size_t new_node_index = nodes.size(); iter != iter_end; ++iter) {
        size_t index = size_t(uint8_t(*iter)) - '0';
        assert(index <= 1);
        nodes.emplace_back();
        assert(current_node_index < nodes.size());
        nodes[current_node_index][index] = new_node_index;
        current_node_index               = new_node_index++;
    }
    assert(current_node_index < nodes.size());
    nodes[current_node_index].is_terminal = true;
}

static void ComputeLinksForNodeChildren(
    size_t node_index, size_t& queue_tail, std::vector<size_t>& bfs_queue,
    vector<size_t>& suffix_links) noexcept {
    node_t& node                  = nodes[node_index];
    const size_t node_suffix_link = suffix_links[node_index];
    for (size_t symbol_index = 0; symbol_index < kAlphabetLength;
         symbol_index++) {
        // const size_t child_link_v_index =
        //     nodes[node.suffix_link][symbol_index];
        const size_t child_link_v_index =
            nodes[node_suffix_link][symbol_index];
        const size_t child_index = node[symbol_index];
        if (child_index != kNullNodeIndex) {
            suffix_links[child_index] = child_link_v_index;
            // nodes[child_index].suffix_link = child_link_v_index;
            const size_t child_comp_sl =
                nodes[child_link_v_index].compressed_suffix_link;
            const bool is_terminal_or_root =
                nodes[child_link_v_index].is_terminal ||
                child_link_v_index == kRootNodeIndex;
            nodes[child_index].compressed_suffix_link =
                is_terminal_or_root ? child_link_v_index : child_comp_sl;
            bfs_queue[queue_tail++] = child_index;
        } else {
            node[symbol_index] = child_link_v_index;
        }
    }
}

static void build() {
    vector<size_t> suffix_links(nodes.size());
    suffix_links[kRootNodeIndex] = kFakePrerootNodeIndex;
    // nodes[kRootNodeIndex].suffix_link            =
    // kFakePrerootNodeIndex;
    nodes[kRootNodeIndex].compressed_suffix_link = kRootNodeIndex;
    nodes[kFakePrerootNodeIndex].edges.fill(kRootNodeIndex);

    vector<size_t> bfs_queue(nodes.size());
    size_t queue_head       = 0;
    size_t queue_tail       = 0;
    bfs_queue[queue_tail++] = kRootNodeIndex;
    do {
        size_t node_index = bfs_queue[queue_head++];
        ComputeLinksForNodeChildren(node_index, queue_tail, bfs_queue,
                                    suffix_links);
    } while (queue_head < queue_tail);
}

enum class Status : uint32_t {
    kNotVisited  = 0,
    kCurrentlyIn = 1,
    kLeft        = 2
};
static vector<Status> visited;

static bool find_clear_cycle(size_t node_index) {
    visited[node_index] = Status::kCurrentlyIn;

    const size_t left_index  = nodes[node_index].edges[0];
    const size_t right_index = nodes[node_index].edges[1];
    const node_t left        = nodes[left_index];
    const node_t right       = nodes[right_index];
    if (visited[left_index] == Status::kCurrentlyIn &&
        !left.term_or_has_csl()) [[unlikely]] {
        return true;
    }
    if (visited[right_index] == Status::kCurrentlyIn &&
        !right.term_or_has_csl()) [[unlikely]] {
        return true;
    }
    if (!left.term_or_has_csl() &&
        visited[left_index] == Status::kNotVisited &&
        find_clear_cycle(left_index)) [[unlikely]] {
        return true;
    }
    if (!right.term_or_has_csl() &&
        visited[right_index] == Status::kNotVisited &&
        find_clear_cycle(right_index)) [[unlikely]] {
        return true;
    }

    visited[node_index] = Status::kLeft;
    return false;
}

#include <chrono>

namespace chrono = std::chrono;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const auto start = chrono::high_resolution_clock::now();
    nodes.reserve(1024);
    nodes.resize(kDefaultNodesCount);
    size_t n;
    cin >> n;
    for (string s; n > 0; n--) {
        cin >> s;
        add(s);
    }
    build();
    visited.resize(nodes.size(), Status::kNotVisited);
    const auto end = chrono::high_resolution_clock::now();
    const auto millis =
        chrono::duration_cast<chrono::milliseconds>(end - start);
    if (millis >= chrono::milliseconds(250)) [[unlikely]] {
#ifndef __VSCODE_LOCAL__
        throw runtime_error("time");
#endif
    }

    constexpr string_view kYes("TAK");
    constexpr string_view kNo("NIE");
    cout << (find_clear_cycle(kRootNodeIndex) ? kYes : kNo);
}
