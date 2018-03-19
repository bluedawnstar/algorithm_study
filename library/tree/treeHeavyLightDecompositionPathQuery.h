#pragma once

//--- HLDPathQuery with Segment Tree ------------------------------------------

template <typename T, typename BinOp = function<T(T, T)>>
struct HLDPathQuery {
    HeavyLightDecomposition& hld;

    typedef SegmentTree<T, BinOp> SegTreeT;

    T                   defaultValue;
    BinOp               mergeOp;
    vector<SegTreeT>    segTrees;

    explicit HLDPathQuery(HeavyLightDecomposition& hld, T dflt = T()) : hld(hld), mergeOp(), defaultValue(dflt) {
        // no action
    }

    HLDPathQuery(HeavyLightDecomposition& hld, BinOp op, T dflt = T()) : hld(hld), mergeOp(op), defaultValue(dflt) {
        // no action
    }

    void build() {
        segTrees.clear();
        segTrees.reserve(hld.heavyPaths.size());

        // make segment trees on all heavy path
        for (const auto& path : hld.heavyPaths) {
            int m = (int)path.size() - 1;
            segTrees.push_back(SegTreeT(m, mergeOp, defaultValue));
        }
    }

    void build(T initValue) {
        segTrees.clear();
        segTrees.reserve(hld.heavyPaths.size());

        // make segment trees on all heavy path
        for (const auto& path : hld.heavyPaths) {
            int m = (int)path.size() - 1;
            segTrees.push_back(SegTreeT(m, mergeOp, defaultValue));
            for (int i = 0; i < m; ++i)
                segTrees.back().update(i, initValue);
        }
    }


    // update a value of an edge(u-v)
    void update(int u, int v, T cost) {
        if (u >= 0 && hld.tree.P[0][u] == v)
            swap(u, v);
        //assert(gP[0][v] == u);

        int path = hld.heavyPathIndex[v];
        int index = hld.indexInPath(path, v);
        segTrees[path].update(index, cost);
    }

    T query(int u, int v) const {
        int lca = hld.tree.findLCA(u, v);
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

    T queryVertex(int u, int v) {
        int lca = hld.tree.findLCA(u, v);
        if (lca == hld.mRoot)
            return mergeOp(mergeOp(queryTopdown(lca, u), queryTopdown(lca, v)), rootValue);
        else
            return mergeOp(queryTopdown(hld.tree.P[0][lca], u), queryTopdown(lca, v));
    }

protected:
    // return max value a path from u to v (u is an ancestor of v)
    T queryTopdown(int u, int v) const {
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

template <typename T, typename BinOp>
HLDPathQuery<T, BinOp> makeHLDPathQuery(HeavyLightDecomposition& hld, BinOp op, T dfltValue = T()) {
    return HLDPathQuery<T, BinOp>(hld, op, dfltValue);
}
