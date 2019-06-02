#pragma once

// Simplified HLDPathQuery with Segment Tree - Lazy Propagation
template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct SimpleHLDPathQueryLazy {
    HLD&                hld;

    typedef CompactSegmentTreeLazyUpdate<T, MergeOp, BlockOp> SegTreeT;

    T                   defaultValue;
    MergeOp             mergeOp;
    BlockOp             blockOp;
    vector<SegTreeT>    segTrees;

    explicit SimpleHLDPathQueryLazy(HLD& hld, T dflt = T())
        : hld(hld), mergeOp(), blockOp(), defaultValue(dflt) {
        // no action
    }

    SimpleHLDPathQueryLazy(HLD& hld, MergeOp mop, BlockOp bop, T dflt = T())
        : hld(hld), mergeOp(mop), blockOp(bop), defaultValue(dflt) {
        // no action
    }

    void build() {
        segTrees.clear();
        segTrees.reserve(hld.paths.size());

        // make segment trees on all paths
        for (const auto& path : hld.paths)
            segTrees.push_back(SegTreeT(int(path.size()), mergeOp, defaultValue));
    }

    void build(T initValue) {
        segTrees.clear();
        segTrees.reserve(hld.paths.size());

        // make segment trees on all paths
        for (const auto& path : hld.paths) {
            int m = int(path.size()) - 1;
            segTrees.push_back(SegTreeT(m + 1, mergeOp, blockOp, defaultValue));
            segTrees.back().updateRange(0, m - 1, initValue);
        }
    }

    //--- edge-based operations

    // update a value of an edge(u-v)
    void update(int u, int v, T cost) {
        if (u >= 0 && hld.P[0][u] == v)
            swap(u, v);

        int path = hld.nodeToPath[v];
        int index = hld.indexInPath(path, v);
        segTrees[path].update(index, cost);
    }

    void updateRange(int u, int v, int lca, T cost) {
        updateRangeBottomup(lca, u, cost);
        updateRangeBottomup(lca, v, cost);
    }

    void updateRange(int u, int v, T cost) {
        int lca = hld.findLCA(u, v);
        updateRangeBottomup(lca, u, cost);
        updateRangeBottomup(lca, v, cost);
    }

    T query(int u, int v, int lca) {
        return mergeOp(queryBottomup(lca, u), queryBottomup(lca, v));
    }

    T query(int u, int v) {
        return query(u, v, hld.findLCA(u, v));
    }

    //--- vertex-based operations

    T rootValue = defaultValue;

    void updateVertex(int v, T cost) {
        if (v == hld.mRoot) {
            rootValue = cost;
            return;
        }
        int path = hld.nodeToPath[v];
        int index = hld.indexInPath(path, v);
        segTrees[path].update(index, cost);
    }

    void updateRangeVertex(int u, int v, int lca, T cost) {
        if (lca == hld.mRoot) {
            updateRangeBottomup(lca, u, cost);
            updateRangeBottomup(lca, v, cost);
            rootValue = mergeOp(rootValue, cost);
        } else {
            updateRangeBottomup(hld.P[0][lca], u, cost);
            updateRangeBottomup(lca, v, cost);
        }
    }

    void updateRangeVertex(int u, int v, T cost) {
        return updateRangeVertex(u, v, hld.findLCA(u, v), cost);
    }

    T queryVertex(int u, int v, int lca) {
        if (lca == hld.mRoot)
            return mergeOp(mergeOp(queryBottomup(lca, u), queryBottomup(lca, v)), rootValue);
        else
            return mergeOp(queryBottomup(hld.P[0][lca], u), queryBottomup(lca, v));
    }

    T queryVertex(int u, int v) {
        return queryVertex(u, v, hld.findLCA(u, v));
    }

protected:
    // PRECONDITION: u is an ancestor of v
    // update values in range (u, v]
    void updateRangeBottomup(int u, int v, T value) {
        if (u == v)
            return;

        if (hld.nodeToPath[u] == hld.nodeToPath[v]) {
            int path = hld.nodeToPath[u];

            int first = hld.indexInPath(path, v);
            int last = hld.indexInPath(path, u) - 1;
            return segTrees[path].updateRange(first, last, value);
        }

        int path = hld.nodeToPath[v];
        int topOfPath = hld.paths[path].back();

        int first = hld.indexInPath(path, v);
        segTrees[path].updateRange(first, int(hld.paths[path].size()) - 2, value);

        updateRangeBottomup(u, topOfPath, value);
    }

    // PRECONDITION: u is an ancestor of v
    // query in range (u, v]
    T queryBottomup(int u, int v) {
        if (u == v)
            return defaultValue;

        if (hld.nodeToPath[u] == hld.nodeToPath[v]) {
            int path = hld.nodeToPath[u];

            int first = hld.indexInPath(path, v);
            int last = hld.indexInPath(path, u) - 1;
            return segTrees[path].query(first, last);
        }

        int path = hld.nodeToPath[v];
        int topOfPath = hld.paths[path].back();

        int first = hld.indexInPath(path, v);
        return mergeOp(queryBottomup(u, topOfPath), segTrees[path].query(first, int(hld.paths[path].size()) - 2));
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline SimpleHLDPathQueryLazy<T, MergeOp, BlockOp> makeSimpleHLDPathQueryLazy(HLD& hld, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return SimpleHLDPathQueryLazy<T, MergeOp, BlockOp>(hld, mop, bop, dfltValue);
}
