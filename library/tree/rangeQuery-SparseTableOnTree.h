#pragma once

/*
<How to use>
    1) create and initialize a tree
    2) call addXXX() / setXXX() to construct the tree
    3) call build()
    4) call query functions
*/

template <typename T, typename MergeOp = function<T(T, T)>>
struct SparseTableOnTree {
    int                 N;          // the number of vertex
    int                 logN;       // log2(N - 1) + 2

    vector<vector<int>> edges;      // edges (vertex number)
    vector<vector<int>> P;          // P[0][n] points to the parent
                                    // parent & acestors
    vector<int>         level;      // depth (root is 0)

    // sparse table
    vector<vector<T>>   value;
    vector<int>         H;
    T                   defaultValue;
    MergeOp             mergeOp;

    explicit SparseTableOnTree(MergeOp op, T dfltValue = T())
        : N(0), logN(0), mergeOp(op), defaultValue(dfltValue) {
    }

    explicit SparseTableOnTree(int n, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        init(n, 0);
    }

    void init(int _n, int _logN = 0) {
        N = _n;
        logN = _logN;
        if (logN <= 0) {
#ifndef __GNUC__
            logN = _lzcnt_u32(1u) - _lzcnt_u32((unsigned int)(N - 1)) + 2;
#else
            logN = __builtin_clz(1u) - __builtin_clz((unsigned int)(N - 1)) + 2;
#endif
        }

        edges = vector<vector<int>>(N);
        P = vector<vector<int>>(logN, vector<int>(N));
        level.assign(N, 0);

        // sparse table
        H.resize(N + 1);
        H[1] = 0;
        for (int i = 2; i < int(H.size()); i++)
            H[i] = H[i >> 1] + 1;

        value.assign(H.back() + 1, vector<T>(N, defaultValue));
    }


    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    void addEdgeDirected(int u, int v) {
        edges[u].push_back(v);
    }

    void setValue(int u, T val) {
        value[0][u] = val;
    }


    // O(NlogN)
    void build(int root) {
        vector<int> ancestors;
        dfsBuild(root, ancestors);
        buildLcaTable();
    }

    // O(NlogN)
    void build(const T val[], int n, int root) {
        for (int i = 0; i < n; i++)
            value[0][i] = val[i];

        vector<int> ancestors;
        dfsBuild(root, ancestors);
        buildLcaTable();
    }

    // O(NlogN)
    void build(const vector<T>& val, int root) {
        build(&val[0], int(val.size()), root);
    }

    //--- query (LCA)

    // O(logH)
    int climbTree(int node, int dist) const {
        if (dist <= 0)
            return node;

        for (int i = 0; dist > 0; i++) {
            if (dist & 1)
                node = P[i][node];
            dist >>= 1;
        }

        return node;
    }

    // O(logH)
    int findLCA(int A, int B) const {
        if (level[A] < level[B])
            swap(A, B);

        A = climbTree(A, level[A] - level[B]);

        if (A == B)
            return A;

        int bitCnt = 0;
        for (int x = level[A]; x; x >>= 1)
            bitCnt++;

        for (int i = bitCnt - 1; i >= 0; i--) {
            if (P[i][A] > 0 && P[i][A] != P[i][B]) {
                A = P[i][A];
                B = P[i][B];
            }
        }

        return P[0][A];
    }

    //--- query (sparse table)

    // O(logH), inclusive
    T query(int u, int v) const {
        if (level[u] > level[v])
            swap(u, v);

        int lca = findLCA(u, v);
        if (lca == u)
            return querySub(u, v);
        else
            return mergeOp(querySub(lca, u), querySub(lca, v));
    }

    // O((logH)^2), inclusive
    T queryNoOverlap(int u, int v) const {
        if (level[u] > level[v])
            swap(u, v);

        int lca = findLCA(u, v);
        if (lca == u)
            return queryNoOverlapSub(u, v);
        else
            return mergeOp(queryNoOverlapSub(climbTree(u, level[u] - level[lca] - 1), u), queryNoOverlapSub(lca, v));
    }


private:
    void dfsBuild(int u, vector<int>& ancestors) {
        int parent = ancestors.empty() ? -1 : ancestors.back();
        P[0][u] = parent;

        {
            int i = int(ancestors.size());
            int level = H[i + 1];
            for (int j = 0; j < level; j++)
                value[j + 1][u] = mergeOp(value[j][ancestors[i - (1 << j)]], value[j][u]);
        }

        ancestors.push_back(u);
        for (int v : edges[u]) {
            if (v == parent)
                continue;

            level[v] = level[u] + 1;
            dfsBuild(v, ancestors);
        }
        ancestors.pop_back();
    }

    void buildLcaTable() {
        for (int i = 1; i < logN; i++) {
            for (int j = 0; j < N; j++) {
                int pp = P[i - 1][j];
                P[i][j] = pp < 0 ? pp : P[i - 1][pp];
            }
        }
    }


    // PRECONDITION: u is an ancestor of v
    // O(logH), inclusive
    T querySub(int u, int v) const {
        int uLevel = level[u];
        int vLevel = level[v];
        if (vLevel < uLevel)
            return defaultValue;

        int length = vLevel - uLevel + 1;
        int k = H[vLevel - uLevel + 1];
        const vector<T>& mink = value[k];
        return mergeOp(mink[climbTree(v, length - (1 << k))], mink[v]);
    }

    // PRECONDITION: u is an ancestor of v
    // O((logH)^2), inclusive
    T queryNoOverlapSub(int u, int v) const {
        int uLevel = level[u];
        int vLevel = level[v];
        if (vLevel < uLevel)
            return defaultValue;

        T res = defaultValue;

        int length = vLevel - uLevel + 1;
        while (length) {
#ifndef __GNUC__
            int i = int(_tzcnt_u32(length));
#else
            int i = __builtin_ctz(length);
#endif
            length &= length - 1;

            int node = climbTree(v, length);
            res = mergeOp(res, value[i][node]);
        }

        return res;
    }
};

template <typename T, typename MergeOp>
inline SparseTableOnTree<T, MergeOp> makeSparseTableOnTree(MergeOp op, T dfltValue = T()) {
    return SparseTableOnTree<T, MergeOp>(op, dfltValue);
}

template <typename T, typename MergeOp>
inline SparseTableOnTree<T, MergeOp> makeSparseTableOnTree(int n, MergeOp op, T dfltValue = T()) {
    return SparseTableOnTree<T, MergeOp>(n, op, dfltValue);
}
