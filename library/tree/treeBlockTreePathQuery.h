#pragma once

template <typename T, typename MergeOp = function<T(T, T)>>
struct BlockTreePathQuery {
    int                 N;              // the number of vertex
    int                 sqrtN;          // sqrt(N) or sqrt(H)

    vector<vector<int>> edges;          // edges (vertex number)

    vector<int>         level;          // level (root is 0)

    vector<int>         parent;         // 
    vector<int>         jump;           // to a ancestor

    vector<T>           values;         // 
    vector<T>           sqrtValues;     // 
    T                   defaultValue;   // 
    MergeOp             mergeOp;        // 

    explicit BlockTreePathQuery(const MergeOp& op, T dflt = T())
        : N(0), sqrtN(0), mergeOp(op), defaultValue(dflt) {
    }

    BlockTreePathQuery(int n, const MergeOp& op, T dflt = T(), int sqrtN = 0)
        : mergeOp(op), defaultValue(dflt) {
        init(n, sqrtN);
    }

    void init(int _n, int _sqrtN = 0) {
        N = _n;
        sqrtN = _sqrtN;
        if (sqrtN <= 0)
            sqrtN = (int)sqrt(N);

        edges = vector<vector<int>>(N);
        level.assign(N, 0);
        parent.assign(N, -1);
        jump.assign(N, -1);

        values.assign(N, defaultValue);
        sqrtValues.assign(N, defaultValue);
    }

    //--- build

    // before calling build()
    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    // before calling build()
    void setInitValue(int u, T value) {
        values[u] = value;
    }

    // O(N)
    void build(int root) {
        dfsBuild(root, -1);
    }

    //--- lca

    // O(sqrt(N))
    int lca(int u, int v) const {
        while (jump[u] != jump[v]) {
            if (level[u] > level[v])
                swap(u, v);
            v = jump[v];
        }
        return lcaNaive(u, v);
    }

    // dist >= 0, O(sqrt(N))
    int climb(int u, int dist) const {
        int p = jump[u];
        while (p >= 0 && (level[u] - level[p]) <= dist) {
            dist -= (level[u] - level[p]);
            u = p;
            p = jump[u];
        }
        return climbNaive(u, dist);
    }

    // kth >= 1, O(sqrt(N))
    // count = valueToCountF(index_of_node, value_or_sqrt_value_of_the_node)
    int climbKth(int u, int kth, const function<int(T)>& valueToCountF) const {
        int p = jump[u];
        int d;
        while (p >= 0 && (d = valueToCountF(sqrtValues[u])) < kth) {
            kth -= d;
            u = p;
            p = jump[u];
        }
        return climbKthNaive(u, kth, valueToCountF);
    }

    //--- update

    // O(sqrt(N))
    void update(int u, T val) {
        if (values[u] != val) {
            values[u] = val;
            dfsUpdateSqrt(u, parent[u]);
        }
    }

    // O(sqrt(N))
    void add(int u, T val) {
        values[u] += val;
        dfsUpdateSqrt(u, parent[u]);
    }

    //--- query

    // O(sqrt(N))
    T queryToRoot(int u) const {
        T res = sqrtValues[u];
        int p = jump[u];
        while (p >= 0) {
            res = mergeOp(res, sqrtValues[p]);
            p = jump[p];
        }
        return res;
    }

    // O(sqrt(N))
    T queryToAncestor(int u, int ancestor) const {
        T res = defaultValue;
        while (jump[u] != jump[ancestor]) {
            res = mergeOp(res, sqrtValues[u]);
            u = jump[u];
        }
        return mergeOp(res, queryToAncestorNaive(u, ancestor));
    }

    // dist >= 0, O(sqrt(N))
    T queryTowardRoot(int u, int dist) const {
        T res = defaultValue;
        int p = jump[u];
        while (p >= 0 && (level[u] - level[p]) <= dist) {
            dist -= (level[u] - level[p]);
            res = mergeOp(res, sqrtValues[u]);
            u = p;
            p = jump[u];
        }
        return mergeOp(res, queryTowardRootNaive(u, dist));
    }

