#pragma once

struct LongestIncreasingSubsequence {
    int N;
    int maxX;
    vector<int> X;

    void init(const int A[], int n, int maxX) {
        this->N = n;
        this->maxX = maxX;
        X.assign(A, A + n);
    }

    void init(const vector<int>& A, int maxX) {
        init(A.data(), int(A.size()), maxX);
    }

    //---

    // qry = { { L, R }, ... }, inclusive
    // O(N*MAXX + N^2 + Q*logN)
    vector<int> solve(const vector<pair<int, int>>& qry) {
        segTree.init(N);

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

        vector<vector<pair<int, int>>> Q(N);
        for (int i = 0; i < int(qry.size()); i++) {
            Q[qry[i].first].emplace_back(qry[i].second, i);
        }

        vector<int> res(qry.size());
        for (int i = N - 1; i >= 0; i--) {
            update(i, 1);
            for (auto& it : Q[i])
                res[it.second] = segTree.query(0, it.first);
        }

        return res;
    }

private:
    struct CompactSegmentTreeMax {
        int         RealN;
        int         N;      // the size of array
        vector<int> tree;   //

        CompactSegmentTreeMax() : RealN(0), N(0), tree() {
        }

        explicit CompactSegmentTreeMax(int size) {
            init(size);
        }

        void init(int size) {
            RealN = size;
            N = size + (size & 1);
            tree.assign(N * 2, 0);
        }

        int query(int left, int right) const {
            int resL = 0;
            int resR = 0;

            for (int L = left + N, R = right + N + 1; L < R; L >>= 1, R >>= 1) {
                if (L & 1)
                    resL = max(resL, tree[L++]);
                if (R & 1)
                    resR = max(tree[--R], resR);
            }

            return max(resL, resR);
        }

        void update(int index, int newValue) {
            tree[index + N] = newValue;

            for (int i = (index + N) >> 1; i > 0; i >>= 1)
                tree[i] = max(tree[i << 1], tree[(i << 1) | 1]);
        }
    };

    vector<int> maxLIS;
    vector<vector<int>> dag;
    CompactSegmentTreeMax segTree;

    void update(int cur, int val) {
        if (val > maxLIS[cur]) {
            for (auto x : dag[cur])
                update(x, val + 1);
            maxLIS[cur] = val;
            segTree.update(cur, val);
        }
    }
};
