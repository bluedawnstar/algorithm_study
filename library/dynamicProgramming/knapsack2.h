#pragma once

// https://leetcode.com/problems/last-stone-weight-ii/


/*
    <minimum last value>

    0. constraints
       - All values are positive.

    1. operations
       1) A[i] == A[j] : remove both
       2) A[i] < A[j]  : remove A[i] and replace A[j] with A[j] - A[i]

    2. result
       1) A is empty     : 0
       2) A is not empty : last value
*/
struct MinLastValue {
    // meet-in-the-middle
    // O(2^(N/2) * logN)
    static long long solveWithMeetInTheMiddle(const vector<int>& A) {
        int N = int(A.size());

        int cntL = N / 2;
        int cntR = N - cntL;

        // left combinations
        vector<long long> dpL;
        for (int i = 1; i < (1 << cntL); i++) {
            long long sum = 0;
            for (int j = 0; j < cntL; j++) {
                if (i & (1 << j))
                    sum += A[j];
                else
                    sum -= A[j];
            }
            dpL.push_back(abs(sum));
        }
        sort(dpL.begin(), dpL.end());
        dpL.erase(unique(dpL.begin(), dpL.end()), dpL.end());

        // right combinations
        vector<long long> dpR;
        for (int i = 1; i < (1 << cntR); i++) {
            long long sum = 0;
            for (int j = 0; j < cntR; j++) {
                if (i & (1 << j))
                    sum += A[cntL + j];
                else
                    sum -= A[cntL + j];
            }
            dpR.push_back(abs(sum));
        }
        sort(dpR.begin(), dpR.end());
        dpR.erase(unique(dpR.begin(), dpR.end()), dpR.end());

        if (dpL.size() > dpR.size())
            swap(dpL, dpR);

        // join left and right
        long long ans = numeric_limits<long long>::max();
        for (auto x : dpL) {
            int i = int(lower_bound(dpR.begin(), dpR.end(), x) - dpR.begin());
            if (i < int(dpR.size()))
                ans = min(ans, dpR[i] - x);
            if (i > 0)
                ans = min(ans, x - dpR[i - 1]);
        }

        return ans;
    }

    // knapsack, O(N * maxValue)
    static int solveWithKnapsack(const vector<int>& A, int maxValue) {
        int N = int(A.size());

        vector<bool> possible(2 * maxValue + 1, false);
        possible[maxValue] = true;

        for (int it : A) {
            vector<bool> next(2 * maxValue + 1, false);
            for (int x = 0; x <= maxValue * 2; x++) {
                if (possible[x])
                    next[x + it] = next[x - it] = true;
            }
            swap(possible, next);
        }

        for (int i = 0; i <= maxValue; i++) {
            if (possible[maxValue + i])
                return i;
        }

        return 0;
    }
};
