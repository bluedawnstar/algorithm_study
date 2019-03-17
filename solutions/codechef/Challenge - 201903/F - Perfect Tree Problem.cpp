#include <memory.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cctype>
#include <cstring>
#include <climits>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include <numeric>
#include <limits>
#include <functional>
#include <tuple>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

#define PROFILE_START(i)    clock_t start##i = clock()
#define PROFILE_STOP(i)     fprintf(stderr, "elapsed time (" #i ") = %f\n", double(clock() - start##i) / CLOCKS_PER_SEC)

typedef long long           ll;
typedef unsigned long long  ull;

struct PerfectTreeLevel {
    int N;
    vector<vector<int>> edges;

    vector<int> parent;
    vector<int> size;
    vector<int> depth;
    int maxDepth;

    void build(const vector<vector<int>>& edges) {
        N = int(edges.size());
        this->edges = edges;

        parent = vector<int>(N);
        size = vector<int>(N);
        depth = vector<int>(N);
        visited = vector<bool>(N);

        maxDepth = 0;
        dfsBuild(0, -1, 0);

        levelSize = vector<vector<pair<int, int>>>(N);
        levelDelta = vector<int>(N);

        mapN = 1;
        maps = vector<map<int,int>>(N);
    }

    const vector<pair<int,int>>& calcLevelSize(int u) {
        if (depth[u] == maxDepth)
            return levelSize[u];

        if (!levelSize[u].empty())
            return levelSize[u];

        dfsD(u, parent[u]);

        int d = depth[u] + 1;
        int n = (u == 0) ? int(edges[u].size()) : int(edges[u].size()) - 1;

        auto& M = maps[levelDelta[u]];
        levelSize[u].reserve(M.size() + 1);

        for (auto it : M) {
            levelSize[u].emplace_back(n, it.first - d);
            d = it.first;
            n += it.second;
        }

        levelSize[u].emplace_back(n, maxDepth + 1 - d);

        return levelSize[u];
    }

private:
    void dfsBuild(int u, int p, int d) {
        depth[u] = d;
        parent[u] = p;
        size[u] = 1;

        for (int v : edges[u]) {
            if (v != p) {
                dfsBuild(v, u, d + 1);
                size[u] += size[v];
            }
        }

        maxDepth = max(maxDepth, d);
    }

    //----

    vector<vector<pair<int,int>>> levelSize;
    vector<int> levelDelta;

    vector<bool> visited;

    int mapN;
    vector<map<int,int>> maps;

    void dfsD(int u, int parent) {
        if (visited[u])
            return;
        visited[u] = true;

        if (depth[u] == maxDepth) {
            levelDelta[u] = 0;
            return;
        }

        int childN = (u == 0) ? int(edges[u].size()) : int(edges[u].size()) - 1;

        if (childN == 1) {
            int v = (edges[u][0] != parent) ? edges[u][0] : edges[u][1];
            if (edges[v].size() <= 2) {
                dfsD(v, u);
                levelDelta[u] = levelDelta[v];
                return;
            }
        }

        levelDelta[u] = mapN++;
        auto& M = maps[levelDelta[u]];

        for (auto v : edges[u]) {
            if (v != parent) {
                dfsD(v, u);

                if (M.empty()) {
                    M = maps[levelDelta[v]];
                } else {
                    for (auto it : maps[levelDelta[v]])
                        M[it.first] += it.second;
                }

                if (edges[v].size() > 2)
                    M[depth[v] + 1] += int(edges[v].size()) - 2;
            }
        }
    }
};

//---

#define MOD     1000000007

#define MAXN    200000
#define MAXQ    100000

int gN, gQ;
vector<vector<int>> gE;

int gAns = 0;

//---

// iterative version
inline int modPow(int x, int n) {
    if (n == 0)
        return 1;

    ll t = x;
    ll res = 1;
    for (; n > 0; n >>= 1) {
        if (n & 1)
            res = res * t % MOD;
        t = t * t % MOD;
    }
    return int(res);
}

inline int modInvIter(int a) {
    int b = MOD;
    int y = 0, x = 1;

    while (a > 1 && b != 0) {
        int q = a / b;

        int t = b;
        b = a % b;
        a = t;

        t = y;
        y = x - q * y;
        x = t;
    }

    return (x % MOD + MOD) % MOD;
}

// M is a prime number.
inline int modInvPrime(int a) {
    return modPow(a, MOD - 2);
}

PerfectTreeLevel gTreeLevel;

// m = cnt
int calcBlock(int d, int n, int m, int p) {
    if (p == 1) {
        return 1ll * n * (1ll * (d + m - 1) * (d + m) / 2 - 1ll * (d - 1) * d / 2) % MOD;
    } else {
        int r = modPow(p, n);
        int rn = modPow(r, m);
        int res = (d - 1 - 1ll * (d + m - 1) * rn + 1ll * (rn - 1) * modInvPrime(r - 1)) % MOD;
        if (res < 0)
            res += MOD;
        return res;
    }
}

int solve(int v, int y) {
    --v;

    ll res = 0;

    if (y == 1) {
        auto& C = gTreeLevel.calcLevelSize(v);
        if (!C.empty()) {
            int d = 1;
            for (auto it : C) {
                int n = it.first;
                int m = it.second;
                res += (1ll * d * m * n + 1ll * n * m * (m - 1) / 2) % MOD;
                d += m;
            }
        }
        res %= MOD;
    } else {
        auto& C = gTreeLevel.calcLevelSize(v);
        if (!C.empty()) {
            int rr = modInvPrime(1 - y + MOD);
            ll p = 1;
            int d = 1;

            int prevN = 0;
            ll pn = 1;
            for (auto it : C) {
                int n = it.first;
                int m = it.second;
                if (n != prevN) {
                    pn = pn * modPow(y, n - prevN) % MOD;
                    prevN = n;
                }
                if (m < 30) {
                    for (int i = 0; i < m; i++) {
                        res += p * (MOD + 1 - pn) % MOD * d++;
                        p = p * pn % MOD;
                    }
                } else {
                    int pnm = modPow(y, n * m);
                    ll t = (d - 1 - 1ll * (d + m - 1) * pnm + 1ll * (pnm - 1) * modInvPrime(pn - 1)) % MOD;
                    if (t < 0)
                        t += MOD;
                    res += p * t % MOD;
                    p = p * pnm % MOD;
                    d += m;
                }
            }
            res = 1ll * res % MOD * (1ll * y * rr % MOD) % MOD;
        }
    }
    return int(res);
}

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    scanf("%d", &T);

    int prevN = 0;
    while (T-- > 0) {
        scanf("%d %d", &gN, &gQ);

        gE = vector<vector<int>>(gN);
        for (int i = 0; i < prevN; i++)
            gE[i].clear();

        for (int i = 1; i < gN; i++) {
            int u, v;
            scanf("%d %d", &u, &v);
            u--;
            v--;
            gE[u].push_back(v);
            gE[v].push_back(u);
        }

        gTreeLevel.build(gE);

        gAns = 0;
        for (int i = 0; i < gQ; i++) {
            int a, b;
            scanf("%d %d", &a, &b);
            int ans = solve(a ^ gAns, b ^ gAns);
            //int ans = solve(a, b);
            printf("%d\n", ans);
            gAns = ans;
        }
        prevN = gN;
    }

    return 0;
}
