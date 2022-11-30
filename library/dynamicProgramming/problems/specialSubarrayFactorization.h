#pragma once

/*
  https://www.hackerearth.com/problem/algorithm/beautiful-prefix-sequences-924c94e1/

  A subarray of integers { x[0], x[1], x[2], ..., x[n-1] } is called *SPECIAL SUBARRAY*
    if it can be split into contiguous subarrays such that

    - Each subarray consists of at least 3 elements
    - x[i] < x[L] and x[i] < x[R] for every i (L < i < R) in contiguous subarray { x[L], x[L + 1], ..., x[R - 1], x[R] }.

  For instance [7, 5, 3, 2, 6, 4, 1, 3, 5] is a *SPECIAL SUBARRAY*
    which can be split into [7,5,3,2,6][4,1,3,5], but [7, 5, 3, 2, 4, 1, 3, 5] is not a *SPECIAL SUBARRAY*.


  - solution: stack + BIT(or segment-tree) + DP
*/
struct SpecialSubarrayFactorization {
    template <typename T>
    struct BIT {
        vector<T> tree;

        BIT() {
        }

        explicit BIT(int n) : tree(n + 1) {
        }


        void init(int n) {
            tree = vector<T>(n + 1);
        }


        T sum(int pos) const {
            pos++;

            T res = 0;
            while (pos > 0) {
                res += tree[pos];
                pos &= pos - 1;
            }

            return res;
        }

        T sumRange(int left, int right) const {
            T res = sum(right);
            if (left > 0)
                res -= sum(left - 1);
            return res;
        }

        void add(int pos, T val) {
            pos++;

            while (pos < int(tree.size())) {
                tree[pos] += val;
                pos += pos & -pos;
            }
        }
    };


    //--- the number of prefix arrays

    // O(N*logN)
    static int countPrefixArrays(const vector<int>& A) {
        int N = int(A.size());
        int res = 0;

        vector<bool> start(N + 1);

        BIT<int> bit(N);
        vector<int> stk;

        start[0] = true;
        bit.add(0, 1);
        stk.push_back(0);
        for (int i = 1; i < N; i++) {
            int j = int(upper_bound(stk.begin(), stk.end(), i, [&A](int a, int b) {
                return A[a] > A[b];
                }) - stk.begin());

            int L = stk[max(0, j - 1)];
            int R = i - 2;
            if ((L <= R) && (bit.sumRange(L, R) > 0)) {
                start[i + 1] = true;
                res++;
            }

            while (!stk.empty() && A[stk.back()] <= A[i]) {
                if (start[stk.back()])
                    bit.add(stk.back(), -1);
                stk.pop_back();
            }
            if (start[i])
                bit.add(i, 1);
            stk.push_back(i);
        }

        return res;
    }


    //--- the number of ways to split prefix arrays

    // O(N*logN)
    static long long countWaysToSplitPrefixArrays(const vector<int>& A) {
        int N = int(A.size());
        long long res = 0;

        vector<long long> start(N + 1);
        BIT<long long> cntDP(N);
        vector<int> stk;

        start[0] = 1;
        cntDP.add(0, 1);
        stk.push_back(0);
        for (int i = 1; i < N; i++) {
            int j = int(upper_bound(stk.begin(), stk.end(), i, [&A](int a, int b) {
                return A[a] > A[b];
                }) - stk.begin());

            int L = stk[max(0, j - 1)];
            int R = i - 2;
            if (L <= R) {
                auto cnt = cntDP.sumRange(L, R);
                if (cnt > 0) {
                    start[i + 1] = cnt;
                    res += cnt;
                }
            }

            while (!stk.empty() && A[stk.back()] <= A[i]) {
                if (start[stk.back()] > 0)
                    cntDP.add(stk.back(), -start[stk.back()]);
                stk.pop_back();
            }
            if (start[i] > 0)
                cntDP.add(i, start[i]);
            stk.push_back(i);
        }

        return res;
    }

