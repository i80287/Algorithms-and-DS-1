#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>
#include <utility>
#include <vector>
#include <numeric>

using std::cin;
using std::cout;
using std::max;
using std::min;
using std::pair;
using std::set;
using std::vector;
using std::iota;
using vertex_t = size_t;
using weight_t = uint64_t;
using graph_t  = vector<vector<vertex_t>>;
using edge_t   = pair<vertex_t, vertex_t>;

// O(log*(n)) [log* - iterated logarithm, log*(n) = log2(log2(log2(...log2(n),
// until log2 > 0))), log*(65536) = 5, log*(2^65536) = 6]
template <typename NodeType>
static NodeType* findRoot(NodeType* node) noexcept {
    NodeType* current_node = node;
    assert(current_node != nullptr);
    while (current_node->parent_ != nullptr) {
        assert(current_node != current_node->parent_);
        current_node = current_node->parent_;
        assert(current_node != nullptr);
    }

    // Now 'current_node' points to the root
    while (node != current_node) {
        NodeType* next = node->parent_;
        node->parent_  = current_node;
        node           = next;
    }

    return current_node;
}

/// @brief See also https://www.youtube.com/watch?v=KFcpDTpoixo
class DisjointSetUnion {
public:
    // Node with rank heuristic
    struct DSUTreeNode {
        DSUTreeNode* parent_;
        size_t rank_;
    };

    DSUTreeNode* nodes_ = nullptr;
    size_t nodes_count_ = 0;

    DisjointSetUnion() = delete;

    // O(n)
    explicit DisjointSetUnion(size_t nodes_count) : nodes_count_{nodes_count} {
        if (nodes_count) {
            nodes_ = static_cast<DSUTreeNode*>(operator new(sizeof(DSUTreeNode) *
                                                            nodes_count));
            std::memset(nodes_, 0, sizeof(DSUTreeNode) * nodes_count);
        }
    }

    // O(n)
    DisjointSetUnion(const DisjointSetUnion& other) : nodes_count_(other.nodes_count_) {
        const size_t count = nodes_count_;
        if (count == 0) {
            return;
        }

        DSUTreeNode* const this_first_node = nodes_ =
            static_cast<DSUTreeNode*>(operator new(sizeof(DSUTreeNode) * count));

        for (size_t i = 0; i < count; ++i) {
            const DSUTreeNode* other_i_node_parent = other.nodes_[i].parent_;
            size_t parent_offset =
                static_cast<size_t>(other_i_node_parent - other.nodes_);
            this_first_node[i].parent_ =
                other_i_node_parent ? this_first_node + parent_offset : nullptr;
            this_first_node[i].rank_ = other.nodes_[i].rank_;
        }
    }

    // O(n)
    DisjointSetUnion& operator=(const DisjointSetUnion& other) {
        return *this = DisjointSetUnion(other);
    }

    // O(1)
    DisjointSetUnion(DisjointSetUnion&& other) noexcept
        : nodes_(other.nodes_), nodes_count_(other.nodes_count_) {
        other.nodes_       = nullptr;
        other.nodes_count_ = 0;
    }

    // O(1)
    DisjointSetUnion& operator=(DisjointSetUnion&& other) noexcept {
        auto nodes         = other.nodes_;
        auto nodes_count   = other.nodes_count_;
        other.nodes_       = nullptr;
        other.nodes_count_ = 0;
        this->~DisjointSetUnion();
        nodes_       = nodes;
        nodes_count_ = nodes_count;
        return *this;
    }

    // O(log*(n))
    bool equal(size_t node_x_index, size_t node_y_index) noexcept {
        assert(node_x_index < nodes_count_ && node_y_index < nodes_count_);
        return findRoot(&nodes_[node_x_index]) == findRoot(&nodes_[node_y_index]);
    }

