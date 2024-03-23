#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <cinttypes>

using namespace std;

#define all(v) v.begin(),v.end()
#define int long long
using pii = pair<int,int>;
using vbool = vector<uint8_t>;
using u64 = uint64_t;

class heap {
public:
    using vertex_t = u64;
    using weight_t = u64;

    static constexpr weight_t kInfDist  = weight_t(-1);
    static constexpr vertex_t kNoVertex = vertex_t(-1);

    struct node_t {
        weight_t dist;
        vertex_t vertex;
    };

    static constexpr heap from_graph(size_t graph_size, vertex_t start_vertex) {
        vector<node_t> heap_nodes;
        heap_nodes.reserve(graph_size);
        heap_nodes.resize(1);
        heap_nodes[0].dist   = 0;
        heap_nodes[0].vertex = start_vertex;
        return heap(move(heap_nodes));
    }

    constexpr void reserve(size_t size) { heap_nodes_.reserve(size); }

    constexpr size_t size() const noexcept { return heap_nodes_.size(); }

    constexpr bool empty() const noexcept { return heap_nodes_.empty(); }

    constexpr node_t& top() noexcept { return heap_nodes_.front(); }

    constexpr const node_t& top() const noexcept { return heap_nodes_.front(); }

    void push(weight_t v_dist, vertex_t vertex) {
        // push elem to the end of the heap and sift upper.

        size_t elem_index   = size();
        size_t parent_index = parentIndex(elem_index);
        heap_nodes_.emplace_back();
        while (elem_index != 0 && heap_nodes_[parent_index].dist > v_dist) {
            heap_nodes_[elem_index] = heap_nodes_[parent_index];
            elem_index              = parent_index;
            parent_index            = parentIndex(elem_index);
        }

        heap_nodes_[elem_index] = {v_dist, vertex};
    }

    void pop_top() noexcept {
        // Remove root, place last element on it's place
        // and return pyramide (heap) to the balanced state.
        assert(!empty());
        node_t sifting_elem = heap_nodes_.front() = heap_nodes_.back();
        size_t parent_index                       = 0;
        size_t son_index                          = leftSonIndex(parent_index);
        while (son_index + 1 < size()) {
            if (heap_nodes_[son_index].dist > heap_nodes_[son_index + 1].dist) {
                son_index++;
            }

            if (sifting_elem.dist <= heap_nodes_[son_index].dist) {
                break;
            }
            heap_nodes_[parent_index] = heap_nodes_[son_index];
            heap_nodes_[son_index]    = sifting_elem;
            parent_index              = son_index;
            son_index                 = leftSonIndex(parent_index);
        }

        heap_nodes_.pop_back();
    }

    constexpr void decrease_top_key(weight_t dist) noexcept { decrease_key(0, dist); }

    constexpr void decrease_key(size_t node_index, weight_t dist) noexcept {
        heap_nodes_[node_index].dist = dist;
        rebalance_heap(node_index);
    }

private:
    constexpr heap(vector<node_t>&& heap_nodes) noexcept
        : heap_nodes_(move(heap_nodes)) {}

    constexpr void rebalance_heap(size_t index) noexcept {
        size_t son_index = leftSonIndex(index);
        if (son_index >= size()) {
            return;
        }

        size_t parent_index = index;
        node_t sifting_elem = heap_nodes_[index];
        if (son_index + 1 != size() &&
            heap_nodes_[son_index].dist > heap_nodes_[son_index + 1].dist) {
            son_index++;
        }

        while (sifting_elem.dist > heap_nodes_[son_index].dist) {
            heap_nodes_[parent_index] = heap_nodes_[son_index];
            heap_nodes_[son_index]    = sifting_elem;

            parent_index = son_index;
            son_index    = leftSonIndex(son_index);
            if (son_index >= size()) {
                return;
            }

            if (son_index + 1 != size() &&
                heap_nodes_[son_index].dist > heap_nodes_[son_index + 1].dist) {
                son_index++;
            }
        }
    }

    static constexpr size_t parentIndex(size_t node_index) noexcept {
        return (node_index - 1) / 2;
    }

    static constexpr size_t leftSonIndex(size_t node_index) noexcept {
        return node_index * 2 | 1;
    }

    vector<node_t> heap_nodes_;
};

constexpr uint32_t digitssum(uint64_t n) noexcept {
    uint32_t s = 0;
    do {
        s += uint32_t(n % 10);
        n /= 10;
    } while (n);
    return s;
}

signed main() {
    u64 n, a, b;
    scanf("%llu %llu %llu", &n, &a, &b);
    vector<u64> ancestor(n + 1, heap::kNoVertex);
    vector<u64> dist(n + 1, heap::kInfDist);
    vector<u64> edges_path_len(n + 1, heap::kInfDist);
    dist[a] = 0;
    edges_path_len[a] = 0;
    vbool visited(n + 1);
    heap Q = heap::from_graph(n, a);
    do {
        u64 v_dist;
        u64 v;
        do {
            v_dist = Q.top().dist;
            v      = Q.top().vertex;
            Q.pop_top();
        } while (visited[v] && !Q.empty());
        assert(v - 1 < n);
        if (visited[v]) {
            break;
        }
        u64 tmp_v = v;
        do {
            for (u64 div = 10;; div *= 10) {
                u64 suffix = tmp_v % div;
                if (suffix != 0) {
                    u64 weight = digitssum(suffix);
                    u64 newpath_weight = v_dist + weight;

                    assert(suffix - 1 < v);
                    u64 u = v - suffix;
                    if (u != 0) {
                        assert(u - 1 < n);
                        if (dist[u] > newpath_weight || (dist[u] == newpath_weight && edges_path_len[u] > edges_path_len[v] + 1)) {
                            dist[u] = newpath_weight;
                            edges_path_len[u] = edges_path_len[v] + 1;
                            ancestor[u] = v;
                            Q.push(newpath_weight, u);
                        }
                    }

                    u = v + suffix;
                    if (u <= n) {
                        if (dist[u] > newpath_weight || (dist[u] == newpath_weight && edges_path_len[u] > edges_path_len[v] + 1)) {
                            dist[u] = newpath_weight;
                            edges_path_len[u] = edges_path_len[v] + 1;
                            ancestor[u] = v;
                            Q.push(newpath_weight, u);
                        }
                    }
                }
                if (div > v) {
                    break;
                }
            }

            tmp_v /= 10;
        } while (tmp_v);
        visited[v] = true;
    } while (!Q.empty());

    if (ancestor[b] == heap::kNoVertex) {
        puts((a != b) ? "-1" : "0\n0");
        return 0;
    }

    vector<u64> path;
    for (u64 v = b; v != a; v = ancestor[v]) {
        path.push_back(v);
    }
    path.push_back(a);
    reverse(all(path));
    printf("%llu\n%zu\n", dist[b], path.size() - 1);
    for (size_t i = 1; i < path.size(); i++) {
        printf("%+lld\n", int64_t(path[i] - path[i - 1]));
    }
}
