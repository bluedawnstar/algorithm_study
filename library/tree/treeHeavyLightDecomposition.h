#pragma once

// Heavy Light Decomposition
struct HeavyLightDecomposition {
    Tree& mTree;

    int mRoot;
    vector<vector<int>> mHeavyPaths;        // heavy paths
    vector<int>         mHeavyPathIndex;    // convert node number(0 ~ N - 1) to index of heavy paths (mHeavyPaths)

    HeavyLightDecomposition(Tree& tree) : mTree(tree), mRoot(0) {
        // no action
    }

    void doHLD(int root) {
        mRoot = root;
        mHeavyPaths.clear();
        mHeavyPathIndex.resize(mTree.mN, -1);

        vector<bool> visited(mTree.mN);

        queue<int> Q;
        Q.push(root);
        visited[root] = true;
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (int v : mTree.mE[u]) {
                if (visited[v])
                    continue;
                Q.push(v);
                visited[v] = true;
            }

            if (u == root)
                continue;

            int p = mTree.mP[0][u];

            if (mTree.mTreeSize[u] * 2 >= mTree.mTreeSize[p] && p != root) {
                // heavy path -> add u to parent's heavy path
                int parentPath = mHeavyPathIndex[p];
                mHeavyPaths[parentPath].push_back(u);
                mHeavyPathIndex[u] = parentPath;
            } else {
                // light path -> make new heavy path
                mHeavyPathIndex[u] = mHeavyPaths.size();
                mHeavyPaths.push_back(vector<int>(2));
                mHeavyPaths.back()[0] = p;
                mHeavyPaths.back()[1] = u;
            }
        }
    }

    // PRECONDITION: v is not the root
    int indexInPath(int v) {
        assert(v != mRoot);
        int topOfPath = mHeavyPaths[mHeavyPathIndex[v]][0];
        return mTree.mLevel[mTree.mP[0][v]] - mTree.mLevel[topOfPath];
    }

    // PRECONDITION: path is the index of v's heavy path
    // PRECONDITION: v is not the root
    int indexInPath(int path, int v) {
        assert(v != mRoot);
        int topOfPath = mHeavyPaths[path][0];
        return mTree.mLevel[mTree.mP[0][v]] - mTree.mLevel[topOfPath];
    }
};

//--- HLDPathQuery with Segment Tree ------------------------------------------

template <typename T, typename BinOp = function<T(T, T)>>
struct HLDPathQuery {
    HeavyLightDecomposition& mHld;

    typedef SegmentTree<T, BinOp> SegTreeT;

    T                   mDefaultValue;
    BinOp               mBinOp;
    vector<SegTreeT>    mSegTrees;

    HLDPathQuery(HeavyLightDecomposition& hld, T dflt = T()) : mHld(hld), mBinOp(), mDefaultValue(dflt) {
        // no action
    }

    HLDPathQuery(HeavyLightDecomposition& hld, BinOp op, T dflt = T()) : mHld(hld), mBinOp(op), mDefaultValue(dflt) {
        // no action
    }

    void build() {
        mSegTrees.clear();
        mSegTrees.reserve(mHld.mHeavyPaths.size());

        // make segment trees on all heavy path
        for (const auto& path : mHld.mHeavyPaths) {
            int m = (int)path.size() - 1;
            mSegTrees.push_back(SegTreeT(m, mBinOp, mDefaultValue));
        }
    }

    void build(T initValue) {
        mSegTrees.clear();
        mSegTrees.reserve(mHld.mHeavyPaths.size());

        // make segment trees on all heavy path
        for (const auto& path : mHld.mHeavyPaths) {
            int m = (int)path.size() - 1;
            mSegTrees.push_back(SegTreeT(m, mBinOp, mDefaultValue));
            for (int i = 0; i < m; ++i)
                mSegTrees.back().update(i, initValue);
        }
    }


