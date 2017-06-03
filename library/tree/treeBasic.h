#pragma once

/*
<How to use>
    1) call setCount() to set the number of vertexes
    2) call add() to construct tree
    3) call dfs() / dfsIter() or bfs() to set level and parent table
    4) call makeLcaTable() to create LCA table

    5) call query functions
*/

// MAXN : modify the maximum number of nodes
// LOGN : modify LCA table size (log2(MAXN))
template <int MaxN, int LogN>
struct Tree {
    int                 mN;         // the number of vertex

    vector<vector<int>> mE;         // edges (vertex number)
    vector<vector<int>> mP;         // mP[0][n] points to the parent
                                    // parent & acestors

    vector<int>         mLevel;     // depth (root is 0)

    //--- tree construction ---------------------------------------------------
    
    Tree() : mE(MaxN), mP(LogN, vector<int>(MaxN)), mLevel(MaxN) {
        mN = 0;
    }

    void clear() {
        if (mN <= 0)
            return;

        for (int i = 0; i < mN; i++)
            mE[i].clear();

        for (int i = 0; i < LogN; i++)
            fill(mP[i].begin(), mP[i].end(), 0);
        fill(mLevel.begin(), mLevel.end(), 0);
    }


    void setVertexCount(int n) {
        mN = n;
    }

    void addEdge(int u, int v) {
        mE[u].push_back(v);
        mE[v].push_back(u);
    }

    void addEdgeDirected(int u, int v) {
        mE[u].push_back(v);
    }

    //--- DFS -----------------------------------------------------------------

    void dfs(int u, int parent) {
        mP[0][u] = parent;

        for (int v : mE[u]) {
            if (v == parent)
                continue;

            mLevel[v] = mLevel[u] + 1;
            dfs(v, u);
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
                mP[0][it.u] = it.parent;
            }
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

    void bfs(int root) {
        vector<bool> visited(mN);

        queue<int> Q;
        Q.push(root);
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
        for (int i = 1; i < LogN; i++) {
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
};
