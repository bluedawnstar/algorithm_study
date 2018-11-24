#pragma once

struct UnionFindWithCount {
    vector<int> parent;
    vector<int> rank;
    vector<int> count;

    UnionFindWithCount() {
    }

    explicit UnionFindWithCount(int N) {
        init(N);
    }

    void init(int N) {
        parent.resize(N);
        rank.assign(N, 0);
        count.assign(N, 1);
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
            count[yset] += count[xset];
            return yset;
        } else {
            parent[yset] = xset;
            if (rank[xset] == rank[yset])
                rank[xset]++;
            count[xset] += count[yset];
            return xset;
        }
    }


    int getCount(int x) {
        return count[find(x)];
    }
};
