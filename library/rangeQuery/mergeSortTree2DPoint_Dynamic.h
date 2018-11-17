#pragma once

struct DynamicMergeSortTree2DPoint {
    int N;
    vector<vector<int>> tree;   // x -> { y0, y1, ... }, 0 <= x < N

    explicit DynamicMergeSortTree2DPoint(int maxX) : N(maxX + 1), tree(N * 2) {
    }


    // 0 <= x < N, O(NlogN)
    void add(int x, int y) {
        tree[N + x].emplace(upper_bound(tree[N + x].begin(), tree[N + x].end(), y), y);

        for (int i = (x + N) >> 1; i > 0; i >>= 1)
            tree[i].emplace(upper_bound(tree[i].begin(), tree[i].end(), y), y);
    }

    // 0 <= x < N, O(NlogN)
    void remove(int x, int y) {
        auto it = find(tree[N + x].begin(), tree[N + x].end(), y);
        if (it == tree[N + x].end())
            return;

        tree[N + x].erase(it);

        for (int i = (x + N) >> 1; i > 0; i >>= 1) {
            auto it = find(tree[i].begin(), tree[i].end(), y);
            if (it != tree[i].end())
                tree[i].erase(it);
        }
    }

    // inclusive, O((logN)^2)
    int count(int x1, int y1, int x2, int y2) const {
        int res = 0;

        for (int L = x1 + N, R = x2 + N; L <= R; L >>= 1, R >>= 1) {
            if (L & 1)
                res += getCount(tree[L++], y1, y2);
            if ((R & 1) == 0)
                res += getCount(tree[R--], y1, y2);
        }

        return res;
    }

private:
    int getCount(const vector<int>& v, int y1, int y2) const {
        return int(upper_bound(v.begin(), v.end(), y2) - lower_bound(v.begin(), v.end(), y1));
    }
};
