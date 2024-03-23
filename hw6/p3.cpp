#include <bits/stdc++.h>

using namespace std;

#define int long long
#define fr(i, s, e) for (int i = s; i < e; i++)
#define pii pair<int, int>
#define pb push_back

const int inf  = (1ll << 60) - 2;
const int maxn = 1e4;

// vector<vector<int>> w;
vector<int> dist;
// vector<pii> edges;
vector<vector<pii>> g;
vector<uint8_t> badv;

void dfs(int v) {
    badv[v] = true;
    for (auto [u, _] : g[v]) {
        if (!badv[u]) {
            dfs(u);
        }
    }
}

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, m;
    cin >> n >> m;
    g.resize(n);
    // w.resize(n, vector<int>(n));
    dist.resize(n);
    badv.resize(n);
    // fr(i, 0, n) {
    //     fr(j, 0, n) { w[i][j] = inf; }
    // }
    fr(i, 0, n) { dist[i] = inf; }
    dist[0] = 0;
    // edges.reserve(m);
    while (m--) {
        int u, v;
        cin >> u >> v;
        // cin >> w[u][v];
        int w;
        cin >> w;
        // edges.pb({u, v});
        g[u].pb({v, w});
    }

    fr(i, 0, n) {
        fr(u, 0, n) {
            for (auto [v, w] : g[u]) {
                dist[v] = min(dist[v], dist[u] + w);
            }
        }
    }

    // for (auto [u, v] : edges) {
    fr(u, 0, n) {
        for (auto [v, w] : g[u]) {
            if (dist[u] + w < dist[v]) {
                // cout << u << ' ' << v << '\n';
                dfs(u);
            }
        }
    }

    fr(v, 1, n) {
        if (badv[v]) {
            cout << "-inf\n";
        } else {
            cout << dist[v] << '\n';
        }
    }

    return 0;
}
