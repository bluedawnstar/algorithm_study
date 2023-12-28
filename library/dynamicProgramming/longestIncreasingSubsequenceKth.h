#pragma once

struct KthLIS {
    // O(N*logN)
    template <typename T>
    static long long countLIS(const vector<T>& A) {
        int N = int(A.size());

        vector<T> dp;                           // 
        vector<vector<pair<T, int>>> dp2;       // dp2[i] = { (value, index), ... }

        auto compValue = [](const pair<T, int>& a, const pair<T, int>& b) {
            return a.first > b.first;
        };

        // step #1 : make reverse DAG (forward)
        vector<vector<pair<int, int>>> prev;    // prev[i] = { (first, last), ... }
        for (int i = 0, j = 0; i < N; i++) {
            if (dp.empty() || dp.back() < A[i]) {
                j = int(dp.size());
                dp.push_back(A[i]);
                dp2.push_back({});
                prev.push_back({});
            } else {
                j = int(lower_bound(dp.begin(), dp.end(), A[i]) - dp.begin());
                dp[j] = A[i];
            }

            if (j > 0) {
                int first = int(upper_bound(dp2[j - 1].begin(), dp2[j - 1].end(), make_pair(A[i], N), compValue) - dp2[j - 1].begin());
                int last = int(dp2[j - 1].size());
                prev[j].emplace_back(first, last);
            }
            dp2[j].emplace_back(A[i], i);
        }

        // step #2 : update the number of path (backward)
        int lisLen = int(dp.size());
        vector<vector<long long>> pathCnt(lisLen);
        pathCnt.back().assign(dp2.back().size(), 1ll);
        for (int i = lisLen - 2; i >= 0; i--) {
            SegmentTreeLazyWithBound<long long> segTree(0ll, 0x3f3f3f3f3f3f3f3fll, int(dp2[i].size()));

            for (int j = 0; j < int(dp2[i + 1].size()); j++) {
                int first, last;
                tie(first, last) = prev[i + 1][j];
                segTree.add(first, last - 1, pathCnt[i + 1][j]);
            }

            pathCnt[i].resize(dp2[i].size());
            for (int j = 0; j < int(dp2[i].size()); j++)
                pathCnt[i][j] = segTree.query(j);
        }

        return accumulate(pathCnt[0].begin(), pathCnt[0].end(), 0ll);
    }

    // https://www.acmicpc.net/problem/18838
    // O(N*logN), K >= 1
    template <typename T>
    static vector<T> findKthOfDistinctValues(const vector<T>& A, long long K) {
        int N = int(A.size());

        vector<T> dp;                           // 
        vector<vector<pair<T, int>>> dp2;       // dp2[i] = { (value, index), ... }

        auto compValue = [](const pair<T, int>& a, const pair<T, int>& b) {
            return a.first > b.first;
            };

        // step #1 : make reverse DAG (forward)
        vector<vector<pair<int, int>>> prev;    // prev[i] = { (first, last), ... }
        for (int i = 0, j = 0; i < N; i++) {
            if (dp.empty() || dp.back() < A[i]) {
                j = int(dp.size());
                dp.push_back(A[i]);
                dp2.push_back({});
                prev.push_back({});
            } else {
                j = int(lower_bound(dp.begin(), dp.end(), A[i]) - dp.begin());
                dp[j] = A[i];
            }

            if (j > 0) {
                int first = int(upper_bound(dp2[j - 1].begin(), dp2[j - 1].end(), make_pair(A[i], N), compValue) - dp2[j - 1].begin());
                int last = int(dp2[j - 1].size());
                prev[j].emplace_back(first, last);
            }
            dp2[j].emplace_back(A[i], i);
        }

        // step #2 : update the number of path (backward)
        int lisLen = int(dp.size());
        vector<vector<long long>> pathCnt(lisLen);
        pathCnt.back().assign(dp2.back().size(), 1ll);
        for (int i = lisLen - 2; i >= 0; i--) {
            SegmentTreeLazyWithBound<long long> segTree(0ll, K, int(dp2[i].size()));

            for (int j = 0; j < int(dp2[i + 1].size()); j++) {
                int first, last;
                tie(first, last) = prev[i + 1][j];
                segTree.add(first, last - 1, pathCnt[i + 1][j]);
            }

            pathCnt[i].resize(dp2[i].size());
            for (int j = 0; j < int(dp2[i].size()); j++)
                pathCnt[i][j] = segTree.query(j);
        }

        // step #3 : find the Kth path (forward)
        vector<T> res;
        pair<T, int> prevVal(numeric_limits<T>::min(), -1);
        for (int i = 0; i < lisLen; i++) {
            bool found = false;
            for (int j = int(dp2[i].size()) - 1; j >= 0; j--) {
                if (dp2[i][j].second < prevVal.second)
                    break;

                if (dp2[i][j].first <= prevVal.first || pathCnt[i][j] <= 0)
                    continue;

                if (K <= pathCnt[i][j]) {
                    prevVal = dp2[i][j];
                    res.push_back(prevVal.first);
                    found = true;
                    break;
                }
                K -= pathCnt[i][j];
            }

            if (!found)
                return {};
        }

        return res;
    }

