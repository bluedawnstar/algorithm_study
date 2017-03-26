#pragma once

template <int MaxN, int LogN, typename T, typename BinOp = function<T(T, T)>>
struct TreeHLD : public Tree<MaxN, LogN> {
    using Tree<MaxN, LogN>::mN;
    using Tree<MaxN, LogN>::mE;
    using Tree<MaxN, LogN>::mP;
    using Tree<MaxN, LogN>::mLevel;

    vector<int> mSubTreeN;                  // subtree size

    TreeHLD() : Tree<MaxN,LogN>(), mSubTreeN(MaxN), mBinOp() {
        // no action
    }

    TreeHLD(BinOp op) : Tree<MaxN, LogN>(), mSubTreeN(MaxN), mBinOp(op) {
        // no action
    }

    void clear() {
        Tree<MaxN, LogN>::clear();
        fill(mSubTreeN.begin(), mSubTreeN.end(), 0);
    }

    //--------- DFS ---------------------------------------------------------------

    void dfs(int u, int parent) {
        mSubTreeN[u] = 1;

        mP[0][u] = parent;
        for (int v : mE[u]) {
            if (v == parent)
                continue;

            mLevel[v] = mLevel[u] + 1;
            dfs(v, u);

            mSubTreeN[u] += mSubTreeN[v];
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
                mSubTreeN[it.u] = 1;
            }

            if (it.vi > 0)
                mSubTreeN[it.u] += mSubTreeN[mE[it.u][it.vi - 1]];

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

    //--- Heavy Light Decomposition -------------------------------------------

    BinOp               mBinOp;

    vector<vector<int>> mHeavyPaths;        // heavy paths
    vector<int>         mHeavyPathIndex;    // convert node number(0 ~ N - 1) to index of heavy paths (mHeavyPaths)

    void doHLD(int root) {
        mHeavyPaths.clear();
        mHeavyPathIndex.resize(mN, -1);

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
                Q.push(v);
                visited[v] = true;
            }

            if (u == root)
                continue;

            int p = mP[0][u];

            if (mSubTreeN[u] * 2 >= mSubTreeN[p] && p != root) {
                // heavy path -> add u to parent's heavy path
                int parentPathIndex = mHeavyPathIndex[p];
                mHeavyPaths[parentPathIndex].push_back(u);
                mHeavyPathIndex[u] = parentPathIndex;
            } else {
                // light path -> make new heavy path
                mHeavyPathIndex[u] = mHeavyPaths.size();
                mHeavyPaths.push_back(vector<int>(2));
                mHeavyPaths.back()[0] = p;
                mHeavyPaths.back()[1] = u;
            }
        }
    }

    int findEdgeInPath(int pathIndex, int v) {
        int topOfPath = mHeavyPaths[pathIndex][0];
        return mLevel[mP[0][v]] - mLevel[topOfPath];
    }

    //----- Segment Tree Part ------------------------------------------

    typedef SegmentTree<T, BinOp> SegTreeT;
    vector<SegTreeT> mSegTrees;

    void initSegTree(T defaultValue) {
        mSegTrees.clear();
        mSegTrees.reserve(mHeavyPaths.size());

        // make segment trees on all heavy path
        for (const auto& path : mHeavyPaths) {
            int m = path.size();
            mSegTrees.push_back(SegTreeT(m - 1, mBinOp));
            for (int i = 0; i < m - 1; ++i)
                mSegTrees.back().update(i, defaultValue);   // TODO: initialize all path values
        }
    }

    void update(int u, int v, T cost) {
        if (mP[0][u] == v)
            swap(u, v);
        //assert(gP[0][v] == u);

        int pathIndex = mHeavyPathIndex[v];
        int indexInPath = findEdgeInPath(pathIndex, v);
        mSegTrees[pathIndex].update(indexInPath, cost);
    }

    T query(int u, int v) {
        int t = Tree<MaxN,LogN>::findLCA(u, v);
        return mBinOp(queryTopdown(t, u), queryTopdown(t, v));
    }

    // return max value a path from u to v
    T queryTopdown(int u, int v) {
        if (u == v)
            return -1;

        if (mHeavyPathIndex[u] == mHeavyPathIndex[v]) {
            int pathIndex = mHeavyPathIndex[u];

            int firstEdge = findEdgeInPath(pathIndex, u) + 1;
            int lastEdge = findEdgeInPath(pathIndex, v);
            return mSegTrees[pathIndex].query(firstEdge, lastEdge);
        }

        int pathIndex = mHeavyPathIndex[v];
        int topOfPath = mHeavyPaths[pathIndex][0];

        //assert(topOfPath != v);

        int lastEdge = findEdgeInPath(pathIndex, v);
        return mBinOp(queryTopdown(u, topOfPath), mSegTrees[pathIndex].query(0, lastEdge));
    }
};
