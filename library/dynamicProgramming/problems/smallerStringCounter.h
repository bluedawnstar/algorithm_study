#pragma once

// https://www.hackerearth.com/practice/algorithms/dynamic-programming/2-dimensional/practice-problems/algorithm/good-strings-4-9f9d2d4b/

template <int mod = 1'000'000'007>
struct SmallerStringCounter {
    // 'a' <= s[i] <= 'z'
    // O(N*K)
    static int countSmallerStringsWithExactKDifferent(const string& s, int K) {
        int N = int(s.length());

        vector<vector<int>> dp(N + 1, vector<int>(K + 1));  // dp[i][k], 1 <= i <= N, 0 <= k <= K
        dp[0][0] = 1;
        for (int i = 1; i <= N; i++) {
            dp[i][0] = 1;
            // first different character
            dp[i][1] = (dp[i - 1][1] + 1ll * dp[i - 1][0] * (s[i - 1] - 'a')) % mod;
            for (int j = 2; j <= K; j++) {
                dp[i][j] = (dp[i - 1][j] + dp[i - 1][j - 1] * 25ll) % mod;
            }
        }

        return dp[N][K];
    }
};