    // https://www.hackerrank.com/challenges/super-kth-lis/problem
    // https://www.acmicpc.net/problem/18837
    // O(N*logN), K >= 1
    template <typename T>
    static vector<T> findKth(const vector<T>& A, long long K) {
        int N = int(A.size());

        vector<T> dp;                           // 
        vector<vector<pair<T, int>>> dp2;        // dp2[i] = { (value, index), ... }

        auto compValue = [](const pair<T, int>& a, const pair<T, int>& b) {
            return a.first > b.first;
            };

        auto compIndex = [](const pair<T, int>& a, const pair<T, int>& b) {
            return a.second < b.second;
        };

        // step #1 : make reverse DAG (forward)
        vector<vector<pair<int, int>>> prev;    // prev[i] = { (first, last), ... }
        for (int i = 0, j = 0; i < N; i++) {
            if (dp.empty() || dp.back() < A[i]) {
                j = int(dp.size());
                dp.push_back(A[i]);
                dp2.push_back({});
                prev.push_back({});
            } else {
                j = int(lower_bound(dp.begin(), dp.end(), A[i]) - dp.begin());
                dp[j] = A[i];
            }

            if (j > 0) {
                int first = int(upper_bound(dp2[j - 1].begin(), dp2[j - 1].end(), make_pair(A[i], N), compValue) - dp2[j - 1].begin());
                int last = int(dp2[j - 1].size());
                prev[j].emplace_back(first, last);
            }
            dp2[j].emplace_back(A[i], i);
        }

        // step #2 : update the number of path (backward)
        int lisLen = int(dp.size());
        vector<vector<long long>> pathCnt(lisLen);     // pathCnt[i] = { path_count, ... }
        pathCnt.back().assign(dp2.back().size(), 1ll);
        for (int i = lisLen - 2; i >= 0; i--) {
            SegmentTreeLazyWithBound<long long> segTree(0ll, K, int(dp2[i].size()));

            for (int j = 0; j < int(dp2[i + 1].size()); j++) {
                int first, last;
                tie(first, last) = prev[i + 1][j];
                segTree.add(first, last - 1, pathCnt[i + 1][j]);
            }

            pathCnt[i].resize(dp2[i].size());
            for (int j = 0; j < int(dp2[i].size()); j++)
                pathCnt[i][j] = segTree.query(j);
        }

        // step #3 : find the Kth path (forward)
        vector<T> res;
        vector<vector<long long>> pathRep(lisLen);  // repeat
        pathRep[0].assign(dp2[0].size(), 1ll);

        int rangeLo = 0, rangeHi = int(dp2[0].size());
        for (int i = 1; i < lisLen; i++) {
            SegmentTreeLazyWithBound<long long> segTree(0ll, K, int(dp2[i].size()));

            bool found = false;
            for (int j = rangeHi - 1; j >= rangeLo; ) {
                T val = dp2[i - 1][j].first;
                long long cnt = 0;
                vector<tuple<int, int, long long>> ranges;
                int last = int(lower_bound(dp2[i].begin(), dp2[i].end(), dp2[i - 1][j], compValue) - dp2[i].begin());
                for (; j >= 0 && dp2[i - 1][j].first == val; j--) {
                    cnt = min(K, cnt + mult(0ll, K, pathCnt[i - 1][j], pathRep[i - 1][j]));
                    int first = int(upper_bound(dp2[i].begin(), dp2[i].end(), dp2[i - 1][j], compIndex) - dp2[i].begin());
                    if (first < last)
                        ranges.emplace_back(first, last, pathRep[i - 1][j]);
                }

                if (K <= cnt) {
                    rangeLo = int(dp2[i].size());
                    rangeHi = 0;
                    for (auto& it : ranges) {
                        segTree.add(get<0>(it), get<1>(it) - 1, get<2>(it));
                        rangeLo = min(rangeLo, get<0>(it));
                        rangeHi = max(rangeHi, get<1>(it));
                    }
                    res.push_back(val);
                    found = true;
                    break;
                }
                K -= cnt;
            }
            if (!found)
                return {};

            pathRep[i].resize(dp2[i].size());
            for (int j = rangeLo; j < rangeHi; j++)
                pathRep[i][j] = segTree.query(j);
        }

        for (int j = rangeHi - 1; j >= rangeLo; j--) {
            long long cnt = mult(0ll, K, pathCnt[lisLen - 1][j], pathRep[lisLen - 1][j]);
            if (K <= cnt) {
                res.push_back(dp2[lisLen - 1][j].first);
                return res;
            }
            K -= cnt;
        }

        return {};
    }

private:
    template <typename T>
    static T add(T minX, T maxX, T a, T b) {
        T x = a + b;
        if (x > maxX)
            return maxX;
        else if (x < minX)
            return minX;
        return x;
    }

