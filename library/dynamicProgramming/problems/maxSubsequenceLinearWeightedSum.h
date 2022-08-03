#pragma once

#include <limits>
#include <vector>

/*
   https://www.hackerearth.com/practice/algorithms/dynamic-programming/2-dimensional/practice-problems/algorithm/maximize-array-function-662cdd83/
   Q) Find the maximum value of F when selecting K elements from array A (0 <= K <= |A|)

                 K
            F = SUM i * A[i]
                i=1

   <Solution>
     dp[i][j] = maximum value of the subarray [1, i] by selecting j elements. (1 <= i <= N)
              = [1, i] subarray에서 j개의 요소를 선택했을 때 최대 값

     dp[i][0] = 0                                                --- j == 0
     dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - 1] + j * A[i])   --- 0 < j < i
     dp[i][i] =                   dp[i - 1][i - 1] + i * A[i]    --- j == i
*/

template <typename T>
struct MaxSubsequenceLinearWeightedSum {
    // O(N^2)
    static T solve(const T A[], int N) {
        vector<vector<T>> dp(N + 1, vector<T>(N + 1, numeric_limits<T>::min()));

        dp[0][0] = 0;
        for (int i = 1; i <= N; i++) {
            dp[i][0] = 0;
            for (int j = 1; j <= i; j++)
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - 1] + j * A[i - 1]);
        }

        T res = dp[N][0];
        for (int j = 1; j <= N; j++)
            res = max(res, dp[N][j]);

        return res;
    }

    static T solve(const vector<T>& A) {
        return solve(A.data(), int(A.size()));
    }
};
