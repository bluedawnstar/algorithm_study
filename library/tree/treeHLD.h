#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

// Simplified heavy-light decomposition
struct HLD {
    int                 N;          // the number of vertex
    int                 logN;       // log2(N - 1) + 2

    vector<vector<int>> edges;      // edges (vertex number)
    vector<vector<int>> P;          // P[0][n] points to the parent
                                    // parent & acestors

    vector<int>         level;      // depth (root is 0)
    vector<int>         treeSize;   // call dfsSize() to calculate tree size

    //---

    int root;
    vector<pair<int,int>> leaves;   // leavels[path ID] = (level, node)
    vector<vector<int>> paths;      // paths (bottom to top)
    vector<int>         nodeToPath; // node to the index in paths

    HLD() : N(0), logN(0), root(0) {
    }

    explicit HLD(int n, int logN = 0) : root(0) {
        init(n, logN);
    }

    //---

    void init(int _n, int _logN = 0) {
        N = _n;
        logN = _logN;
        if (logN <= 0) {
#ifndef __GNUC__
            logN = _lzcnt_u32(1u) - _lzcnt_u32((unsigned int)(N - 1)) + 2;
#else
            logN = __builtin_clz(1u) - __builtin_clz((unsigned int)(N - 1)) + 2;
#endif
        }

        edges = vector<vector<int>>(N);
        P = vector<vector<int>>(logN, vector<int>(N));
        level.assign(N, 0);
        treeSize.assign(N, 0);
    }


    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    void addEdgeDirected(int u, int v) {
        edges[u].push_back(v);
    }

    void buildWithSize(int root) {
        this->root = root;
        dfs(root, -1);
        //dfsIter(root);
        makeLcaTable();

        //decomposeWithBfsSize();
        decomposeWithDfsSize();
    }

    void buildWithHeight(int root) {
        this->root = root;
        dfs(root, -1);
        //dfsIter(root);
        makeLcaTable();

        decomposeWithHeight();
    }

    //--- LCA with sparse table (it's faster than methods to use HLD paths)

    // O(logN)
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

    // O(logN)
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

    // find LCA when the root is changed
    // O(logN)
    int findLCA(int root, int A, int B) const {
        int lca = findLCA(A, B);

        int temp = findLCA(A, root);
        if (level[temp] > level[lca])
            lca = temp;

        temp = findLCA(B, root);
        if (level[temp] > level[lca])
            lca = temp;

        return lca;
    }

    // O(1)
    int distance(int u, int v, int lca) const {
        return level[u] + level[v] - level[lca] * 2;
    }

    // O(logN)
    int distance(int u, int v) const {
        return distance(u, v, findLCA(u, v));
    }

    //--- LCA with HLD paths

    // O(logN)
    int climbTree2(int node, int dist) const {
        while (dist > 0) {
            int path = nodeToPath[node];
            int len = int(paths[path].size()) - 1;
            int i = level[paths[path].front()] - level[node];
            int d = min(dist, len - i);
            dist -= d;
            node = paths[path][i + d];
        }
        return node;
    }

    // O(logN)
    int findLCA2(int u, int v) const {
        while (nodeToPath[u] != nodeToPath[v]) {
            int up = paths[nodeToPath[u]].back();
            int vp = paths[nodeToPath[v]].back();
            if (vp < 0 || (up >= 0 && level[up] > level[vp]))
                swap(u, v);
            v = paths[nodeToPath[v]].back();
        }
        return (level[u] <= level[v]) ? u : v;
    }

    // find LCA when the root is changed
    // O(logN)
    int findLCA2(int root, int A, int B) const {
        int lca = findLCA2(A, B);

        int temp = findLCA2(A, root);
        if (level[temp] > level[lca])
            lca = temp;

        temp = findLCA2(B, root);
        if (level[temp] > level[lca])
            lca = temp;

        return lca;
    }

    // O(1)
    int distance2(int u, int v, int lca) const {
        return level[u] + level[v] - level[lca] * 2;
    }

    // O(logN)
    int distance2(int u, int v) const {
        return distance2(u, v, findLCA2(u, v));
    }

    //--- HLD

    int indexInPath(int v) {
        return level[paths[nodeToPath[v]].front()] - level[v];
    }

    int indexInPath(int pathID, int v) {
        return level[paths[pathID].front()] - level[v];
    }

private:
    //--- DFS

