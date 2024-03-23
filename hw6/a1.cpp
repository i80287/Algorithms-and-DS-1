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

class Graph {
public:
    Graph();

    Graph(const vector<vector<vertex_t>>& weights);

    static Graph from_edges_list(const vector<edge_t>& a_edges);

    // Can be implemented using bfs or dfs
    bool is_connected() const;

    // Can be implemented using dfs
    bool has_cycle() const;

    // Can be implemented using dfs
    vector<vertex_t> find_any_cycle() const;

    void sort_edges_descending();

    bool has_edge(vertex_t u, vertex_t v) const;

    void add_edge(edge_t e, weight_t w);

    void delete_edge(edge_t e);

    weight_t set_edge_weight(edge_t t, weight_t new_weight);

    weight_t get_edge_weight(edge_t t) const;

    std::size_t vertices_size() const;

    std::size_t edges_size() const;

    const vector<edge_t>& get_edges() const;
};

Graph alg1(Graph& g) {
    g.sort_edges_descending();
    Graph t = Graph::from_edges_list(g.get_edges());

    for (edge_t e : g.get_edges()) {
        t.delete_edge(e);
        if (!t.is_connected()) {
            t.add_edge(e, g.get_edge_weight(e));
        }
    }

    return t;
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
        edge_t max_e{};
        weight_t max_e_weight = std::numeric_limits<weight_t>::min();
        for (size_t i = 1; i < cycle.size(); i++) {
            edge_t e_i(cycle[i - 1], cycle[i]);
            weight_t e_i_weight = g.get_edge_weight(e_i);
            if (e_i_weight > max_e_weight) {
                max_e = e_i;
                max_e_weight = e_i_weight;
            }
        }

        if (max_e_weight == std::numeric_limits<weight_t>::min()) {
            t.delete_edge(max_e);
        }
    }

    return t;
}
