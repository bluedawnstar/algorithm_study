#pragma once

//--- HLDPathQuery with Segment Tree with Lazy Propagation --------------------

template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct HLDPathQueryLazy {
    HeavyLightDecomposition& hld;

    typedef CompactSegmentTreeLazyUpdate<T, MergeOp, BlockOp> SegTreeT;

    T                   defaultValue;
    MergeOp             mergeOp;
    BlockOp             blockOp;
    vector<SegTreeT>    segTrees;

    explicit HLDPathQueryLazy(HeavyLightDecomposition& hld, T dflt = T())
        : hld(hld), mergeOp(), blockOp(), defaultValue(dflt) {
        // no action
    }

    HLDPathQueryLazy(HeavyLightDecomposition& hld, MergeOp mop, BlockOp bop, T dflt = T())
        : hld(hld), mergeOp(mop), blockOp(bop), defaultValue(dflt) {
        // no action
    }

    void build() {
        segTrees.clear();
        segTrees.reserve(hld.heavyPaths.size());

        // make segment trees on all heavy path
        for (const auto& path : hld.heavyPaths) {
            int m = int(path.size()) - 1;
            segTrees.push_back(SegTreeT(m + 1, mergeOp, defaultValue));
        }
    }

    void build(T initValue) {
        segTrees.clear();
        segTrees.reserve(hld.heavyPaths.size());

        // make segment trees on all heavy path
        for (const auto& path : hld.heavyPaths) {
            int m = int(path.size()) - 1;
            segTrees.push_back(SegTreeT(m + 1, mergeOp, blockOp, defaultValue));
            segTrees.back().update(0, m, initValue);
        }
    }


    // update a value of an edge(u-v)
    void update(int u, int v, T cost) {
        if (u >= 0 && hld.P[0][u] == v)
            swap(u, v);
        //assert(hld.P[0][v] == u);

        int path = hld.heavyPathIndex[v];
        int index = hld.indexInPath(path, v);
        segTrees[path].update(index, cost);
    }

    void updateRange(int u, int v, T cost) {
        int lca = hld.findLCA(u, v);
        updateRangeTopdown(lca, u, cost);
        updateRangeTopdown(lca, v, cost);
    }

    T query(int u, int v) {
        int lca = hld.findLCA(u, v);
        return mergeOp(queryTopdown(lca, u), queryTopdown(lca, v));
    }

    //--- to update & query vertexes

    T rootValue = defaultValue;

    // update a value of an edge(parent of v, v) or a vertex(v)
    void updateVertex(int v, T cost) {
        if (v == hld.mRoot) {
            rootValue = cost;
            return;
        }

        int path = hld.heavyPathIndex[v];
        int index = hld.indexInPath(path, v);
        segTrees[path].update(index, cost);
    }

    void updateRangeVertex(int u, int v, T cost) {
        int lca = hld.findLCA(u, v);
        if (lca == hld.mRoot) {
            updateRangeTopdown(lca, u, cost);
            updateRangeTopdown(lca, v, cost);
            rootValue = mergeOp(rootValue, cost);
        } else {
            updateRangeTopdown(hld.P[0][lca], u, cost);
            updateRangeTopdown(lca, v, cost);
        }
    }

    T queryVertex(int u, int v) {
        int lca = hld.findLCA(u, v);
        if (lca == hld.mRoot)
            return mergeOp(mergeOp(queryTopdown(lca, u), queryTopdown(lca, v)), rootValue);
        else
            return mergeOp(queryTopdown(hld.P[0][lca], u), queryTopdown(lca, v));
    }

protected:
    void updateRangeTopdown(int u, int v, T value) {
        if (u == v)
            return;

        if (hld.heavyPathIndex[u] == hld.heavyPathIndex[v]) {
            int path = hld.heavyPathIndex[u];

            int first = hld.indexInPath(path, u) + 1;
            int last = hld.indexInPath(path, v);
            return segTrees[path].updateRange(first, last, value);
        }

        int path = hld.heavyPathIndex[v];
        int topOfPath = hld.heavyPaths[path][0];

        //assert(topOfPath != v);

        int last = hld.indexInPath(path, v);
        segTrees[path].updateRange(0, last, value);

        updateRangeTopdown(u, topOfPath, value);
    }

    // PRECONDITION: u is an ancestor of v
    T queryTopdown(int u, int v) {
        if (u == v)
            return defaultValue;

        if (hld.heavyPathIndex[u] == hld.heavyPathIndex[v]) {
            int path = hld.heavyPathIndex[u];

            int first = hld.indexInPath(path, u) + 1;
            int last = hld.indexInPath(path, v);
            return segTrees[path].query(first, last);
        }

        int path = hld.heavyPathIndex[v];
        int topOfPath = hld.heavyPaths[path][0];

        //assert(topOfPath != v);

        int last = hld.indexInPath(path, v);
        return mergeOp(queryTopdown(u, topOfPath), segTrees[path].query(0, last));
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline HLDPathQueryLazy<T, MergeOp, BlockOp> makeHLDPathQueryLazy(HeavyLightDecomposition& hld, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return HLDPathQueryLazy<T, MergeOp, BlockOp>(hld, mop, bop, dfltValue);
}