    template <typename T>
    static T mult(T minX, T maxX, T a, T b) {
        if (a != 0) {
            /*
                  a * b >= maxX
              <=> b >= maxX / a
            */
            if (b >= (maxX + a - 1) / a)
                return maxX;
            /*
                  a * b <= minX
              <=> b <= minX / a
            */
            else if (b <= minX / a)
                return minX;
        }
        return a * b;
    }

    template <typename T = long long>
    struct SegmentTreeLazyWithBound {
        int         N;
        vector<T>   tree;
        vector<T>   lazy;

        T           minX;
        T           maxX;

        SegmentTreeLazyWithBound(T minX, T maxX) : N(0), minX(minX), maxX(maxX) {
        }

        SegmentTreeLazyWithBound(T minX, T maxX, int size) {
            init(minX, maxX, size);
        }

        void init(T minX, T maxX, int size) {
            N = size;
            tree.resize(size * 4);
            lazy.resize(size * 4);
            this->minX = minX;
            this->maxX = maxX;
        }

        void clear() {
            fill(tree.begin(), tree.end(), T(0));
            fill(lazy.begin(), lazy.end(), T(0));
        }

        T query(int index) {
            return querySub(index, 1, 0, N - 1);
        }

        T add(int left, int right, T newValue) {
            return addSub(left, right, newValue, 1, 0, N - 1);
        }

    private:
        T add(T a, T b) {
            return KthLIS::add(minX, maxX, a, b);
        }

        T mult(T a, T b) {
            return KthLIS::mult(minX, maxX, a, b);
        }

        T querySub(int index, int node, int nodeLeft, int nodeRight) {
            if (index < nodeLeft || nodeRight < index)
                return 0;

            if (nodeLeft == nodeRight)
                return tree[node];

            int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
            if (lazy[node]) {
                pushDown(lazy[node], node * 2, nodeLeft, mid);
                pushDown(lazy[node], node * 2 + 1, mid + 1, nodeRight);
                lazy[node] = 0;
            }

            if (index <= mid)
                return querySub(index, node * 2, nodeLeft, mid);
            else
                return querySub(index, node * 2 + 1, mid + 1, nodeRight);
        }

        T addSub(int left, int right, T newValue, int node, int nodeLeft, int nodeRight) {
            if (right < nodeLeft || nodeRight < left)
                return tree[node];

            if (nodeLeft == nodeRight)
                return tree[node] = add(tree[node], newValue);

            if (left <= nodeLeft && nodeRight <= right) {
                lazy[node] = add(lazy[node], newValue);
                return tree[node] = add(tree[node], mult(newValue, nodeRight - nodeLeft + 1));
            }

            int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
            if (lazy[node]) {
                pushDown(lazy[node], node * 2, nodeLeft, mid);
                pushDown(lazy[node], node * 2 + 1, mid + 1, nodeRight);
                lazy[node] = 0;
            }

            return tree[node] = add(addSub(left, right, newValue, node * 2, nodeLeft, mid),
                addSub(left, right, newValue, node * 2 + 1, mid + 1, nodeRight));
        }

        T pushDown(T newValue, int node, int nodeLeft, int nodeRight) {
            if (nodeLeft == nodeRight)
                return tree[node] = add(tree[node], newValue);
            lazy[node] = add(lazy[node], newValue);
            return tree[node] = add(tree[node], mult(newValue, nodeRight - nodeLeft + 1));
        }
    };
};