    // update a value of an edge(u-v)
    void update(int u, int v, T cost) {
        if (u >= 0 && mHld.mTree.mP[0][u] == v)
            swap(u, v);
        //assert(gP[0][v] == u);

        int path = mHld.mHeavyPathIndex[v];
        int index = mHld.indexInPath(path, v);
        mSegTrees[path].update(index, cost);
    }

    T query(int u, int v) {
        int lca = mHld.mTree.findLCA(u, v);
        return mBinOp(queryTopdown(lca, u), queryTopdown(lca, v));
    }

    //--- to update & query vertexes

    T mRootValue = mDefaultValue;

    // update a value of an edge(parent of v, v) or a vertex(v)
    void updateVertex(int v, T cost) {
        if (v == mHld.mRoot) {
            mRootValue = cost;
            return;
        }

        int path = mHld.mHeavyPathIndex[v];
        int index = mHld.indexInPath(path, v);
        mSegTrees[path].update(index, cost);
    }

    T queryVertex(int u, int v) {
        int lca = mHld.mTree.findLCA(u, v);
        if (lca == mHld.mRoot)
            return mBinOp(mBinOp(queryTopdown(lca, u), queryTopdown(lca, v)), mRootValue);
        else
            return mBinOp(queryTopdown(mHld.mTree.mP[0][lca], u), queryTopdown(lca, v));
    }

protected:
    // return max value a path from u to v (u is an ancestor of v)
    T queryTopdown(int u, int v) {
        if (u == v)
            return mDefaultValue;

        if (mHld.mHeavyPathIndex[u] == mHld.mHeavyPathIndex[v]) {
            int path = mHld.mHeavyPathIndex[u];

            int first = mHld.indexInPath(path, u) + 1;
            int last = mHld.indexInPath(path, v);
            return mSegTrees[path].query(first, last);
        }

        int path = mHld.mHeavyPathIndex[v];
        int topOfPath = mHld.mHeavyPaths[path][0];

        //assert(topOfPath != v);

        int last = mHld.indexInPath(path, v);
        return mBinOp(queryTopdown(u, topOfPath), mSegTrees[path].query(0, last));
    }
};

template <typename T, typename BinOp>
HLDPathQuery<T, BinOp> makeHLDPathQuery(HeavyLightDecomposition& hld, BinOp op, T dfltValue = T()) {
    return HLDPathQuery<T, BinOp>(hld, op, dfltValue);
}


//--- HLDPathQuery with Segment Tree with Lazy Propagation --------------------

