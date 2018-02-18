#pragma once

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
