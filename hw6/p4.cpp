#include <bits/stdc++.h>

using namespace std;

#define int         long long
#define uint        unsigned int
#define fr(i, s, e) for (int i = s; i < e; i++)
const uint inf = ((1ull << 63) - 1);

uint dist[501][501] = {};

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, m;
    cin >> n >> m;
    fr(i, 0, n) {
        fr(j, 0, n) {
            dist[i][j] = inf;
        }
    }
    fr(i, 0, n) {
        dist[i][i] = 0;
    }
    while (m--) {
        int u, v;
        uint ww;
        cin >> u >> v >> ww;
        dist[u][v] = ww;
    }

    fr(k, 0, n)
        fr(i, 0, n)
            fr(j, 0, n)
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

    fr (i, 0, n)
        fr (j, 0, n)
            if (i != j)
                cout << i << ' ' << j << ' ' << (dist[i][j] != inf ? (int)dist[i][j] : -1) << '\n';

    return 0;
}
