#pragma once

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

    T query(int u, int v) const {
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

    T queryVertex(int u, int v) const {
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
    T queryTopdown(int u, int v) const {
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
