#pragma once

// Simplified HLDPathQuery with BITs
template <typename T>
struct SimpleHLDPathQueryBIT {
    HLD& hld;

    vector<FenwickTreeMultAdd<T>> BIT;

    explicit SimpleHLDPathQueryBIT(HLD& hld) : hld(hld) {
        // no action
    }

    void build() {
        BIT.clear();
        BIT.reserve(hld.paths.size());

        // make segment trees on all heavy path
        for (const auto& path : hld.paths) {
            int m = int(path.size()) - 1;
            BIT.push_back(FenwickTreeMultAdd<T>(m + 1));
        }
    }

    //--- edge-based operations

    // add a value to an edge(u-v)
    void add(int u, int v, T cost) {
        if (u >= 0 && hld.tree.P[0][u] == v)
            swap(u, v);

        int path = hld.nodeToPath[v];
        int index = hld.indexInPath(path, v);
        BIT[path].add(index, cost);
    }

    void addRange(int u, int v, int lca, T cost) {
        addRangeBottomup(lca, u, cost);
        addRangeBottomup(lca, v, cost);
    }

    void addRange(int u, int v, T cost) {
        return addRange(u, v, hld.tree.findLCA(u, v), cost);
    }

    T query(int u, int v, int lca) const {
        return queryBottomup(lca, u) + queryBottom(lca, v);
    }

    T query(int u, int v) const {
        return query(u, v, hld.tree.findLCA(u, v));
    }

    //--- vertex-based operations

    T rootValue = 0;

    void addVertex(int v, T cost) {
        if (v == hld.root) {
            rootValue += cost;
            return;
        }
        int path = hld.nodeToPath[v];
        int index = hld.indexInPath(path, v);
        BIT[path].add(index, cost);
    }

    void addRangeVertex(int u, int v, int lca, T cost) {
        if (lca == hld.root) {
            addRangeBottomup(lca, u, cost);
            addRangeBottomup(lca, v, cost);
            rootValue += cost;
        } else {
            addRangeBottomup(hld.tree.P[0][lca], u, cost);
            addRangeBottomup(lca, v, cost);
        }
    }

    void addRangeVertex(int u, int v, T cost) {
        return addRangeVertex(u, v, hld.tree.findLCA(u, v), cost);
    }

    T queryVertex(int u, int v, int lca) const {
        if (lca == hld.root)
            return queryBottomup(lca, u) + queryBottomup(lca, v) + rootValue;
        else
            return queryBottomup(hld.tree.P[0][lca], u) + queryBottomup(lca, v);
    }

    T queryVertex(int u, int v) const {
        return queryVertex(u, v, hld.tree.findLCA(u, v));
    }

protected:
    void addRangeBottomup(int u, int v, T delta) {
        if (u == v)
            return;

        if (hld.nodeToPath[u] == hld.nodeToPath[v]) {
            int path = hld.nodeToPath[u];

            int first = hld.indexInPath(path, v);
            int last = hld.indexInPath(path, u) - 1;
            return BIT[path].addRange(first, last, delta);
        }

        int path = hld.nodeToPath[v];
        int topOfPath = hld.paths[path][0];

        int first = hld.indexInPath(path, v);
        BIT[path].addRange(first, int(hld.paths[path].size()) - 2, delta);

        addRangeBottomup(u, topOfPath, delta);
    }

    // PRECONDITION: u is an ancestor of v
    // query in range (u, v]
    T queryBottomup(int u, int v) const {
        if (u == v)
            return 0;

        if (hld.nodeToPath[u] == hld.nodeToPath[v]) {
            int path = hld.nodeToPath[u];

            int first = hld.indexInPath(path, v);
            int last = hld.indexInPath(path, u) - 1;
            return BIT[path].sumRange(first, last);
        }

        int path = hld.nodeToPath[v];
        int topOfPath = hld.paths[path].back();

        int first = hld.indexInPath(path, v);
        return queryBottomup(u, topOfPath) + BIT[path].sumRange(first, int(hld.paths[path].size()) - 2);
    }
};
