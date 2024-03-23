#include <algorithm>
#include <cinttypes>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <tuple>
#include <utility>
#include <valarray>
#include <vector>

using namespace std;

using i32  = int32_t;
using u32  = uint32_t;
using i64  = long long;
using u64  = unsigned long long;
using i128 = __int128_t;
using u128 = __uint128_t;
using vint = vector<i64>;

#define fr(i, s, e) for (int i = s; i < e; i++)
#define all(v)      v.begin(), v.end()

const i64 inf  = (1ll << 60) - 2;
const i64 maxn = 2500;

signed main() {
    u32 n, m;
    scanf("%u %u", &n, &m);
    vector<vector<i32>> cap(n, vector<i32>(n));
    vector<vector<i32>> flow(n, vector<i32>(n));
    while (m--) {
        u32 u, v, w;
        scanf("%u %u %u", &u, &v, &w);
        u--;
        v--;
        cap[u][v] = i32(w);
    }

    vector<u64> q(n);
    vector<u64> parent(n);
    for (;;) {
        fill(all(parent), u64(-1));
        u32 h = 0, t = 0;
        q[t++]    = 0;
        parent[0] = 0;
        while (h < t) {
            u64 u = q[h++];
            for (u64 v = 0; v < n; v++) {
                if (parent[v] == u64(-1) && flow[u][v] < cap[u][v]) {
                    q[t++]    = v;
                    parent[v] = u;
                }
            }
        }

        if (parent[n - 1] == u64(-1)) {
            break;
        }

        i32 curflow = INT32_MAX;
        for (u64 v = n - 1; v > 0;) {
            u64 p   = parent[v];
            curflow = min(curflow, cap[p][v] - flow[p][v]);
            v       = p;
        }
        for (u64 v = n - 1; v > 0;) {
            u64 p = parent[v];
            flow[p][v] += curflow;
            flow[v][p] -= curflow;
            v = p;
        }
    }

    i64 maxflow = 0;
    for (u64 v = 0; v < n; v++) {
        if (cap[0][v] > 0) {
            maxflow += flow[0][v];
        }
    }
    printf("%" PRId64 "\n", maxflow);
    return 0;
}
