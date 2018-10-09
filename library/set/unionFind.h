#pragma once

struct UnionFind {
    vector<int> parent;
    vector<int> rank;

    UnionFind() {
    }

    explicit UnionFind(int N) {
        init(N);
    }

    void init(int N) {
        parent.resize(N);
        rank.assign(N, 0);
        for (int i = 0; i < N; i++)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] == x)
            return x;
        return parent[x] = find(parent[x]);
    }

    int merge(int x, int y) {
        int xset = find(x);
        int yset = find(y);
        if (xset == yset)
            return xset;

        if (rank[xset] < rank[yset]) {
            parent[xset] = yset;
            return yset;
        } else {
            parent[yset] = xset;
            if (rank[xset] == rank[yset])
                rank[xset]++;
            return xset;
        }
    }
};

// Simplified Union-Find for Disjoint-Set
struct DSU {
    vector<int> parent;

    DSU() {
    }

    explicit DSU(int N) {
        init(N);
    }

    void init(int N) {
        parent.resize(N);
        for (int i = 0; i < N; i++)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] == x)
            return x;
        return parent[x] = find(parent[x]);
    }

    int merge(int x, int y) {
        int xset = find(x);
        int yset = find(y);
        if (xset != yset)
            parent[yset] = xset;
        return xset;
    }
};