template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct HLDPathQueryLazy {
    HeavyLightDecomposition& mHld;

    typedef SegmentTreeLazy<T, MergeOp, BlockOp> SegTreeT;

    T                   mDefaultValue;
    MergeOp             mMergeOp;
    BlockOp             mBlockOp;
    vector<SegTreeT>    mSegTrees;

    HLDPathQueryLazy(HeavyLightDecomposition& hld, T dflt = T())
        : mHld(hld), mMergeOp(), mBlockOp(), mDefaultValue(dflt) {
        // no action
    }

    HLDPathQueryLazy(HeavyLightDecomposition& hld, MergeOp mop, BlockOp bop, T dflt = T())
        : mHld(hld), mMergeOp(mop), mBlockOp(bop), mDefaultValue(dflt) {
        // no action
    }

    void build() {
        mSegTrees.clear();
        mSegTrees.reserve(mHld.mHeavyPaths.size());

        // make segment trees on all heavy path
        for (const auto& path : mHld.mHeavyPaths) {
            int m = (int)path.size() - 1;
            mSegTrees.push_back(SegTreeT(m, mBinOp, mDefaultValue));
        }
    }

    void build(T initValue) {
        mSegTrees.clear();
        mSegTrees.reserve(mHld.mHeavyPaths.size());

        // make segment trees on all heavy path
        for (const auto& path : mHld.mHeavyPaths) {
            int m = (int)path.size() - 1;
            mSegTrees.push_back(SegTreeT(m, mBinOp, mDefaultValue));
            mSegTrees.back().updateRange(0, m, initValue);
        }
    }


    // update a value of an edge(u-v)
    void update(int u, int v, T cost) {
        if (u >= 0 && mHld.mTree.mP[0][u] == v)
            swap(u, v);
        //assert(gP[0][v] == u);

        int path = mHld.mHeavyPathIndex[v];
        int index = mHld.indexInPath(path, v);
        mSegTrees[path].update(index, cost);
    }

    void updateRange(int u, int v, T cost) {
        int lca = mHld.mTree.findLCA(u, v);
        updateRangeTopdown(lca, u, cost);
        updateRangeTopdown(lca, v, cost);
    }

    T query(int u, int v) {
        int lca = mHld.mTree.findLCA(u, v);
        return mMergeOp(queryTopdown(lca, u), queryTopdown(lca, v));
    }

    //--- to update & query vertexes

    T mRootValue = mDefaultValue;

    // update a value of an edge(parent of v, v) or a vertex(v)
    void updateVertex(int v, T cost) {
        if (v == mHld.mRoot) {
            mRootValue = cost;
            return;
        }

        int path = mHld.mHeavyPathIndex[v];
        int index = mHld.indexInPath(path, v);
        mSegTrees[path].update(index, cost);
    }

    void updateRangeVertex(int u, int v, T cost) {
        int lca = mHld.mTree.findLCA(u, v);
        if (lca == mHld.mRoot) {
            updateRangeTopdown(lca, u, cost);
            updateRangeTopdown(lca, v, cost);
            mRootValue += cost;
        } else {
            updateRangeTopdown(mHld.mTree.mP[0][lca], u, cost);
            updateRangeTopdown(lca, v, cost);
        }
    }

    T queryVertex(int u, int v) {
        int lca = mHld.mTree.findLCA(u, v);
        if (lca == mHld.mRoot)
            return mMergeOp(mMergeOp(queryTopdown(lca, u), queryTopdown(lca, v)), mRootValue);
        else
            return mMergeOp(queryTopdown(mHld.mTree.mP[0][lca], u), queryTopdown(lca, v));
    }

protected:
    void updateRangeTopdown(int u, int v, T value) {
        if (u == v)
            return;

        if (mHld.mHeavyPathIndex[u] == mHld.mHeavyPathIndex[v]) {
            int path = mHld.mHeavyPathIndex[u];

            int first = mHld.indexInPath(path, u) + 1;
            int last = mHld.indexInPath(path, v);
            return mSegTrees[path].updateRange(first, last, value);
        }

        int path = mHld.mHeavyPathIndex[v];
        int topOfPath = mHld.mHeavyPaths[path][0];

        //assert(topOfPath != v);

        int last = mHld.indexInPath(path, v);
        mSegTrees[path].updateRange(0, last, value);

        updateRangeTopdown(u, topOfPath, value);
    }

    // return max value a path from u to v (u is an ancestor of v)
    T queryTopdown(int u, int v) {
        if (u == v)
            return mDefaultValue;

        if (mHld.mHeavyPathIndex[u] == mHld.mHeavyPathIndex[v]) {
            int path = mHld.mHeavyPathIndex[u];

            int first = mHld.indexInPath(path, u) + 1;
            int last = mHld.indexInPath(path, v);
            return mSegTrees[path].query(first, last);
        }

        int path = mHld.mHeavyPathIndex[v];
        int topOfPath = mHld.mHeavyPaths[path][0];

        //assert(topOfPath != v);

        int last = mHld.indexInPath(path, v);
        return mBinOp(queryTopdown(u, topOfPath), mSegTrees[path].query(0, last));
    }
};

template <typename T, typename MergeOp, typename BlockOp>
HLDPathQueryLazy<T, MergeOp, BlockOp> makeHLDPathQueryLazy(HeavyLightDecomposition& hld, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return HLDPathQueryLazy<T, MergeOp, BlockOp>(hld, mop, bop, dfltValue);
}


//--- HLDPathQuery with BITs --------------------------------------------------

template <typename T>
struct HLDPathQueryBIT {
    HeavyLightDecomposition& mHld;

