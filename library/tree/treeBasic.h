#pragma once

/*
<How to use>
    1) call add() to construct tree
    2) call build() to fill P, level, and treeSize
       - It will call dfs() / dfsIter() or bfs() to set level and parent table
       - It will also call makeLcaTable() to create LCA table
    3) call query functions
*/

// N : the number of nodes
// logN : modify LCA table size (log2(MAXN))
struct Tree {
    int                 N;          // the number of vertex
    int                 logN;       // log2(N - 1) + 2

    vector<vector<int>> edges;      // edges (vertex number)
    vector<vector<int>> P;          // P[0][n] points to the parent
                                    // parent & acestors

    vector<int>         level;      // depth (root is 0)
    vector<int>         treeSize;   // call dfsSize() to calculate tree size

    Tree() : N(0), logN(0) {
    }

    explicit Tree(int n, int logN = 0) {
        init(n, logN);
    }

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
        dfs(root, -1);
        //dfsIter(root);
        //bfs(root);
        makeLcaTable();
    }

    //--- DFS -----------------------------------------------------------------

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

            if (it.vi >= (int)edges[it.u].size()) {
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

    //--- BFS -----------------------------------------------------------------

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

    //--- LCA -----------------------------------------------------------------

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

    //--- Centroid ------------------------------------------------------------

    int findCentroid(int u, int parent, int N) {
        bool isMajor = true;
        for (int v : edges[u]) {
            if (v == parent)
                continue;

            int res = findCentroid(v, u, N);
            if (res != -1)
                return res;

            if (treeSize[v] + treeSize[v] > N)
                isMajor = false;
        }

        if (isMajor && (N - treeSize[u]) * 2 <= N)
            return u;

        return -1;
    }

    int findCentroid(int start) {
        treeSize = vector<int>(N);
        dfsSize(start, -1);
        return findCentroid(start, -1, treeSize[start]);
    }

    //--- Center --------------------------------------------------------------

    void dfsDist(vector<int>& dist, int u, int parent, int d) {
        dist[u] = d;
        for (int v : edges[u]) {
            if (v != parent)
                dfsDist(dist, v, u, d + 1);
        }
    }

    int findFarthest(const vector<int>& dist) {
        int index = -1;
        for (int i = 0; i < N; i++) {
            if (dist[i] != N && (index == -1 || dist[index] < dist[i]))
                index = i;
        }
        return index;
    }

    bool dfsPath(vector<int>& path, int u, int parent, int target) {
        if (u == target) {
            path.push_back(target);
            return true;
        }
        path.push_back(u);
        for (int v : edges[u]) {
            if (v != parent && dfsPath(path, v, u, target))
                return true;
        }
        path.pop_back();
        return false;
    }

    vector<int> findCenters(int start) {
        vector<int> dist(N, N);

        dfsDist(dist, start, -1, 0);
        start = findFarthest(dist);

        dfsDist(dist, start, -1, 0);
        int farthest = findFarthest(dist);

        vector<int> path;
        dfsPath(path, start, -1, farthest);
        if (path.size() % 2 == 0)
            return vector<int>{ path[(int)path.size() / 2 - 1], path[(int)path.size() / 2] };
        else
            return vector<int>{ path[(int)path.size() / 2] };
    }

    vector<int> findCentersEx() {
        vector<int> leaves;
        leaves.reserve(N);

        vector<int> deg(N);
        for (int u = 0; u < N; u++) {
            deg[u] = (int)edges[u].size();
            if (deg[u] <= 1)
                leaves.push_back(u);
        }

        int removedLeaves = (int)leaves.size();
        while (removedLeaves < N) {
            vector<int> newLeaves;
            newLeaves.reserve(N);
            for (int u : leaves) {
                for (int v : edges[u]) {
                    if (--deg[v] == 1)
                        newLeaves.push_back(v);
                }
            }
            swap(leaves, newLeaves);
            removedLeaves += (int)leaves.size();
        }
        return leaves;
    }

    //--- Diameter ------------------------------------------------------------

    int getDiameter() {
        vector<int> dist(N, N);

        dfsDist(dist, 0, -1, 0);
        int farthest = findFarthest(dist);

        dfsDist(dist, farthest, -1, 0);
        int target = findFarthest(dist);

        return dist[target];
    }

    //--- Hashing of tree shape (Tree isomorphism) ----------------------------

    static const unsigned long long M1 = 1000000007;
    static const unsigned long long B1 = 31;
    static const unsigned long long Z1 = 16983;

    static const unsigned long long M2 = 1000000009;
    static const unsigned long long B2 = 37;
    static const unsigned long long Z2 = 18657;

    unsigned long long unrootedHash() {
        vector<int> centers = findCenters(0);
        vector<unsigned long long> lhs;
        lhs.reserve(centers.size());

        for (int c : centers)
            lhs.push_back(rootedHash(c, -1));
        sort(lhs.begin(), lhs.end());

        unsigned long long hl = Z1, hr = Z2;
        for (unsigned long long lh : lhs) {
            auto lhl = (unsigned long long)(unsigned int)(lh >> 32);
            auto lhr = (unsigned long long)(unsigned int)lh;
            hl = (hl * B1 + lhl * lhl) % M1;
            hr = (hr * B2 + lhr * lhr) % M2;
        }
        return (hl << 32) | hr;
    }

    unsigned long long rootedHash(int u, int parent) {
        vector<unsigned long long> hs;
        hs.reserve(edges[u].size());

        for (int v : edges[u]) {
            if (v != parent)
                hs.push_back(rootedHash(v, u));
        }
        sort(hs.begin(), hs.end());

        unsigned long long hl = Z1, hr = Z2;
        for (unsigned long long h : hs) {
            hl = (hl * B1 + h * h) % M1;
            hr = (hr * B2 + h * h) % M2;
        }
        return (hl << 32) | hr;
    }

    unsigned long long rootedHash2(int u, int parent) {
        vector<unsigned long long> res;
        res.reserve(edges[u].size());

        for (int v : edges[u]) {
            if (parent != v)
                res.push_back(rootedHash(v, u));
        }
        sort(res.begin(), res.end());

        unsigned long long ans = 4242424242424243ull;
        unsigned long long h = Z1;
        for (unsigned long long x : res) {
            h = (h * B1) % M1;
            ans ^= h * x;
        }
        return ans;
    }
};