    // O(N*logN)
    static int countWaysToSplitPrefixArrays(const vector<int>& A, int mod) {
        int N = int(A.size());
        long long res = 0;

        vector<long long> start(N + 1);
        BIT<long long> cntDP(N);
        vector<int> stk;

        start[0] = 1;
        cntDP.add(0, 1);
        stk.push_back(0);
        for (int i = 1; i < N; i++) {
            int j = int(upper_bound(stk.begin(), stk.end(), i, [&A](int a, int b) {
                return A[a] > A[b];
                }) - stk.begin());

            int L = stk[max(0, j - 1)];
            int R = i - 2;
            if (L <= R) {
                auto cnt = cntDP.sumRange(L, R) % mod;
                if (cnt < 0)
                    cnt += mod;

                if (cnt != 0) {
                    start[i + 1] = cnt;
                    res += cnt;
                    if (res >= mod)
                        res -= mod;
                    else if (res < 0)
                        res += mod;
                }
            }

            while (!stk.empty() && A[stk.back()] <= A[i]) {
                if (start[stk.back()] != 0)
                    cntDP.add(stk.back(), -start[stk.back()]);
                stk.pop_back();
            }
            if (start[i] != 0)
                cntDP.add(i, start[i]);
            stk.push_back(i);
        }

        return int(res);
    }


    //--- the number of ways to split subarrays

    // O(N*logN)
    static long long countWaysToSplitSubarrays(const vector<int>& A) {
        int N = int(A.size());
        long long res = 0;

        vector<long long> start(N + 1);
        BIT<long long> cntDP(N);
        vector<int> stk;

        start[0] = 1;
        cntDP.add(0, 1);
        stk.push_back(0);
        for (int i = 1; i < N; i++) {
            int j = int(upper_bound(stk.begin(), stk.end(), i, [&A](int a, int b) {
                return A[a] > A[b];
                }) - stk.begin());

            int L = stk[max(0, j - 1)];
            int R = i - 2;
            if (L <= R) {
                auto cnt = cntDP.sumRange(L, R);
                if (cnt > 0) {
                    start[i + 1] = cnt;
                    res += cnt;
                }
            }
            start[i] += 1;

            while (!stk.empty() && A[stk.back()] <= A[i]) {
                if (start[stk.back()] > 0)
                    cntDP.add(stk.back(), -start[stk.back()]);
                stk.pop_back();
            }
            if (start[i] > 0)
                cntDP.add(i, start[i]);
            stk.push_back(i);
        }

        return res;
    }

    // O(N*logN)
    static int countWaysToSplitSubarrays(const vector<int>& A, int mod) {
        int N = int(A.size());
        long long res = 0;

        vector<long long> start(N + 1);
        BIT<long long> cntDP(N);
        vector<int> stk;

        start[0] = 1;
        cntDP.add(0, 1);
        stk.push_back(0);
        for (int i = 1; i < N; i++) {
            int j = int(upper_bound(stk.begin(), stk.end(), i, [&A](int a, int b) {
                return A[a] > A[b];
                }) - stk.begin());

            int L = stk[max(0, j - 1)];
            int R = i - 2;
            if (L <= R) {
                auto cnt = cntDP.sumRange(L, R) % mod;
                if (cnt < 0)
                    cnt += mod;

                if (cnt != 0) {
                    start[i + 1] = cnt;
                    res += cnt;
                    if (res >= mod)
                        res -= mod;
                    else if (res < 0)
                        res += mod;
                }
            }
            start[i] += 1;

            while (!stk.empty() && A[stk.back()] <= A[i]) {
                if (start[stk.back()] != 0)
                    cntDP.add(stk.back(), -start[stk.back()]);
                stk.pop_back();
            }
            if (start[i] != 0)
                cntDP.add(i, start[i]);
            stk.push_back(i);
        }

        return int(res);
    }
};
