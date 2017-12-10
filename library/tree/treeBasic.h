#pragma once

/*
<How to use>
    1) call add() to construct tree
    2) call build() to fill mP, mLevel, and mTreeSize
       - It will call dfs() / dfsIter() or bfs() to set level and parent table
       - It will also call makeLcaTable() to create LCA table
    3) call query functions
*/

// N : the number of nodes
// logN : modify LCA table size (log2(MAXN))
struct Tree {
    int                 mN;         // the number of vertex
    int                 mLogN;      // ceil(log2(mN))

    vector<vector<int>> mE;         // edges (vertex number)
    vector<vector<int>> mP;         // mP[0][n] points to the parent
                                    // parent & acestors

    vector<int>         mLevel;     // depth (root is 0)
    vector<int>         mTreeSize;  // call dfsSize() to calculate tree size

    //--- tree construction ---------------------------------------------------

    Tree() {
        mN = 0;
        mLogN = 0;
    }

    Tree(int N, int logN) : mE(N), mP(logN, vector<int>(N)), mLevel(N), mTreeSize(N) {
        mN = N;
        mLogN = logN;
    }

    void init(int N, int logN) {
        mN = N;
        mLogN = logN;

        mE = vector<vector<int>>(N);
        mP = vector<vector<int>>(logN, vector<int>(N));
        mLevel.assign(N, 0);
        mTreeSize.assign(N, 0);
    }

    void addEdge(int u, int v) {
        mE[u].push_back(v);
        mE[v].push_back(u);
    }

    void addEdgeDirected(int u, int v) {
        mE[u].push_back(v);
    }

    void build(int root) {
        dfs(root, -1);
        //dfsIter(root, -1);
        //bfs(root);
        makeLcaTable();
    }

    //--- DFS -----------------------------------------------------------------

    void dfs(int u, int parent) {
        mTreeSize[u] = 1;
        mP[0][u] = parent;

        for (int v : mE[u]) {
            if (v == parent)
                continue;

            mLevel[v] = mLevel[u] + 1;
            dfs(v, u);

            mTreeSize[u] += mTreeSize[v];
        }
    }

    void dfsIter(int root) {
        struct Item {
            int u;
            int parent;
            int vi;         // child index
        };
        vector<Item> st;
        st.reserve(mN);

        st.push_back(Item{ root, -1, -1 });
        while (!st.empty()) {
            Item& it = st.back();
            if (++it.vi == 0) {
                // enter ...
                mTreeSize[it.u] = 1;
                mP[0][it.u] = it.parent;
            }

            if (it.vi > 0)
                mTreeSize[it.u] += mTreeSize[mE[it.u][it.vi - 1]];

            if (it.vi >= (int)mE[it.u].size()) {
                // leave ...
                st.pop_back();
            } else if (mE[it.u][it.vi] != it.parent) {
                // recursion
                int v = mE[it.u][it.vi];
                mLevel[v] = mLevel[it.u] + 1;
                st.push_back(Item{ v, it.u, -1 });
            }
        }
    }

    //--- BFS -----------------------------------------------------------------

    void dfsSize(int u, int parent) {
        mTreeSize[u] = 1;
        for (int v : mE[u]) {
            if (v != parent) {
                dfsSize(v, u);
                mTreeSize[u] += mTreeSize[v];
            }
        }
    }

    void bfs(int root) {
        vector<bool> visited(mN);

        queue<int> Q;
        Q.push(root);
        mP[0][root] = -1;
        visited[root] = true;
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (int v : mE[u]) {
                if (visited[v])
                    continue;

                visited[v] = true;

                mP[0][v] = u;
                mLevel[v] = mLevel[u] + 1;
                Q.push(v);
            }
        }
    }

    //--- LCA -----------------------------------------------------------------

    void makeLcaTable() {
        for (int i = 1; i < mLogN; i++) {
            for (int j = 0; j < mN; j++) {
                int pp = mP[i - 1][j];
                mP[i][j] = pp < 0 ? pp : mP[i - 1][pp];
            }
        }
    }

    int climbTree(int node, int dist) {
        if (dist <= 0)
            return node;

        for (int i = 0; dist > 0; i++) {
            if (dist & 1)
                node = mP[i][node];
            dist >>= 1;
        }

        return node;
    }

    int findLCA(int A, int B) {
        if (mLevel[A] < mLevel[B])
            swap(A, B);

        A = climbTree(A, mLevel[A] - mLevel[B]);

        if (A == B)
            return A;

        int bitCnt = 0;
        for (int x = mLevel[A]; x; x >>= 1)
            bitCnt++;

        for (int i = bitCnt - 1; i >= 0; i--) {
            if (mP[i][A] >= 0 && mP[i][A] != mP[i][B]) {
                A = mP[i][A];
                B = mP[i][B];
            }
        }

        return mP[0][A];
    }

    //--- Centroid ------------------------------------------------------------

    int findCentroid(int u, int parent, int N) {
        bool isMajor = true;
        for (int v : mE[u]) {
            if (v == parent)
                continue;

            int res = findCentroid(v, u, N);
            if (res != -1)
                return res;

            if (mTreeSize[v] + mTreeSize[v] > N)
                isMajor = false;
        }

        if (isMajor && (N - mTreeSize[u]) * 2 <= N)
            return u;

        return -1;
    }

    int findCentroid(int start) {
        mTreeSize = vector<int>(mN);
        dfsSize(start, -1);
        return findCentroid(start, -1, mTreeSize[start]);
    }

    //--- Center --------------------------------------------------------------

    void dfsDist(vector<int>& dist, int u, int parent, int d) {
        dist[u] = d;
        for (int v : mE[u]) {
            if (v != parent)
                dfsDist(dist, v, u, d + 1);
        }
    }

    int findFarthest(const vector<int>& dist) {
        int index = -1;
        for (int i = 0; i < mN; i++) {
            if (dist[i] != mN && (index == -1 || dist[index] < dist[i]))
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
        for (int v : mE[u]) {
            if (v != parent && dfsPath(path, v, u, target))
                return true;
        }
        path.pop_back();
        return false;
    }

    vector<int> findCenters(int start) {
        vector<int> dist(mN, mN);

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
        leaves.reserve(mN);

        vector<int> deg(mN);
        for (int u = 0; u < mN; u++) {
            deg[u] = (int)mE[u].size();
            if (deg[u] <= 1)
                leaves.push_back(u);
        }

        int removedLeaves = (int)leaves.size();
        while (removedLeaves < mN) {
            vector<int> newLeaves;
            newLeaves.reserve(mN);
            for (int u : leaves) {
                for (int v : mE[u]) {
                    if (--deg[v] == 1)
                        newLeaves.push_back(v);
                }
            }
            swap(leaves, newLeaves);
            removedLeaves += (int)leaves.size();
        }
        return move(leaves);
    }

    //--- Diameter ------------------------------------------------------------

    int getDiameter() {
        vector<int> dist(mN, mN);

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
        hs.reserve(mE[u].size());

        for (int v : mE[u]) {
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
        res.reserve(mE[u].size());

        for (int v : mE[u]) {
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
