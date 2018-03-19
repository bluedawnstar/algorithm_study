#pragma once

/*
    <How to use>
    init() -> build() -> climb() / findLCA()
*/
struct LcaSchieberVishkin {
    int                 N;
    vector<int>         parent;
    vector<int>         preorder;   // visit time of nodes
    vector<int>         head;       // 
    vector<int>         path;       // 
    vector<int>         A;          // all
    int                 time;

    LcaSchieberVishkin() {
        N = 0;
        time = 0;
    }

    LcaSchieberVishkin(vector<vector<int>>& edges, int root) {
        build(edges, root);
    }

    // O(N)
    void build(vector<vector<int>>& edges, int root) {
        init((int)edges.size());

        time = 0;
        dfs1(edges, root, -1);
        dfs2(edges, root, -1, 0);
    }


    // O(1)
    int lca(int x, int y) const {
        int hb = (path[x] == path[y]) ? lsb(path[x]) : msb(path[x] ^ path[y]);
        int hz = lsb(A[x] & A[y] & (~hb + 1));
        int ex = enterIntoStrip(x, hz);
        int ey = enterIntoStrip(y, hz);
        return preorder[ex] < preorder[ey] ? ex : ey;
    }

private:
    void init(int N) {
        this->N = N;
        parent.resize(N);
        preorder.resize(N);
        path.resize(N);
        head.resize(N);
        A.resize(N);
        time = 0;
    }

    // p: parent node of u
    void dfs1(vector<vector<int>>& edges, int u, int p) {
        parent[u] = p;
        path[u] = preorder[u] = time++;
        for (int v : edges[u]) {
            if (v == p)
                continue;
            dfs1(edges, v, u);
            if (lsb(path[v]) > lsb(path[u]))
                path[u] = path[v];
        }
        head[path[u]] = u;
    }

    // p: parent node of u
    void dfs2(vector<vector<int>>& edges, int u, int p, int up) {
        A[u] = up | lsb(path[u]);
        for (int v : edges[u]) {
            if (v == p)
                continue;
            dfs2(edges, v, u, A[u]);
        }
    }

    int enterIntoStrip(int x, int hz) const {
        if (lsb(path[x]) == hz)
            return x;
        int hw = msb(A[x] & (hz - 1));
        return parent[head[path[x] & (~hw + 1) | hw]];
    }


    static int clz(int x) {
#ifndef __GNUC__
        return (int)__lzcnt((unsigned)x);
#else
        return __builtin_clz((unsigned)x);
#endif
    }

    static int lsb(int x) {
        return x & -x;
    }

    static int msb(int x) {
        if (x == 0)
            return 0;
        return 1 << (sizeof(int) * 8 - 1 - clz(x));
    }
};
