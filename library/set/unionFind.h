#pragma once

class UnionFind {
public:
    UnionFind() {
    }

    explicit UnionFind(int N) : parent(N, 0), rank(N, 0) {
        for (int i = 0; i < N; i++)
            parent[i] = i;
    }

    void init(int N) {
        parent.resize(N);
        rank.assign(N, 0);
        for (int i = 0; i < N; i++)
            parent[i] = i;
    }


    int findSet(int x) {
        if (parent[x] == x)
            return x;
        return parent[x] = findSet(parent[x]);
    }

    int unionSet(int x, int y) {
        int xset = findSet(x);
        int yset = findSet(y);
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

private:
    vector<int> parent;
    vector<int> rank;
};
