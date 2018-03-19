#pragma once

//--- HLDPathQuery with BITs --------------------------------------------------

template <typename T>
struct HLDPathQueryBIT {
    HeavyLightDecomposition& hld;

    vector<FenwickTreeMultAdd<T>> BIT;

    explicit HLDPathQueryBIT(HeavyLightDecomposition& hld) : hld(hld) {
        // no action
    }

    void build() {
        BIT.clear();
        BIT.reserve(hld.heavyPaths.size());

        // make segment trees on all heavy path
        for (const auto& path : hld.heavyPaths) {
            int m = (int)path.size() - 1;
            BIT.push_back(FenwickTreeMultAdd<T>(m));
        }
    }

    // update a value of an edge(u-v)
    void add(int u, int v, T cost) {
        if (u >= 0 && hld.tree.P[0][u] == v)
            swap(u, v);
        //assert(gP[0][v] == u);

        int path = hld.heavyPathIndex[v];
        int index = hld.indexInPath(path, v);
        BIT[path].add(index, cost);
    }

    void addRange(int u, int v, T cost) {
        int lca = hld.tree.findLCA(u, v);
        addRangeTopdown(lca, u, cost);
        addRangeTopdown(lca, v, cost);
    }

    T query(int u, int v) const {
        int lca = hld.tree.findLCA(u, v);
        return queryTopdown(lca, u) + queryTopdown(lca, v);
    }

    //--- to update & query vertexes

    T rootValue = 0;

    // update a value of an edge(parent of v, v) or a vertex(v)
    void addVertex(int v, T cost) {
        if (v == hld.root) {
            rootValue = cost;
            return;
        }

        int path = hld.heavyPathIndex[v];
        int index = hld.indexInPath(path, v);
        BIT[path].add(index, cost);
    }

    void addRangeVertex(int u, int v, T cost) {
        int lca = hld.tree.findLCA(u, v);
        if (lca == hld.root) {
            addRangeTopdown(lca, u, cost);
            addRangeTopdown(lca, v, cost);
            rootValue += cost;
        } else {
            addRangeTopdown(hld.tree.P[0][lca], u, cost);
            addRangeTopdown(lca, v, cost);
        }
    }

    T queryVertex(int u, int v) const {
        int lca = hld.tree.findLCA(u, v);
        if (lca == hld.root)
            return queryTopdown(lca, u) + queryTopdown(lca, v) + rootValue;
        else
            return queryTopdown(hld.tree.P[0][lca], u) + queryTopdown(lca, v);
    }

protected:
    void addRangeTopdown(int u, int v, T delta) {
        if (u == v)
            return;

        if (hld.heavyPathIndex[u] == hld.heavyPathIndex[v]) {
            int path = hld.heavyPathIndex[u];

            int first = hld.indexInPath(path, u) + 1;
            int last = hld.indexInPath(path, v);
            return BIT[path].addRange(first, last, delta);
        }

        int path = hld.heavyPathIndex[v];
        int topOfPath = hld.heavyPaths[path][0];

        //assert(topOfPath != v);

        int last = hld.indexInPath(path, v);
        BIT[path].addRange(0, last, delta);

        addRangeTopdown(u, topOfPath, delta);
    }

    // return max value a path from u to v (u is an ancestor of v)
    T queryTopdown(int u, int v) const {
        if (u == v)
            return 0;

        if (hld.heavyPathIndex[u] == hld.heavyPathIndex[v]) {
            int path = hld.heavyPathIndex[u];

            int first = hld.indexInPath(path, u) + 1;
            int last = hld.indexInPath(path, v);
            return BIT[path].sumRange(first, last);
        }

        int path = hld.heavyPathIndex[v];
        int topOfPath = hld.heavyPaths[path][0];

        //assert(topOfPath != v);

        int last = hld.indexInPath(path, v);
        return queryTopdown(u, topOfPath) + BIT[path].sumRange(0, last);
    }
};
