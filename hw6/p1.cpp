#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

using std::vector;

// Min vertex is 0, max is g.size() - 1
using vertex_t = std::size_t;
using graph_t  = vector<vector<vertex_t>>;

namespace kosaraju_algorithm {

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
vector<vector<vertex_t>> strongly_connected_components(const graph_t& g) {
    vector<vector<vertex_t>> components;
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

using std::cin, std::cout, std::vector, std::ios;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    uint32_t n, m;
    cin >> n >> m;
    graph_t g(n);
    for (auto i = 0u; i < m; i++) {
        uint32_t u, v;
        cin >> u >> v;
        u--;
        v--;
        g[u].push_back(v);
    }
    auto components = kosaraju_algorithm::strongly_connected_components(g);

    uint32_t comp_ind = 1;
    vector<uint32_t> ans(n);
    for (const auto& component : components) {
        for (vertex_t v : component) {
            ans[v] = comp_ind;
        }
        comp_ind++;
    }
    cout << components.size() << '\n';
    for (uint32_t comp_ind_ : ans) {
        cout << comp_ind_ << ' ';
    }
}
