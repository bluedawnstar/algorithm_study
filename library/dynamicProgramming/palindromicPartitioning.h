#pragma once

struct PalindromicPartitioning {
    /*
        https://leetcode.com/problems/palindrome-partitioning-ii/

        dp[i] = min cut in [0, i]

        1) dp[i] = 0                     , [0, i] is a palindrom

        2) dp[i] = MIN { dp[j - 1] + 1 } , [j, i] is a palindrom
                  0<j<=i
    */
    // O(N^2)
    static int minCut(string s) {
        int N = int(s.length());
        if (N <= 1)
            return 0;

        // palindromic substring
        vector<vector<bool>> pal(N, vector<bool>(N));
        pal[N - 1][N - 1] = true;
        for (int i = N - 2; i >= 0; i--) {
            pal[i][i] = true;
            pal[i][i + 1] = (s[i] == s[i + 1]);
            for (int j = i + 2; j < N; j++)
                pal[i][j] = (s[i] == s[j] && pal[i + 1][j - 1]);
        }

        vector<int> dp(N, N);
        dp[0] = 0;
        for (int i = 1; i < N; i++) {
            if (pal[0][i]) {
                dp[i] = 0;
                continue;
            }
            for (int j = i; j > 0; j--) {
                if (pal[j][i])
                    dp[i] = min(dp[i], dp[j - 1] + 1);
            }
        }

        return dp.back();
    }

    //---

    /*
        https://leetcode.com/problems/palindrome-partitioning-iii/

        change[i][j] = the min number of characters to be changed to convert substring[i, j] to a palindrome

        dp[i][k] = the min number of characters to be changed to divide substring[0, i]
                   into k non-empty disjoint substrings such that each substring is palindrome
    */
    // O(N^2*K)
    // the minimal number of characters to be changed to divide the string
    //   into k non-empty disjoint substrings such that each substring is palindrome
    static int palindromePartitionMemoization(string s, int k) {
        int N = int(s.length());

        const int INF = N;

        vector<vector<int>> change(N, vector<int>(N));
        vector<vector<int>> dp(N, vector<int>(k + 1, INF));

        for (int i = 0; i < N; i++) {
            for (int j = i; j < N; j++) {
                for (int L = i, R = j; L < R; L++, R--)
                    change[i][j] += (s[L] != s[R]);
            }
        }

        function<int(int,int)> dfs;
        dfs = [&](int start, int k) -> int {
            if (N - start < k)
                return INF;
            else if (N - start == k)
                return 0;
            else if (k == 1)
                return change[start][N - 1];

            if (dp[start][k] < INF)
                return dp[start][k];

            int res = INF;
            for (int i = start + 1; i < N; i++)
                res = min(res, change[start][i - 1] + dfs(i, k - 1));        

            return dp[start][k] = res;
        };

        return dfs(0, k);
    }

    static int palindromePartitionDP(string s, int k) {
        int N = int(s.length());

        const int INF = N;

        vector<vector<int>> change(N, vector<int>(N));
        for (int i = 0; i < N; i++) {
            for (int j = i; j < N; j++) {
                for (int L = i, R = j; L < R; L++, R--)
                    change[i][j] += (s[L] != s[R]);
            }
        }

        vector<vector<int>> dp(N, vector<int>(k + 1, INF));
        for (int i = 0; i < N; i++)
            dp[i][1] = change[0][i];
        for (int cnt = 2; cnt <= k; cnt++) {
            for (int i = cnt - 1; i < N; i++) {
                for (int j = cnt - 1; j <= i; j++)
                    dp[i][cnt] = min(dp[i][cnt], dp[j - 1][cnt - 1] + change[j][i]);
            }
        }

        return dp[N - 1][k];
    }
};
