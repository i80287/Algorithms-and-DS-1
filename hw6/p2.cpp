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

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace kosaraju_algorithm {

using std::vector;

// Min vertex is 0, max is g.size() - 1
using vertex_t = std::size_t;
using graph_t  = vector<vector<vertex_t>>;

namespace impl {

static void fill_stack_dfs(const graph_t& g, vector<uint8_t>& visited,
                           vector<vertex_t>& order_stack, vertex_t v) noexcept {
    visited[v] = true;
    for (vertex_t u : g[v]) {
        if (!visited[u]) {
            fill_stack_dfs(g, visited, order_stack, u);
        }
    }
    assert(order_stack.capacity() >= g.size());
    order_stack.push_back(v);
}

static void form_component_dfs(const graph_t& tr_g, vector<uint8_t>& visited,
                               vector<vertex_t>& component, vertex_t v) noexcept {
    visited[v] = true;
    assert(component.capacity() >= tr_g.size());
    component.push_back(v);
    for (vertex_t u : tr_g[v]) {
        if (!visited[u]) {
            form_component_dfs(tr_g, visited, component, u);
        }
    }
}

}  // namespace impl

template <bool ShrinkComponents = false>
graph_t strongly_connected_components(const graph_t& g) {
    graph_t components;
    const auto n = g.size();

    vector<std::uint8_t> visited(n, false);
    vector<vertex_t> order_stack;
    order_stack.reserve(n);
    for (vertex_t v = 0; v < n; v++) {
        if (!visited[v]) {
            impl::fill_stack_dfs(g, visited, order_stack, v);
        }
    }
    assert(order_stack.size() == n);
    std::fill(visited.begin(), visited.end(), false);

    graph_t transposed_g(n);

    std::vector<vertex_t> current_component;
    current_component.reserve(n);

    for (vertex_t v = 0; v < n; v++) {
        for (vertex_t u : g[v]) {
            transposed_g[u].push_back(v);
        }
    }

    for (auto iter = order_stack.crbegin(), end = order_stack.crend(); iter != end;
         ++iter) {
        vertex_t v = *iter;
        if (!visited[v]) {
            impl::form_component_dfs(transposed_g, visited, current_component, v);
            // Use O(E) memory instead of O(V^2)
            std::vector<vertex_t>& new_component =
                components.emplace_back(current_component.size());
            std::copy(current_component.cbegin(), current_component.cend(),
                      new_component.begin());
            current_component.clear();
        }
    }

    return components;
}

}  // namespace kosaraju_algorithm

using std::set;
using std::array;
using std::vector, std::cin, std::cout, std::pair, std::min, std::max;
using i64 = int64_t;
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
    constexpr DisjointSetUnion(DisjointSetUnion&& other) noexcept
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

static array<array<weight_t, 100>, 100> weights = {};

using u64 = unsigned long long;

template <class Comp>
static vector<edge_t> kruskal_mst(const set<edge_t, Comp>& edges, size_t n, weight_t& w) {
    DisjointSetUnion dsu(n);
    vector<edge_t> mst;
    mst.reserve(n - 1);
    for (auto [u, v] : edges) {
        if (!dsu.equal(u, v)) {
            mst.push_back({u, v});
            w += weights[u][v];
            dsu.unite(u, v);
        }
    }
    return mst;
}

