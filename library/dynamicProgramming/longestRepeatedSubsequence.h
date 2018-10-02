#pragma once

// "aabebcdd" -> "abd" x 2

struct LongestRepeatedSubsequence {
    // O(N^2)
    static int findLength(const char s[], int n) {
        vector<vector<int>> dp(n + 1, vector<int>(n + 1));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (s[i - 1] == s[j - 1] && i != j)
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                else
                    dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
            }
        }
        return dp[n][n];
    }

    static int findLength(const string& s) {
        return findLength(s.data(), int(s.length()));
    }

    //---

    // O(N^2)
    static string find(const char s[], int n) {
        vector<vector<int>> dp(n + 1, vector<int>(n + 1));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (s[i - 1] == s[j - 1] && i != j)
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                else
                    dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
            }
        }

        string res;
        res.reserve(dp[n][n]);

        int i = n, j = n;
        while (i > 0 && j > 0) {
            if (dp[i][j] == dp[i - 1][j - 1] + 1) {
                res += s[i - 1];
                i--;
                j--;
            } else if (dp[i][j] == dp[i - 1][j])
                i--;
            else
                j--;
        }
        reverse(res.begin(), res.end());
        return res;
    }

    static string find(const string& s) {
        return find(s.data(), int(s.length()));
    }
};