    void dfs(int u, int parent) {
        treeSize[u] = 1;
        P[0][u] = parent;

        for (int v : edges[u]) {
            if (v == parent)
                continue;

            level[v] = level[u] + 1;
            dfs(v, u);

            treeSize[u] += treeSize[v];
        }
    }

    void dfsIter(int root) {
        struct Item {
            int u;
            int parent;
            int vi;         // child index
        };
        vector<Item> st;
        st.reserve(N);

        st.push_back(Item{ root, -1, -1 });
        while (!st.empty()) {
            Item& it = st.back();
            if (++it.vi == 0) {
                // enter ...
                treeSize[it.u] = 1;
                P[0][it.u] = it.parent;
            }

            if (it.vi > 0)
                treeSize[it.u] += treeSize[edges[it.u][it.vi - 1]];

            if (it.vi >= int(edges[it.u].size())) {
                // leave ...
                st.pop_back();
            } else if (edges[it.u][it.vi] != it.parent) {
                // recursion
                int v = edges[it.u][it.vi];
                level[v] = level[it.u] + 1;
                st.push_back(Item{ v, it.u, -1 });
            }
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

    //--- HLD

    void dfsHLD(int u, int parent) {
        int bigChild = -1;
        int bigSize = 0;
        for (int v : edges[u]) {
            if (v == parent)
                continue;
            if (bigSize < treeSize[v]) {
                bigChild = v;
                bigSize = treeSize[v];
            }
        }

        // heavy path
        if (bigChild >= 0) {
            nodeToPath[bigChild] = nodeToPath[u];
            paths[nodeToPath[u]].emplace_back(bigChild);
            dfsHLD(bigChild, u);
        }
        // light path
        for (int v : edges[u]) {
            if (v == parent || v == bigChild)
                continue;
            nodeToPath[v] = int(paths.size());
            paths.emplace_back(vector<int>{ u, v });
            dfsHLD(v, u);
        }
    }

    void decomposeWithDfsSize() {
        paths.clear();
        nodeToPath.assign(N, -1);

        nodeToPath[root] = int(paths.size());
        paths.emplace_back(vector<int>{ -1, root });
        dfsHLD(root, -1);

        leaves.clear();
        for (int i = 0; i < int(paths.size()); i++) {
            reverse(paths[i].begin(), paths[i].end());
            int u = paths[i].front();
            leaves.emplace_back(level[u], u);
        }
    }

    //---

    void decomposeWithBfsSize() {
        paths.clear();
        nodeToPath.assign(N, -1);

        vector<bool> visited(N);

        queue<int> Q;
        Q.push(root);
        visited[root] = true;
        nodeToPath[root] = int(paths.size());
        paths.emplace_back(vector<int>{ -1, root });
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            int bigChild = -1;
            int bigSize = 0;
            for (int v : edges[u]) {
                if (visited[v])
                    continue;
                if (bigSize < treeSize[v]) {
                    bigChild = v;
                    bigSize = treeSize[v];
                }
            }
            for (int v : edges[u]) {
                if (visited[v])
                    continue;
                Q.push(v);
                visited[v] = true;
                if (v == bigChild) {
                    nodeToPath[v] = nodeToPath[u];
                    paths[nodeToPath[u]].emplace_back(v);
                } else {
                    nodeToPath[v] = int(paths.size());
                    paths.emplace_back(vector<int>{ u, v });
                }
            }
        }

        leaves.clear();
        for (int i = 0; i < int(paths.size()); i++) {
            reverse(paths[i].begin(), paths[i].end());
            int u = paths[i].front();
            leaves.emplace_back(level[u], u);
        }
    }

    //---

    void decomposeWithHeight() {
        leaves.clear();
        leaves.reserve(N);
        for (int u = 0; u < N; u++) {
            if (root != u && int(edges[u].size()) == 1)
                leaves.emplace_back(level[u], u);
        }
        sort(leaves.begin(), leaves.end(), greater<pair<int, int>>());

        paths = vector<vector<int>>(leaves.size());
        nodeToPath.assign(N, -1);

        vector<bool> visited(N);
        for (int g = 0; g < int(leaves.size()); g++) {
            int v = leaves[g].second;
            for (; v >= 0 && !visited[v]; v = P[0][v]) {
                nodeToPath[v] = g;
                paths[g].push_back(v);
                visited[v] = true;
            }
            paths[g].push_back(v);  // parent of this path
        }
    }
};
