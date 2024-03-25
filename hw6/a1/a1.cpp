#include <algorithm>
#include <cstdint>
#include <numeric>
#include <random>
#include <utility>
#include <vector>
#include <limits>

using std::vector;

using vertex_t = std::int64_t;
using edge_t = std::pair<vertex_t, vertex_t>;
using weight_t = std::int64_t;

inline constexpr vertex_t kInvalidVertex = vertex_t(-1);
inline constexpr weight_t kInfWeight = std::numeric_limits<weight_t>::max() / 2;

namespace alg_1
{
    
} // namespace alg_1


class Graph {
public:
    Graph();

    static Graph from_edges_list(const vector<edge_t>& a_edges, const vector<vector<vertex_t>>& weights);

    // Can be implemented using bfs or dfs
    bool is_connected() const;

    // Can be implemented using dfs
    bool has_cycle() const;

    // Can be implemented using dfs
    vector<vertex_t> find_any_cycle() const;

    bool has_edge(vertex_t u, vertex_t v) const;

    void add_edge(edge_t e, weight_t w);

    void delete_edge(edge_t e);

    weight_t set_edge_weight(edge_t t, weight_t new_weight);

    weight_t get_edge_weight(edge_t t) const;

    std::size_t vertices_size() const;

    std::size_t edges_size() const;

    const vector<edge_t>& get_edges() const;

    const vector<vector<weight_t>>& get_weights() const;
private:
    vector<edge_t> edges;
    vector<vector<weight_t>> weights;
};

std::vector<edge_t> alg1(const Graph& g) {
    vector<edge_t> edges = g.get_edges();
    std::sort(edges.begin(), edges.end(), [&g](edge_t e1, edge_t e2) {
        return g.get_edge_weight(e1) > g.get_edge_weight(e2);
    });
    Graph t = Graph::from_edges_list(edges, g.get_weights());

    for (edge_t e : edges) {
        t.delete_edge(e);
        if (!t.is_connected()) {
            t.add_edge(e, g.get_edge_weight(e));
        }
    }

    return t.get_edges();
}

Graph alg2(const Graph& g) {
    Graph t;

    vector<std::size_t> random_edges_indexes(g.edges_size());
    std::iota(random_edges_indexes.begin(), random_edges_indexes.end(), 0zu);
    std::shuffle(random_edges_indexes.begin(), random_edges_indexes.end(), std::mt19937());

    for (size_t random_edge_index : random_edges_indexes) {
        edge_t e = g.get_edges()[random_edge_index];
        t.add_edge(e, g.get_edge_weight(e));
        if (t.has_cycle()) {
            t.delete_edge(e);
        }
    }

    return t;
}

Graph alg3(const Graph& g) {
    Graph t;

    vector<std::size_t> random_edges_indexes(g.edges_size());
    std::iota(random_edges_indexes.begin(), random_edges_indexes.end(), 0zu);
    std::shuffle(random_edges_indexes.begin(), random_edges_indexes.end(), std::mt19937());

    for (size_t random_edge_index : random_edges_indexes) {
        edge_t e = g.get_edges()[random_edge_index];
        t.add_edge(e, g.get_edge_weight(e));
        // May be empty
        vector<vertex_t> cycle = t.find_any_cycle();
        if (cycle.empty()) {
            continue;
        }

        edge_t e_max(cycle[0], cycle[1]);
        weight_t e_max_weight = g.get_edge_weight(e_max);
        for (size_t i = 2; i < cycle.size(); i++) {
            edge_t e_i(cycle[i - 1], cycle[i]);
            weight_t e_i_weight = g.get_edge_weight(e_i);
            if (e_i_weight > e_max_weight) {
                e_max = e_i;
                e_max_weight = e_i_weight;
            }
        }

        t.delete_edge(e_max);
    }

    return t;
}
