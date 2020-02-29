#pragma once

/*
    https://www.codechef.com/problems/ARMYOFME
    https://discuss.codechef.com/t/armyofme-editorial/52233

    Maximum length subarray such that the subarray has consecutive numbers.

        value : 0, 1, 2, ..., N-1
        index : 0, 1, 2, ..., N-1

    6 |                     x
    5 |             x
    4 |     x
    3 |         x
    2 |                 x
    1 | x
    0 |                         x
      +---+---+---+---+---+---+---
        0   1   2   3   4   5   6

       query[0, 2] = 2
       query[0, 3] = 3
       query[0, 4] = 5
       query[1, 3] = 3
       query[1, 4] = 4
*/
struct MaxConsecutiveSubarray {
    struct SegmentTreeMin {
        int N;
        vector<int> values;             // min value
        vector<int> lazyAdd;

        SegmentTreeMin() {
        }

        explicit SegmentTreeMin(int n) {
            init(n);
        }

        void init(int n) {
            N = n;
            values.assign(n * 4, 0);
            lazyAdd.assign(n * 4, 0);
        }

        // [left, right]
        void add(int left, int right, int value) {
            add(left, right, value, 1, 0, N - 1);
        }

        int getFirst() const {
            return getFirst(1, 0, N - 1);
        }

        int getLast() const {
            return getLast(1, 0, N - 1);
        }

    private:
        void add(int left, int right, int value, int node, int nodeLeft, int nodeRight) {
            if (right < nodeLeft || nodeRight < left)
                return;

            if (left <= nodeLeft && nodeRight <= right) {
                values[node] += value;
                lazyAdd[node] += value;
                return;
            }

            int mid = (nodeLeft + nodeRight) >> 1;

            add(left, right, value, node * 2, nodeLeft, mid);
            add(left, right, value, node * 2 + 1, mid + 1, nodeRight);

            values[node] = min(values[node * 2], values[node * 2 + 1]) + lazyAdd[node];
        }

        int getFirst(int node, int nodeLeft, int nodeRight) const {
            if (nodeLeft == nodeRight)
                return nodeLeft;

            int mid = (nodeLeft + nodeRight) >> 1;
            if (values[node * 2] <= values[node * 2 + 1])
                return getFirst(node * 2, nodeLeft, mid);
            else
                return getFirst(node * 2 + 1, mid + 1, nodeRight);
        }

        int getLast(int node, int nodeLeft, int nodeRight) const {
            if (nodeLeft == nodeRight)
                return nodeLeft;

            int mid = (nodeLeft + nodeRight) >> 1;
            if (values[node * 2] < values[node * 2 + 1])
                return getLast(node * 2, nodeLeft, mid);
            else
                return getLast(node * 2 + 1, mid + 1, nodeRight);
        }
    };

    struct JumpPointerSparseTable {
        int N;                                  // the number of vertex
        int logN;                               // 
        vector<vector<pair<int, int>>> prev;    // (prev, length);
        vector<vector<pair<int, int>>> next;    // (next, length);

        JumpPointerSparseTable() {
        }

        explicit JumpPointerSparseTable(int N) {
            init(N);
        }

        void init(int n) {
            N = n;
            logN = int(log2(N)) + 1;
            prev.resize(logN);
            next.resize(logN);
            for (int i = 0; i < logN; i++) {
                prev[i].resize(N);
                next[i].resize(N);
            }
        }

        // [PRECONDITION] initialize prev[0][0..N-1] and next[0][0..N-1]
        // O(NlogN)
        void build() {
            for (int i = 1; i < logN; i++) {
                for (int j = 0; j < N; j++) {
                    int pp = prev[i - 1][j].first - 1;
                    prev[i][j].first = (pp < 0) ? pp : prev[i - 1][pp].first;
                    prev[i][j].second = max(prev[i - 1][j].second, (pp < 0) ? 0 : prev[i - 1][pp].second);

                    pp = next[i - 1][j].first + 1;
                    next[i][j].first = (pp >= N) ? pp : next[i - 1][pp].first;
                    next[i][j].second = max(next[i - 1][j].second, (pp >= N) ? 0 : next[i - 1][pp].second);
                }
            }
        }

        int query(int left, int right) const {
            int res = 1;

            for (int i = logN - 1; i >= 0; i--) {
                if (left < N && next[i][left].first <= right) {
                    res = max(res, next[i][left].second);
                    left = next[i][left].first + 1;
                }
            }

            for (int i = logN - 1; i >= 0; i--) {
                if (right >= 0 && prev[i][right].first >= left) {
                    res = max(res, prev[i][right].second);
                    right = prev[i][right].first - 1;
                }
            }

            res = max(res, right - left + 1);

            return res;
        }
    };

    int N;
    JumpPointerSparseTable sparseTable;

    void build(const int P[], int n) {
        N = n;
        sparseTable.init(N);
        {
            SegmentTreeMin segTree(N);
            segTree.add(0, N - 1, 1);

            vector<pair<int, int>> stkMax, stkMin;  // (value, index)
            stkMax.emplace_back(N, -1);
            stkMin.emplace_back(-1, -1);
            for (int i = 0; i < N; i++) {
                while (stkMax.back().first < P[i]) {
                    segTree.add(stkMax[stkMax.size() - 2].second + 1, stkMax.back().second, P[i] - stkMax.back().first);
                    stkMax.pop_back();
                }
                stkMax.emplace_back(P[i], i);

                while (stkMin.back().first > P[i]) {
                    segTree.add(stkMin[stkMin.size() - 2].second + 1, stkMin.back().second, stkMin.back().first - P[i]);
                    stkMin.pop_back();
                }
                stkMin.emplace_back(P[i], i);

                segTree.add(0, i, -1);

                sparseTable.prev[0][i].first = segTree.getFirst();
                sparseTable.prev[0][i].second = i - sparseTable.prev[0][i].first + 1;
            }
        }
        {
            SegmentTreeMin segTree(N);
            segTree.add(0, N - 1, 1);

            vector<pair<int, int>> stkMax, stkMin;  // (value, index)
            stkMax.emplace_back(N, N);
            stkMin.emplace_back(-1, N);
            for (int i = N - 1; i >= 0; i--) {
                while (stkMax.back().first < P[i]) {
                    segTree.add(stkMax.back().second, stkMax[stkMax.size() - 2].second - 1, P[i] - stkMax.back().first);
                    stkMax.pop_back();
                }
                stkMax.emplace_back(P[i], i);

                while (stkMin.back().first > P[i]) {
                    segTree.add(stkMin.back().second, stkMin[stkMin.size() - 2].second - 1, stkMin.back().first - P[i]);
                    stkMin.pop_back();
                }
                stkMin.emplace_back(P[i], i);

                segTree.add(i, N - 1, -1);

                sparseTable.next[0][i].first = segTree.getLast();
                sparseTable.next[0][i].second = sparseTable.next[0][i].first - i + 1;
            }
        }
        sparseTable.build();
    }

    void build(const vector<int>& P) {
        build(P.data(), int(P.size()));
    }

    int query(int left, int right) const {
        return sparseTable.query(left, right);
    }
};
