#define NDEBUG

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
    size_t edges[kAlphabetLength] = {};
    size_t compressed_suffix_link = {};
    bool is_terminal              = {};

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

[[maybe_unused]] static void add_1(vector<node_t>& nodes, string_view s) {
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

[[maybe_unused]] void add(vector<node_t>& nodes, string_view s) {
    size_t current_node_index = kRootNodeIndex;
    for (char c : s) {
        size_t index           = size_t(uint8_t(c)) - '0';
        size_t next_node_index = nodes[current_node_index][index];
        if (next_node_index != kNullNodeIndex) {
            current_node_index = next_node_index;
        } else {
            size_t new_node_index = nodes.size();
            nodes.emplace_back();
            nodes[current_node_index][index] = new_node_index;
            current_node_index               = new_node_index;
        }
    }
    nodes[current_node_index].is_terminal = true;
}

static void ComputeLinksForNodeChildren(
    size_t node_index, size_t& queue_tail, std::vector<size_t>& bfs_queue,
    vector<node_t>& nodes, vector<size_t>& suffix_links) noexcept {
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

static void build(vector<node_t>& nodes) {
    vector<size_t> suffix_links(nodes.size());
    suffix_links[kRootNodeIndex] = kFakePrerootNodeIndex;
    nodes[kRootNodeIndex].compressed_suffix_link = kRootNodeIndex;
    nodes[kFakePrerootNodeIndex].edges[0]        = kRootNodeIndex;
    nodes[kFakePrerootNodeIndex].edges[1]        = kRootNodeIndex;

    vector<size_t> bfs_queue(nodes.size());
    size_t queue_head       = 0;
    size_t queue_tail       = 0;
    bfs_queue[queue_tail++] = kRootNodeIndex;
    do {
        size_t node_index = bfs_queue[queue_head++];
        ComputeLinksForNodeChildren(node_index, queue_tail, bfs_queue,
                                    nodes, suffix_links);
    } while (queue_head < queue_tail);
}

static bool find_clear_cycle(const vector<node_t>& nodes) {
    enum class Status : uint32_t {
        kNotVisited  = 0,
        kCurrentlyIn = 1,
        kLeft        = 2
    };
    enum class State : size_t {
        kPreCycle   = 0,
        kFirstIter  = 1,
        kSecondIter = 2,
    };
    struct StackFrame final {
        size_t node_index;
        State state;
        constexpr StackFrame(size_t a_node_index, State a_state) noexcept
            : node_index(a_node_index), state(a_state) {}
    };

    vector<Status> visited(nodes.size(), Status::kNotVisited);
    vector<StackFrame> stack;
    stack.emplace_back(kRootNodeIndex, State::kPreCycle);
    while (!stack.empty()) {
        const auto [node_index, state] = stack.back();
        switch (state) {
            case State::kPreCycle: {
                visited[node_index]     = Status::kCurrentlyIn;
                const size_t left_index = nodes[node_index].edges[0];
                const node_t left_son   = nodes[left_index];
                stack.back().state      = State::kFirstIter;
                if (!left_son.term_or_has_csl()) {
                    switch (visited[left_index]) {
                        case Status::kNotVisited:
                            stack.emplace_back(left_index,
                                               State::kPreCycle);
                            break;
                        case Status::kCurrentlyIn:
                            return true;
                        case Status::kLeft:
                            break;
                        default:
                            __builtin_unreachable();
                            break;
                    }
                }
            } break;
            case State::kFirstIter: {
                const size_t right_index = nodes[node_index].edges[1];
                const node_t right_son   = nodes[right_index];
                stack.back().state       = State::kSecondIter;
                if (!right_son.term_or_has_csl()) {
                    switch (visited[right_index]) {
                        case Status::kNotVisited:
                            stack.emplace_back(right_index,
                                               State::kPreCycle);
                            break;
                        case Status::kCurrentlyIn:
                            return true;
                        case Status::kLeft:
                            break;
                        default:
                            __builtin_unreachable();
                            break;
                    }
                }
            } break;
            case State::kSecondIter: {
                visited[node_index] = Status::kLeft;
                stack.pop_back();
            } break;
        }
    }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<node_t> nodes;
    nodes.reserve(1024);
    nodes.resize(kDefaultNodesCount);
    size_t n;
    cin >> n;
    for (string s; n > 0; n--) {
        cin >> s;
        add(nodes, s);
    }
    build(nodes);
    constexpr string_view kYes("TAK");
    constexpr string_view kNo("NIE");
    cout << (find_clear_cycle(nodes) ? kYes : kNo);
}