    // O(log*(n))
    void unite(size_t node_x_index, size_t node_y_index) noexcept {
        assert(node_x_index < nodes_count_ && node_y_index < nodes_count_);
        DSUTreeNode* node_x_root_ptr = findRoot(&nodes_[node_x_index]);
        DSUTreeNode* node_y_root_ptr = findRoot(&nodes_[node_y_index]);
        if (node_x_root_ptr == node_y_root_ptr) {
            // Do not unite already united nodes so that for each root node:
            // root_node->parent_ == nullptr
            return;
        }

        size_t node_x_root_rank = node_x_root_ptr->rank_;
        size_t node_y_root_rank = node_y_root_ptr->rank_;
        if (node_x_root_rank > node_y_root_rank) {
            node_y_root_ptr->parent_ = node_x_root_ptr;
        } else if (node_x_root_rank != node_y_root_rank) {
            // node_x_root_rank < node_y_root_rank
            node_x_root_ptr->parent_ = node_y_root_ptr;
        } else {
            // node_x_root_rank == node_y_root_rank
            node_x_root_ptr->parent_ = node_y_root_ptr;
            node_y_root_ptr->rank_++;
        }
    }

    void resetData() { std::memset(nodes_, 0, sizeof(DSUTreeNode) * nodes_count_); }

    // O(n)
    ~DisjointSetUnion() {
        operator delete(nodes_);
        nodes_ = nullptr;
    }
};

struct slowdsu {
    vector<uint32_t> arr;

    slowdsu(size_t n) : arr(n) {
        iota(arr.begin(), arr.end(), 0);
    }

    void unite(size_t x, size_t y) {
        uint32_t c1 = arr[x];
        uint32_t c2 = arr[y];
        if (c1 != c2)
            for (size_t i = 0; i < arr.size(); i++)
                if (arr[i] == c2)
                    arr[i] = c1;
    }

    bool equal(size_t x, size_t y) { return arr[x] == arr[y]; }
};

static weight_t weights[100][100] = {};

using u64 = unsigned long long;

template <class Comp>
static vector<edge_t> find_mst(const set<edge_t, Comp>& edges, size_t n, weight_t& w) {
    // DisjointSetUnion
    slowdsu dsu(n);
    vector<edge_t> mst;
    mst.reserve(n - 1);
    // for (auto [u, v] : edges) {
    for (auto it_v = edges.cbegin(), it_v_end = edges.cend(); it_v != it_v_end; ++it_v) {
        auto u = it_v->first;
        auto v = it_v->second;
        if (!dsu.equal(u, v)) {
            mst.push_back({u, v});
            w += weights[u][v];
            dsu.unite(u, v);
        }
    }
    return mst;
}

int main() {
    cin.tie(nullptr);
    std::ios::sync_with_stdio(false);

    vertex_t n, m;
    cin >> n >> m;
    assert(n <= 100);
    // constexpr
    auto comp = [](edge_t e1, edge_t e2)
    // constexpr
    { return weights[e1.first][e1.second] < weights[e2.first][e2.second]; };
    set<edge_t, decltype(comp)> edges(comp);
    for (auto i = 0u; i < m; i++) {
        vertex_t u, v, c;
        cin >> u >> v >> c;
        u--;
        v--;
        assert(u < n);
        assert(v < n);
        weights[u][v] = c;
        weights[v][u] = c;
        edges.emplace(min(u, v), max(u, v));
    }

    weight_t min1      = 0;
    weight_t min2      = weight_t(-1);
    vector<edge_t> mst = find_mst(edges, n, min1);
    // assert(mst.size() + 1 == n);

    // for (auto [m_u, m_v] : mst) {
    for (auto it = mst.cbegin(), it_end = mst.cend(); it != it_end; ++it) {
        auto m_u = it->first;
        auto m_v = it->second;
        // auto r =
        edges.erase({m_u, m_v});
        // assert(r);

        weight_t loc_min2 = 0;
        size_t treesize   = 0;
        // DisjointSetUnion
        slowdsu dsu(n);
        // for (auto [u, v] : edges) {
        for (auto it_v = edges.cbegin(), it_v_end = edges.cend(); it_v != it_v_end;
             ++it_v) {
            auto u = it_v->first;
            auto v = it_v->second;
            if (!dsu.equal(u, v)) {
                loc_min2 += weights[u][v];
                treesize++;
                dsu.unite(u, v);
            }
        }
        if (treesize + 1 == n) {
            min2 = min(min2, loc_min2);
        }

        edges.insert({m_u, m_v});
    }

    // assert(min1 <= min2);
    // assert(min2 != weight_t(-1);
    cout << min1 << ' ' << min2 << '\n';
}