    vector<FenwickTreeMultAdd<T>> mBIT;

    HLDPathQueryBIT(HeavyLightDecomposition& hld) : mHld(hld) {
        // no action
    }

    void build() {
        mBIT.clear();
        mBIT.reserve(mHld.mHeavyPaths.size());

        // make segment trees on all heavy path
        for (const auto& path : mHld.mHeavyPaths) {
            int m = (int)path.size() - 1;
            mBIT.push_back(FenwickTreeMultAdd<T>(m));
        }
    }

    // update a value of an edge(u-v)
    void add(int u, int v, T cost) {
        if (u >= 0 && mHld.mTree.mP[0][u] == v)
            swap(u, v);
        //assert(gP[0][v] == u);

        int path = mHld.mHeavyPathIndex[v];
        int index = mHld.indexInPath(path, v);
        mBIT[path].add(index, cost);
    }

    void addRange(int u, int v, T cost) {
        int lca = mHld.mTree.findLCA(u, v);
        addRangeTopdown(lca, u, cost);
        addRangeTopdown(lca, v, cost);
    }

    T query(int u, int v) {
        int lca = mHld.mTree.findLCA(u, v);
        return queryTopdown(lca, u) + queryTopdown(lca, v);
    }

    //--- to update & query vertexes

    T mRootValue = 0;

    // update a value of an edge(parent of v, v) or a vertex(v)
    void addVertex(int v, T cost) {
        if (v == mHld.mRoot) {
            mRootValue = cost;
            return;
        }

        int path = mHld.mHeavyPathIndex[v];
        int index = mHld.indexInPath(path, v);
        mBIT[path].add(index, cost);
    }

    void addRangeVertex(int u, int v, T cost) {
        int lca = mHld.mTree.findLCA(u, v);
        if (lca == mHld.mRoot) {
            addRangeTopdown(lca, u, cost);
            addRangeTopdown(lca, v, cost);
            mRootValue += cost;
        } else {
            addRangeTopdown(mHld.mTree.mP[0][lca], u, cost);
            addRangeTopdown(lca, v, cost);
        }
    }
    
    T queryVertex(int u, int v) {
        int lca = mHld.mTree.findLCA(u, v);
        if (lca == mHld.mRoot)
            return queryTopdown(lca, u) + queryTopdown(lca, v) + mRootValue;
        else
            return queryTopdown(mHld.mTree.mP[0][lca], u) + queryTopdown(lca, v);
    }

protected:
    void addRangeTopdown(int u, int v, T delta) {
        if (u == v)
            return;

        if (mHld.mHeavyPathIndex[u] == mHld.mHeavyPathIndex[v]) {
            int path = mHld.mHeavyPathIndex[u];

            int first = mHld.indexInPath(path, u) + 1;
            int last = mHld.indexInPath(path, v);
            return mBIT[path].addRange(first, last, delta);
        }

        int path = mHld.mHeavyPathIndex[v];
        int topOfPath = mHld.mHeavyPaths[path][0];

        //assert(topOfPath != v);

        int last = mHld.indexInPath(path, v);
        mBIT[path].addRange(0, last, delta);

        addRangeTopdown(u, topOfPath, delta);
    }

    // return max value a path from u to v (u is an ancestor of v)
    T queryTopdown(int u, int v) {
        if (u == v)
            return 0;

        if (mHld.mHeavyPathIndex[u] == mHld.mHeavyPathIndex[v]) {
            int path = mHld.mHeavyPathIndex[u];

            int first = mHld.indexInPath(path, u) + 1;
            int last = mHld.indexInPath(path, v);
            return mBIT[path].sumRange(first, last);
        }

        int path = mHld.mHeavyPathIndex[v];
        int topOfPath = mHld.mHeavyPaths[path][0];

        //assert(topOfPath != v);

        int last = mHld.indexInPath(path, v);
        return queryTopdown(u, topOfPath) + mBIT[path].sumRange(0, last);
    }
};
