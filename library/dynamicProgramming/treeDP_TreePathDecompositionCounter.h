#pragma once

/*
  The number of decompositions of the tree into paths and the path from u to v that is one of the paths in the decomposition.
    https://www.hackerearth.com/problem/algorithm/path-decomposition-ii-ad4b4a0c/

  1. algorithm
     1) bottom-up DP
        dpUp[u][0] = A
        dpUp[u][1] = A + B

        A =        u                  u                  u                  u
               x  x  x  x     +   /  x  x  x     +   x  /  x  x     +   x  x  \  x     + ...
              v0 v1 v2 v3 ...    v0 v1 v2 v3 ...    v0 v1 v2 v3 ...    v0 v1 v2 v3 ...

        B =        u                  u                  u                        u                  u
               /  /  x  x     +   /  x  \  x     +   /  x  x  \     + ... +   x  /  \  x     +   x  /  x  \     + ...
              v0 v1 v2 v3 ...    v0 v1 v2 v3 ...    v0 v1 v2 v3 ...          v0 v1 v2 v3 ...    v0 v1 v2 v3 ...

     2) top-down DP

        dpDown[vi] = The number of decompositions of the tree into paths when 'vi' is the parent of u

                               u
                         .  . .     x
                        .  .  .      x
                       v0 v1 v2 ... (vi) ...

     3) path DP

        pathDp[root] = 1
        pathDP[u] = pathDp[parent] * PROD{ dpUp[v][1] } / dpUp[u][1]
                                      v in children of u

                          root
                        /  /  \
                       *  a2   *
                      /  /  \  ...
                   ...  a1   *
                      / | \   ...
                     *  u  *
                      / | \   ...
                     *  *  *

        pathDP[u] = PROD dpUp[i][1]
                   i in { *s }

  2. query

                          (C)
                            \
                        ____lca_______
                        /  /  \  \   \
                      (B) p2  (B) p3 (B)
                      /  /  \ ... | \
                   ...  p1  (A)  (A) p4
                      / | \   ...    | \
                    (A) u (A)       (A) v
                      / | \   ...     / | \
                    (A)(A)(A)       (A)(A)(A)

*/
template <int mod = 998'244'353>
struct TreePathDecompositionCounterMod {
    int N;
    int logN;

    vector<vector<int>> edges;
    vector<vector<int>> P;          // P[logN][N]
    vector<int> level;

    int inv2;                       // 1/2

    vector<array<int, 2>> dpUp;     // dpUp[u] = (up, up + subtree)
    vector<int> dpDown;             // 

    vector<int> invSum;             // invSum[u] = SUM { dpUp[u][0] / dpUp[u][1], ... }
    vector<int> invSum2;            // invSum[u] = SUM { (dpUp[u][0] / dpUp[u][1])^2, ... }
    vector<int> prodChildren;       // prodChildren[u] = dpUp[v0][1] * dpUp[v1][1] * ...     , 'v's are children of u

    vector<int> pathDp;             // pathDp[u] = prodChildren[u][1] * prodChildren[parent of u][1] * ... * prodChildren[0][1]
                                    //           / (dpUp[u][1] * dpUp[parent of u][1] * ... * dpUp[0][1])

    TreePathDecompositionCounterMod() {
    }

    explicit TreePathDecompositionCounterMod(int n) {
        init(n);
    }

