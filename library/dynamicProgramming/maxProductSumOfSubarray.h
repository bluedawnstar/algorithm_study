#pragma once

// https://www.hackerrank.com/contests/hourrank-26/challenges/pair-sums/problem

// max{ product-sum of all pairs in S }, S = any of noneempty subarray
struct AllPairMaxProductSumOfSubarray {
    int N;
    vector<int> A;
    vector<long long> prefixSum;
    vector<long long> prefixSquareSum;

    void build(const vector<int>& A) {
        N = int(A.size());
        this->A = A;

        prefixSum.resize(N + 1);
        prefixSquareSum.resize(N + 1);
        for (int i = 0; i < N; i++) {
            prefixSum[i + 1] = prefixSum[i] + A[i];
            prefixSquareSum[i + 1] = prefixSquareSum[i] + 1ll * A[i] * A[i];
        }
    }

    long long solve() {
        return dfs(0, N - 1) / 2;
    }

private:
    long long dfs(int L, int R) {
        if (L >= R)
            return numeric_limits<long long>::min();
        else if (L + 1 == R)
            return 2ll * A[L] * A[R];

        int mid = L + (R - L) / 2;

        // make m[L..mid]
        vector<int> idxL(mid - L + 1);
        iota(idxL.begin(), idxL.end(), L);
        sort(idxL.begin(), idxL.end(), [this](int l, int r) {
            return prefixSum[l] > prefixSum[r];
        });

        // query x[mid + 1..R]
        vector<int> idxR(R - mid);
        iota(idxR.begin(), idxR.end(), mid + 2);
        sort(idxR.begin(), idxR.end(), [this](int l, int r) {
            return prefixSum[l] < prefixSum[r];
        });

        DPConvexHullTrickMin<long long> cht;
        for (int i : idxL)
            cht.insert(2ll * prefixSum[i], -(prefixSum[i] * prefixSum[i] + prefixSquareSum[i]));

        long long ans = 0;
        for (int i : idxR) {
            ans = max(ans, (prefixSum[i] * prefixSum[i] - prefixSquareSum[i]) - cht.query(prefixSum[i]));
        }

        // left half
        ans = max(ans, dfs(L, mid));
        // right half
        ans = max(ans, dfs(mid + 1, R));

        return ans;
    }
};