static vector<vertex_t> prim_mst(const graph_t& g) {
    const size_t n = g.size();
    vector<uint8_t> in_queue(n, true);
    vector<vertex_t> prnt(n, vertex_t(-1));
    vector<weight_t> key(n, weight_t(-1));
    key[0] = 0;
    set<pair<weight_t, vertex_t>> q;
    for (vertex_t v = 0; v < n; v++) {
        auto [_, inserted] = q.insert({key[v], v});
        assert(inserted);
    }
    assert(q.size() == n);

    do {
        auto min_iter = q.begin();
        vertex_t v = min_iter->second;
        assert(min_iter->first == key[v]);
        q.extract(min_iter);
        in_queue[v] = false;
        for (vertex_t u : g[v]) {
            assert(in_queue[u] == (q.find({key[u], u}) != q.end()));
            if (in_queue[u] && key[u] > weights[v][u]) {
                prnt[u] = v;
                auto node = q.extract({key[u], u});
                assert(!node.empty());
                assert(node.value().first == key[u]);
                assert(node.value().second == u);
                key[u] = weights[v][u];
                q.insert({key[u], u});
            }
        }
    } while (!q.empty());

    return prnt;
}

int main() {
    cin.tie(nullptr);
    std::ios::sync_with_stdio(false);

    vertex_t n, m;
    cin >> n >> m;
    assert(n <= 100);
    vector<edge_t> edges_list(m);
    graph_t g(n);
    for (size_t i = 0; i < m; i++) {
        vertex_t u, v, c;
        cin >> u >> v >> c;
        u--;
        v--;
        assert(u < n);
        assert(v < n);
        weights[u][v] = c;
        weights[v][u] = c;
        edges_list[i] = {min(u, v), max(u, v)};
        g[u].push_back(v);
        g[v].push_back(u);
    }

    weight_t min1      = 0;
    weight_t min2      = weight_t(-1);

    // constexpr auto comp = [](edge_t e1, edge_t e2) constexpr {
    //     return weights[e1.first][e1.second] < weights[e2.first][e2.second];
    // };
    // set<edge_t, decltype(comp)> sorted_edges(comp);
    // for (auto [u, v] : edges_list) {
    //     sorted_edges.insert({u, v});
    // }
    // vector<edge_t> mst = kruskal_mst(sorted_edges, n, min1);
    // assert(mst.size() + 1 == n);
    // for (auto [m_u, m_v] : mst) {
    //     auto r = sorted_edges.erase({m_u, m_v});
    //     g.at(m_u).erase(std::find(g[m_u].cbegin(), g[m_u].cend(), m_v));
    //     g.at(m_v).erase(std::find(g[m_v].cbegin(), g[m_v].cend(), m_u));
    //     // assert(r);
        
    //     if (kosaraju_algorithm::strongly_connected_components(g).size() == 1) {
    //         weight_t loc_min2 = 0;
    //         vector<edge_t> mst1 = kruskal_mst(sorted_edges, n, loc_min2);
    //         if (mst1.size() + 1 == n && loc_min2 < min2) {
    //             min2 = loc_min2;
    //         }
    //     }

    //     sorted_edges.insert({m_u, m_v});
    //     g[m_u].push_back(m_v);
    //     g[m_v].push_back(m_u);
    // }
    
    vector<vertex_t> mst = prim_mst(g);
    vector<edge_t> mst_edges(n - 1);
    assert(mst[0] == vertex_t(-1));
    for (vertex_t v = 1; v < n; v++) {
        min1 += weights[v][mst[v]];
        mst_edges[v - 1] = {v, mst[v]};
    }

    for (auto [u, v] : mst_edges) {
        g.at(u).erase(std::find(g[u].cbegin(), g[u].cend(), v));
        g.at(v).erase(std::find(g[v].cbegin(), g[v].cend(), u));

        if (kosaraju_algorithm::strongly_connected_components(g).size() == 1) {
            vector<vertex_t> mst1 = prim_mst(g);
            assert(mst1[0] == vertex_t(-1));
            weight_t locmin2 = 0;
            for (vertex_t vv = 1; vv < n; vv++) {
                locmin2 += weights[vv].at(mst1[vv]);
            }
            min2 = min(min2, locmin2);
        }

        g[u].push_back(v);
        g[v].push_back(u);
    }

    assert(min1 <= min2);
    assert(min2 != weight_t(-1));
    cout << min1 << ' ' << min2 << '\n';
    return 0;
}
