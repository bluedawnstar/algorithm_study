#pragma once

template <typename T>
struct DynamicBlockTreePathSum {
    int                 N;              // the number of vertex
    int                 sqrtN;          // sqrt(N) or sqrt(H)

    vector<set<int>>    edges;          // edges (vertex number)

    vector<int>         level;          // level (root is 0)

    vector<int>         parent;         // 
    vector<int>         jump;           // to a ancestor

    vector<T>           values;         // 
    vector<T>           sqrtValues;     // 

    DynamicBlockTreePathSum() : N(0), sqrtN(0) {
    }

    explicit DynamicBlockTreePathSum(int n, int sqrtN = 0) {
        init(n, sqrtN);
    }

    void init(int _n, int _sqrtN = 0) {
        N = _n;
        sqrtN = _sqrtN;
        if (sqrtN <= 0)
            sqrtN = int(sqrt(N));

        edges = vector<set<int>>(N);
        level.assign(N, 0);
        parent.assign(N, -1);
        jump.assign(N, -1);

        values.assign(N, 0);
        sqrtValues.assign(N, 0);
    }

    //--- build

    // before calling build()
    void addEdge(int u, int v) {
        edges[u].insert(v);
        edges[v].insert(u);
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


    // worst case O(N)
    // PRECONDITION: u must be not root
    // PRECONDITION: newParent >= 0
    void changeParent(int u, int newParent) {
        if (newParent == parent[u])
            return;

        if (parent[u] >= 0) {
            edges[u].erase(parent[u]);
            edges[parent[u]].erase(u);
        }

        addEdge(u, newParent);
        parent[u] = newParent;
        dfsBuild(u, newParent);
    }

    //--- query

    // O(sqrt(N))
    T queryToRoot(int u) const {
        T res = sqrtValues[u];
        int p = jump[u];
        while (p >= 0) {
            res += sqrtValues[p];
            p = jump[p];
        }
        return res;
    }

    // O(sqrt(N))
    T queryToAncestor(int u, int ancestor) const {
        T res = 0;
        while (jump[u] != jump[ancestor]) {
            res += sqrtValues[u];
            u = jump[u];
        }
        res += sqrtValues[u] - sqrtValues[ancestor] + values[ancestor];
        return res;
    }

    // dist >= 0, O(sqrt(N))
    T queryTowardRoot(int u, int dist) const {
        T res = 0;
        int p = jump[u];
        while (p >= 0 && (level[u] - level[p]) <= dist) {
            dist -= (level[u] - level[p]);
            res += sqrtValues[u];
            u = p;
            p = jump[u];
        }
        res += queryTowardRootNaive(u, dist);
        return res;
    }

    // O(sqrt(N))
    T query(int u, int v) const {
        if (u == v)
            return values[u];

        T res = 0;
        while (jump[u] != jump[v]) {
            if (level[u] > level[v])
                swap(u, v);
            res += sqrtValues[v];
            v = jump[v];
        }

        int lc = lcaNaive(u, v);
        res += sqrtValues[u] + sqrtValues[v] - 2 * sqrtValues[lc] + values[lc];

        return res;
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
            sqrtValues[u] = sqrtValues[par] + values[u];
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
            sqrtValues[u] = sqrtValues[par] + values[u];

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
