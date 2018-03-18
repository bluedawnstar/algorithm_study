#pragma once

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

    T query(int u, int v) const {
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
    T queryTopdown(int u, int v) const {
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