    // O(sqrt(N))
    T query(int u, int v) const {
        if (u == v)
            return values[u];

        T res = defaultValue;
        while (jump[u] != jump[v]) {
            if (level[u] > level[v])
                swap(u, v);
            res = mergeOp(res, sqrtValues[v]);
            v = jump[v];
        }

        int lc = lcaNaive(u, v);
        if (lc == u)
            return mergeOp(res, queryToAncestorNaive(v, u));
        else if (lc == v)
            return mergeOp(res, queryToAncestorNaive(u, v));
        else
            return mergeOp(res, mergeOp(queryToAncestorNaive(u, lc), queryTowardRootNaive(v, level[v] - level[lc] - 1)));
    }

    //--- for accumulative operation
    // Use this functions when MergeOp is 'add' (supporting subtraction)

    // O(sqrt(N))
    T queryToAncestorAccumulative(int u, int ancestor) const {
        T res = defaultValue;
        while (jump[u] != jump[ancestor]) {
            res = mergeOp(res, sqrtValues[u]);
            u = jump[u];
        }
        return mergeOp(res, sqrtValues[u] - sqrtValues[ancestor] + values[ancestor]);
    }

    // O(sqrt(N))
    T queryAccumulative(int u, int v) const {
        if (u == v)
            return values[u];

        T res = defaultValue;
        while (jump[u] != jump[v]) {
            if (level[u] > level[v])
                swap(u, v);
            res = mergeOp(res, sqrtValues[v]);
            v = jump[v];
        }

        int lc = lcaNaive(u, v);
        return mergeOp(res, sqrtValues[u] + sqrtValues[v] - 2 * sqrtValues[lc] + values[lc]);
    }

protected:
    void dfsBuild(int u, int par) {
        parent[u] = par;
        if (par >= 0)
            level[u] = level[par] + 1;

        if (level[u] % sqrtN == 0) {
            jump[u] = parent[u];
            sqrtValues[u] = values[u];
        } else {
            jump[u] = jump[par];
            sqrtValues[u] = mergeOp(sqrtValues[par], values[u]);
        }

        for (auto v : edges[u]) {
            if (v != par)
                dfsBuild(v, u);
        }
    }

    void dfsUpdateSqrt(int u, int par) {
        int d = level[u] % sqrtN;

        if (d == 0)
            sqrtValues[u] = values[u];
        else
            sqrtValues[u] = mergeOp(sqrtValues[par], values[u]);

        if (d == sqrtN - 1)
            return;

        for (auto v : edges[u]) {
            if (v != par)
                dfsUpdateSqrt(v, u);
        }
    }


    int lcaNaive(int u, int v) const {
        if (u == v)
            return u;

        if (level[u] > level[v])
            swap(u, v);

        while (level[u] != level[v])
            v = parent[v];

        while (u != v) {
            u = parent[u];
            v = parent[v];
        }
        return u;
    }

    int climbNaive(int u, int dist) const {
        while (u >= 0 && dist-- > 0)
            u = parent[u];
        return u;
    }

    int climbKthNaive(int u, int kth, const function<int(T)>& valueToCountF) const {
        while (u >= 0 && kth > 0) {
            if (valueToCountF(values[u])) {
                if (--kth == 0)
                    break;
            }
            u = parent[u];
        }
        return u;
    }

    T queryToAncestorNaive(int u, int ancestor) const {
        T res = values[u];
        if (u != ancestor) {
            do {
                u = parent[u];
                res = mergeOp(res, values[u]);
            } while (u != ancestor);
        }
        return res;
    }

    T queryTowardRootNaive(int u, int dist) const {
        T res = values[u];
        if (dist > 0) {
            do {
                u = parent[u];
                res = mergeOp(res, values[u]);
            } while (u >= 0 && --dist > 0);
        }
        return res;
    }
};

template <typename T, typename MergeOp>
inline BlockTreePathQuery<T, MergeOp> makeBlockTreePathQuery(const MergeOp& op, T dfltValue) {
    return BlockTreePathQuery<T, MergeOp>(op, dfltValue);
}

template <typename T, typename MergeOp>
inline BlockTreePathQuery<T, MergeOp> makeBlockTreePathQuery(int size, const MergeOp& op, T dfltValue, int sqrtN = 0) {
    return BlockTreePathQuery<T, MergeOp>(size, op, dfltValue, sqrtN);
}
