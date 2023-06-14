#pragma once

// https://www.hackerearth.com/practice/algorithms/searching/linear-search/practice-problems/algorithm/and-subsequence-9682a719/

struct SubsetAnd {
    /*
        longest subsequence when { AND A[i] } >= X, O(N * log(maxX))
                                 i in subset of {0,1,2,...}
    */
    static int longestSubsequenceGreaterThanOrEqual(const int A[], int N, int X, int bitSize) {
        // equal to X
        int res = max(res, longestSubsequenceGreaterThanOrEqual(A, N, X));

        // greater than X
        int baseX = 0;
        for (int bit = bitSize - 1; bit >= 0; bit--) {
            if (X & (1 << bit)) {
                baseX |= 1 << bit;
                continue;
            }
            res = max(res, longestSubsequenceGreaterThanOrEqual(A, N, baseX | (1 << bit)));
        }

        return res;
    }

    static int longestSubsequenceGreaterThanOrEqual(const vector<int>& A, int X, int bitSize) {
        return longestSubsequenceGreaterThanOrEqual(A.data(), int(A.size()), X, bitSize);
    }

private:
    // O(N)
    static int longestSubsequenceGreaterThanOrEqual(const int A[], int N, int mask) {
        int res = 0;
        for (int i = 0; i < N; i++) {
            if ((A[i] & mask) == mask)
                res++;
        }

        return res;
    }
};
