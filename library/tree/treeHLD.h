#pragma once

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

    void build(int root) {
        this->root = root;
        dfs(root, -1);
        //dfsIter(root);
        //bfs(root);
        makeLcaTable();

        doHLD();
    }

    //--- LCA

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

    // find LCA when the root is changed
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

    int distance(int u, int v) const {
        return level[u] + level[v] - level[findLCA(u, v)] * 2;
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

    //--- BFS

    void dfsSize(int u, int parent) {
        treeSize[u] = 1;
        for (int v : edges[u]) {
            if (v != parent) {
                dfsSize(v, u);
                treeSize[u] += treeSize[v];
            }
        }
    }

    void bfs(int root) {
        vector<bool> visited(N);

        queue<int> Q;
        Q.push(root);
        P[0][root] = -1;
        visited[root] = true;
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (int v : edges[u]) {
                if (visited[v])
                    continue;

                visited[v] = true;

                P[0][v] = u;
                level[v] = level[u] + 1;
                Q.push(v);
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

    void doHLD() {
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
