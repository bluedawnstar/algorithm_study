#pragma once

// tree[i] = merge(tree[i * 2], tree[i * 2 + 1])
template <typename T>
struct MergeSortTree2DPoint {
    int       N;                // the size of array
    vector<vector<T>> tree;     // x -> { y0, y1, ... }
    vector<T> X;

    MergeSortTree2DPoint() : N(0), tree(), X() {
    }

    MergeSortTree2DPoint(const pair<int, int> v[], int n) {
        build(v, n);
    }

    explicit MergeSortTree2DPoint(const vector<pair<int, int>>& v) {
        build(v);
    }


    // O(NlogN)
    void build(const pair<T, T> v[], int n) {
        N = n;

        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [v](int a, int b) {
            return v[a].first < v[b].first;
        });

        tree = vector<vector<T>>(n * 2);
        X = vector<T>(n);

        for (int i = 0; i < n; i++) {
            tree[n + i].resize(1);
            X[i] = v[idx[i]].first;
            tree[n + i][0] = v[idx[i]].second;
        }

        for (int i = n - 1; i >= 1; i--) {
            auto& L = tree[i * 2];
            auto& R = tree[i * 2 + 1];
            tree[i].resize(L.size() + R.size());
            merge(L.begin(), L.end(), R.begin(), R.end(), tree[i].begin());
        }
    }

    // O(NlogN)
    void build(const vector<pair<T, T>>& v) {
        build(v.data(), int(v.size()));
    }


    // inclusive, O((logN)^2)
    int count(T x1, T y1, T x2, T y2) const {
        int res = 0;

        int L = int(lower_bound(X.begin(), X.end(), x1) - X.begin());
        int R = int(upper_bound(X.begin(), X.end(), x2) - X.begin()) - 1;
        for (L += N, R += N; L <= R; L >>= 1, R >>= 1) {
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
