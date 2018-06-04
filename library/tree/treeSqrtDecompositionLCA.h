#pragma once

struct TreeSqrtDecompositionLCA {
    int                 N;          // the number of vertex
    int                 sqrtN;      // sqrt(N)

    vector<vector<int>> edges;      // edges (vertex number)

    vector<int>         level;      // level

    vector<int>         parent;     // 
    vector<int>         jump;       // to a ancestor

    TreeSqrtDecompositionLCA()
        : N(0), sqrtN(0) {
    }

    explicit TreeSqrtDecompositionLCA(int n, int sqrtN = 0) {
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
    }

    //--- build

    // before calling build()
    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    // O(N)
    void build(int root) {
        dfsBuild(root, -1);
    }

    //--- lca

    // O(sqrt(H))
    int lca(int u, int v) const {
        while (jump[u] != jump[v]) {
            if (level[u] > level[v])
                swap(u, v);
            v = jump[v];
        }
        return lcaNaive(u, v);
    }

    // dist >= 0, O(sqrt(H))
    int climb(int u, int dist) const {
        int p = jump[u];
        while (p >= 0 && level[u] - level[p] <= dist) {
            dist -= level[u] - level[p];
            u = p;
            p = jump[u];
        }
        return climbNaive(u, dist);
    }

protected:
    void dfsBuild(int u, int par) {
        parent[u] = par;
        if (par >= 0)
            level[u] = level[par] + 1;

        if (level[u] % sqrtN == 0)
            jump[u] = parent[u];
        else
            jump[u] = jump[par];

        for (auto v : edges[u]) {
            if (v != par)
                dfsBuild(v, u);
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
};
