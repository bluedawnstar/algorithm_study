#pragma once

struct LongestIncreasingSubsequenceOnline {
    int N;
    int maxX;
    vector<int> X;

    // O(N*MAXX + N^2)
    void init(const int A[], int n, int maxX) {
        this->N = n;
        this->maxX = maxX;
        X.assign(A, A + n);

        int root = segTree.build(N);

        maxLIS = vector<int>(N);
        dag = vector<vector<int>>(N);

        vector<int> last(maxX + 1, -1);
        for (int i = 0; i < N; i++) {
            last[X[i]] = i;

            int prev = -1;
            for (int j = X[i] - 1; j > 0; j--) {
                if (last[j] > prev) {
                    prev = last[j];
                    dag[last[j]].push_back(i);
                }
            }
        }

        roots.resize(N);
        for (int i = N - 1; i >= 0; i--) {
            root = update(root, i, 1);
            roots[i] = root;
        }
    }

    void init(const vector<int>& A, int maxX) {
        init(A.data(), int(A.size()), maxX);
    }

    //---

    // 0 <= L <= R < N, inclusive
    // O(logN)
    int query(int L, int R) const {
        return segTree.query(roots[L], 0, R);
    }

private:
    struct SimplePersistentSegmentTreeMax {
        int         N;
        vector<int> value;
        vector<int> L;
        vector<int> R;
        int         initRoot;

        SimplePersistentSegmentTreeMax() {
        }

        explicit SimplePersistentSegmentTreeMax(int n) {
            build(n);
        }

        int build(int n) {
            N = n;
            value.reserve(N * 4);
            L.reserve(N * 4);
            R.reserve(N * 4);

            return initRoot = dfsBuild(0, N - 1);
        }

        //---

        int update(int root, int index, int val) {
            return dfsUpdate(root, 0, N - 1, index, val);
        }

        int query(int root, int left, int right) const {
            return dfsQuery(root, 0, N - 1, left, right);
        }

    private:
        int dfsBuild(int left, int right) {
            int l = -1, r = -1;
            if (left < right) {
                int mid = (left + right) >> 1;
                l = dfsBuild(left, mid);
                r = dfsBuild(mid + 1, right);
            }
            value.push_back(0);
            L.push_back(l);
            R.push_back(r);
            return int(value.size()) - 1;
        }

        int dfsUpdate(int node, int left, int right, int index, int val) {
            int newValue;
            int l = -1, r = -1;
            if (left == right) {
                newValue = val;
            } else {
                int mid = (left + right) >> 1;
                if (index <= mid) {
                    l = dfsUpdate(L[node], left, mid, index, val);
                    r = R[node];
                } else {
                    l = L[node];
                    r = dfsUpdate(R[node], mid + 1, right, index, val);
                }
                newValue = max(value[l], value[r]);
            }
            value.push_back(newValue);
            L.push_back(l);
            R.push_back(r);
            return int(value.size()) - 1;
        }

        int dfsQuery(int node, int left, int right, int indexL, int indexR) const {
            if (indexR < left || right < indexL)
                return 0;

            if (indexL <= left && right <= indexR)
                return value[node];

            int mid = (left + right) >> 1;
            return max(dfsQuery(L[node], left, mid, indexL, indexR),
                dfsQuery(R[node], mid + 1, right, indexL, indexR));
        }
    };

    vector<int> maxLIS;
    vector<vector<int>> dag;
    SimplePersistentSegmentTreeMax segTree;
    vector<int> roots;

    int update(int root, int u, int val) {
        if (val > maxLIS[u]) {
            for (auto v : dag[u])
                root = update(root, v, val + 1);
            maxLIS[u] = val;
            root = segTree.update(root, u, val);
        }
        return root;
    }
};
