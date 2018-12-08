#pragma once

// DSU of 1D segments
struct UnionFindSegment {
    vector<int> parent;
    vector<int> rank;

    vector<int> left;
    vector<int> right;

    UnionFindSegment() {
    }

    explicit UnionFindSegment(int N) {
        init(N);
    }

    void init(int N) {
        parent.resize(N);
        rank.assign(N, 0);
        left.resize(N);
        right.resize(N);
        for (int i = 0; i < N; i++) {
            parent[i] = -1;
            left[i] = i;
            right[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] == x)
            return x;
        return parent[x] = find(parent[x]);
    }


    bool isAdded(int x) const {
        return parent[x] != -1;
    }

    void add(int x) {
        parent[x] = x;
        if (x - 1 >= 0 && parent[x - 1] != -1)
            merge(x - 1, x);
        if (x + 1 < int(parent.size()) && parent[x + 1] != -1)
            merge(x, x + 1);
    }

    pair<int, int> getSegment(int x) {
        if (!isAdded(x))
            return { -1, -1 };
        
        x = find(x);
        return { left[x], right[x] };
    }

private:
    int merge(int x, int y) {
        int xset = find(x);
        int yset = find(y);
        if (xset == yset)
            return xset;

        if (rank[xset] < rank[yset]) {
            parent[xset] = yset;
            swap(xset, yset);
        } else {
            parent[yset] = xset;
            if (rank[xset] == rank[yset])
                rank[xset]++;
        }
        left[xset] = min(left[xset], left[yset]);
        right[xset] = max(right[xset], right[yset]);

        return xset;
    }
};
