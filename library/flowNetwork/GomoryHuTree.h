#pragma once

template <typename T, const T INF = 0x3f3f3f3f>
struct SparseTableOnGomoryHuTree {
    int                 N;          // the number of vertex
    int                 logN;       // log2(N - 1) + 2

    vector<vector<int>> P;          // P[0][n] points to the parent
                                    // parent & acestors
    vector<int>         level;      // depth (root is 0)

    // sparse table
    vector<vector<T>>   value;
    vector<int>         H;

    SparseTableOnGomoryHuTree(vector<vector<pair<int, T>>>& edges, int root) {
        build(edges, root);
    }

    void build(vector<vector<pair<int, T>>>& edges, int root) {
        init(int(edges.size()));

        dfsAssignValue(edges, root, -1);

        vector<int> ancestors;
        dfsBuild(edges, root, ancestors);
        buildLcaTable();
    }

    //--- query (LCA)

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

    // O(log(H)), inclusive
    T query(int u, int v) const {
        //assert(u != v);

        if (level[u] > level[v])
            swap(u, v);

        int lca = findLCA(u, v);
        if (lca == u)
            return querySub(climbTree(v, level[v] - level[lca] - 1), v);
        else
            return min(querySub(climbTree(u, level[u] - level[lca] - 1), u),
                       querySub(climbTree(v, level[v] - level[lca] - 1), v));
    }

private:
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

        P = vector<vector<int>>(logN, vector<int>(N));
        level.assign(N, 0);

        // sparse table
        H.resize(N + 1);
        H[1] = 0;
        for (int i = 2; i < int(H.size()); i++)
            H[i] = H[i >> 1] + 1;

        value.assign(H.back() + 1, vector<T>(N, INF));
    }

    void dfsAssignValue(vector<vector<pair<int, T>>>& edges, int u, int parent) {
        for (auto& it : edges[u]) {
            if (it.first == parent)
                continue;
            dfsAssignValue(edges, it.first, u);
            value[0][it.first] = it.second;
        }
    }

    void dfsBuild(vector<vector<pair<int, T>>>& edges, int u, vector<int>& ancestors) {
        int parent = ancestors.empty() ? -1 : ancestors.back();
        P[0][u] = parent;

        {
            int i = int(ancestors.size());
            int level = H[i + 1];
            for (int j = 0; j < level; j++)
                value[j + 1][u] = min(value[j][ancestors[i - (1 << j)]], value[j][u]);
        }

        ancestors.push_back(u);
        for (auto& it : edges[u]) {
            int v = it.first;
            if (v == parent)
                continue;

            level[v] = level[u] + 1;
            dfsBuild(edges, v, ancestors);
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
    // O(log(H)), inclusive
    T querySub(int u, int v) const {
        int uLevel = level[u];
        int vLevel = level[v];
        if (vLevel < uLevel)
            return INF;

        int length = vLevel - uLevel + 1;
        int k = H[vLevel - uLevel + 1];
        const vector<T>& mink = value[k];
        return min(mink[climbTree(v, length - (1 << k))], mink[v]);
    }
};

template <typename T, const T INF = 0x3f3f3f3f>
struct GomoryHuTree {
    int N;
    vector<vector<pair<int, T>>> tree; // (v, flow)

    explicit GomoryHuTree(int n) : N(n), tree(N) {
    }

    // O(N)
    T calcMaxFlow(int s, int t, T maxFlow = INF) {
        return dfs(s, t, -1, maxFlow);
    }

    SparseTableOnGomoryHuTree<T, INF> buildSparseTable() {
        return SparseTableOnGomoryHuTree<T, INF>(tree, 0);
    }

private:
    T dfs(int u, int t, int parent, T maxFlow) {
        if (u == t)
            return maxFlow;

        T flow = INF;
        for (auto& e : tree[u]) {
            int v = e.first;
            if (v != parent)
                flow = min(flow, dfs(v, t, u, min(maxFlow, e.second)));
        }

        return flow;
    }
};

// for undirected graph
template <typename T>
struct GomoryHuTreeBuilder {
    static const T INF = T(0x3f3f3f3f);

    struct Edge {
        int to;                 // v
        int revIndex;           // for (v -> u)

        T   flow;
        T   capacity;           // 
    };

    int N;                      // the number of vertices
    vector<vector<Edge>> edges; // graph

    GomoryHuTreeBuilder() : N(0) {
    }

    explicit GomoryHuTreeBuilder(int n) : N(n), edges(N + 1) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<Edge>>(N + 1);
    }

    // add edges to a undirected graph
    void addEdge(int u, int v, T capacity) {
        int uN = int(edges[u].size());
        int vN = int(edges[v].size());
        edges[u].push_back(Edge{ v, vN, 0, capacity });
        edges[v].push_back(Edge{ u, uN, 0, capacity });
    }

    void clearFlow() {
        for (auto& vec : edges) {
            for (auto& e : vec)
                e.flow = 0;
        }
    }

    // O(V^3 * flow) or O(VE * flow)
    GomoryHuTree<T> build() {
        vector<int> parent(N, 0);
        vector<T> maxFlow(N);
        vector<pair<int, int>> prev(N); // (u, edge index in u's edges)

        for (int s = 1; s < N; ++s) {
            T totalFlow = 0;
            clearFlow();

            int t = parent[s];
            while (true) {
                queue<int> Q;
                Q.push(s);

                fill(prev.begin(), prev.end(), make_pair(-1, -1));
                prev[s].first = -2;
                while (!Q.empty() && prev[t].first == -1) {
                    int u = Q.front();
                    Q.pop();

                    for (int i = 0; i < int(edges[u].size()); i++) {
                        Edge& e = edges[u][i];
                        int v = e.to;
                        if (prev[v].first == -1 && (e.capacity - e.flow) > 0) {
                            prev[v] = make_pair(u, i);
                            Q.push(v);
                        }
                    }
                }
                if (prev[t].first == -1)
                    break;

                T flow = INF;
                for (int v = t; v != s; v = prev[v].first) {
                    int u = prev[v].first;
                    Edge& e = edges[u][prev[v].second];
                    flow = min(flow, e.capacity - e.flow);
                }
                for (int v = t; v != s; v = prev[v].first) {
                    int u = prev[v].first;
                    Edge& e = edges[u][prev[v].second];
                    e.flow += flow;
                    edges[e.to][e.revIndex].flow += flow;   //+
                }
                totalFlow += flow;
            }

            maxFlow[s] = totalFlow;
            for (int u = 0; u < N; ++u) {
                if (u != s && prev[u].first != -1 && parent[u] == t)
                    parent[u] = s;
            }
            if (prev[parent[t]].first != -1) {
                parent[s] = parent[t];
                parent[t] = s;
                maxFlow[s] = maxFlow[t];
                maxFlow[t] = totalFlow;
            }
        }

        GomoryHuTree<T> res(N);
        for (int u = 0; u < N; ++u) {
            if (u != parent[u]) {
                res.tree[u].emplace_back(parent[u], maxFlow[u]);
                res.tree[parent[u]].emplace_back(u, maxFlow[u]);
            }
        }

        return res;
    }
};