    void init(int n) {
        N = n;

        logN = 1;
        while ((1 << logN) <= N)
            logN++;

        edges = vector<vector<int>>(N);
        P = vector<vector<int>>(logN, vector<int>(N));
        level = vector<int>(N);

        inv2 = modInv(2);

        dpUp = vector<array<int, 2>>(N);
        dpDown = vector<int>(N);

        invSum = vector<int>(N);
        invSum2 = vector<int>(N);
        prodChildren = vector<int>(N);

        pathDp = vector<int>(N);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    //---

    // O(N*(logN + logMOD))
    void build() {
        inv2 = modInv(2);       // O(logMOD)

        dfsUp(0, -1);           // O(N*logMOD)
        dfsDown(0, -1, 0, 1);   // O(N*logMOD)
        makeLcaTable();         // O(N*logN)

        dfsBuild(0, -1);        // O(N*logMOD)
    }

    //--- queries

    // O(1)
    int count() const {
        return dpUp[0][2];
    }

    // O(1)
    int countSubtree(int u) const {
        return dpUp[u][2];
    }

    // O(logN + logMOD)
    int countExceptPath(int u, int v) const {
        if (level[u] > level[v])
            swap(u, v);

        long long res;
        int lca = findLCA(u, v);
        if (lca == u) {
            res = 1ll * pathDp[v] * modInv(pathDp[lca]) % mod;  // ... (A)
        } else {
            long long invLcaDp = modInv(pathDp[lca]);
            res = (1ll * pathDp[u] * invLcaDp % mod)
                * (1ll * pathDp[v] * invLcaDp % mod) % mod;     // ... (A)
        }
        res = res * prodChildren[lca] % mod;                    // ... (B)
        res = res * dpDown[lca] % mod;                          // ... (C)

        return int(res);
    }

    //-

    // O(N)
    int countNaive() const {
        vector<bool> vis(N);
        return dfsCount(vis, 0, -1).second;
    }

    // O(N)
    int countSubtreeNaive(int u) const {
        vector<bool> vis(N);
        if (P[0][u] >= 0)
            vis[P[0][u]] = true;
        return dfsCount(vis, u, P[0][u]).second;
    }

    // O(N)
    int countExceptPathNaive(int u, int v) {
        vector<int> vec;
        vector<bool> vis(N);

        if (level[u] > level[v])
            swap(u, v);

        while (level[u] < level[v]) {
            vis[v] = true;
            vec.push_back(v);
            v = P[0][v];
        }

        while (u != v) {
            vis[u] = true;
            vis[v] = true;
            vec.push_back(u);
            vec.push_back(v);
            u = P[0][u];
            v = P[0][v];
        }
        vis[u] = true;
        vec.push_back(u);

        long long res = 1;
        for (int t : vec) {
            for (auto i : edges[t]) {
                if (vis[i])
                    continue;
                res = res * dfsCount(vis, i, t).second % mod;
            }
        }

        return int(res);
    }

private:
    pair<int, int> dfsCount(vector<bool>& vis, int u, int parent) const {
        vector<pair<int, int>> dp;
        for (int v : edges[u]) {
            if (vis[v] || v == parent)
                continue;

            auto r = dfsCount(vis, v, u);
            dp.push_back(r);
        }

        pair<int, int> res;
        if (dp.empty()) {
            res.first = 1;
            res.second = 1;
        } else {
            long long isum = 0;
            long long isum2 = 0;

            long long prod = 1;
            for (int i = 0; i < int(dp.size()); i++) {
                prod = 1ll * prod * dp[i].second % mod;
                ll inv = 1ll * dp[i].first * modInv(dp[i].second) % mod;

                isum += inv;
                if (isum >= mod)
                    isum -= mod;
                isum2 = (isum2 + inv * inv) % mod;
            }

            long long sum0 = prod * (1 + isum) % mod;
            long long sum1 = (prod * ((isum * isum % mod - isum2) * inv2 % mod) + sum0) % mod;
            if (sum1 < 0)
                sum1 += mod;

            res.first = int(sum0);
            res.second = int(sum1);
        }

        return res;
    }


    //---

    void dfsUp(int u, int parent) {
        if (edges[u].empty() || (parent >= 0 && edges[u].size() == 1)) {
            dpUp[u][0] = 1;
            dpUp[u][1] = 1;
            prodChildren[u] = 1;
        }

        P[0][u] = parent;

        long long isum = 0, isum2 = 0, prod = 1;
        for (int v : edges[u]) {
            if (v == parent)
                continue;

            level[v] = level[u] + 1;
            dfsUp(v, u);

            prod = prod * dpUp[v][1] % mod;

            long long inv = 1ll * dpUp[v][0] * modInv(dpUp[v][1]) % mod;
            isum += inv;
            if (isum >= mod)
                isum -= mod;
            isum2 = (isum2 + inv * inv) % mod;
        }
        prodChildren[u] = int(prod);
        invSum[u] = int(isum);
        invSum2[u] = int(isum2);

        long long sum0 = prod * (1 + isum) % mod;   // connect 'u' to none or one of children
        long long sum1 = (prod * ((isum * isum % mod - isum2) * inv2 % mod) + sum0) % mod;
        if (sum1 < 0)                               // connect between twe children
            sum1 += mod;

        dpUp[u][0] = int(sum0);
        dpUp[u][1] = int(sum1);
    }

    void dfsDown(int u, int parent, int a, int b) {
        dpDown[u] = b;

        long long newProd = 1ll * prodChildren[u] * b % mod;
        long long invU = 1ll * a * modInv(b) % mod;
        long long newInvSum = invSum[u] + invU;
        long long newInvSum2 = (invSum2[u] + invU * invU) % mod;
        if (newInvSum >= mod)
            newInvSum -= mod;

        for (int v : edges[u]) {
            if (v == parent)
                continue;

            long long inv = 1ll * dpUp[v][0] * modInv(dpUp[v][1]) % mod;

            long long prod = newProd * modInv(dpUp[v][1]) % mod;
            long long isum = newInvSum - inv;
            long long isum2 = newInvSum2 - 1ll * inv * inv % mod;
            if (isum < 0)
                isum += mod;
            if (isum2 < 0)
                isum2 += mod;

            long long sum0 = prod * (1 + isum) % mod;
            long long sum1 = (prod * ((isum * isum % mod - isum2) * inv2 % mod) + sum0) % mod;
            if (sum1 < 0)
                sum1 += mod;

            dfsDown(v, u, int(sum0), int(sum1));
        }
    }

    void dfsBuild(int u, int parent) {
        if (parent < 0) {
            pathDp[u] = 1;
        } else {
            pathDp[u] = int(1ll * pathDp[parent] * prodChildren[u] % mod * modInv(dpUp[u][1]) % mod);
        }

        for (int v : edges[u]) {
            if (v == parent)
                continue;
            dfsBuild(v, u);
        }
    }

    //--- LCA

    void makeLcaTable() {
        for (int i = 1; i < logN; i++) {
            for (int j = 0; j < N; j++) {
                int pp = P[i - 1][j];
                P[i][j] = pp < 0 ? pp : P[i - 1][pp];
            }
        }
    }

    int climbTree(int node, int dist) const {
        if (dist <= 0)
            return node;

        for (int i = 0; dist > 0; i++) {
            if (dist & 1)
                node = P[i][node];
            dist >>= 1;
        }

        return node;
    }

    int findLCA(int A, int B) const {
        if (level[A] < level[B])
            swap(A, B);

        A = climbTree(A, level[A] - level[B]);

        if (A == B)
            return A;

        int bitCnt = 0;
        for (int x = level[A]; x; x >>= 1)
            bitCnt++;

        for (int i = bitCnt - 1; i >= 0; i--) {
            if (P[i][A] > 0 && P[i][A] != P[i][B]) {
                A = P[i][A];
                B = P[i][B];
            }
        }

        return P[0][A];
    }

    int distance(int u, int v) const {
        return level[u] + level[v] - level[findLCA(u, v)] * 2;
    }

    //--- modular operations

    static int modPow(int x, int n) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return int(res);
    }

    static int modInv(int a) {
        return modPow(a, mod - 2);
    }
};
