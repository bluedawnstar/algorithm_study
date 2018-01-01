#pragma once

// include empty subsequence, O(N)
inline int countDistinctSubsequence(string s) {
    int n = (int)s.length();
    if (n == 0)
        return 0;

    vector<int> last(256, -1);
    vector<int> dp(n + 1);

    dp[0] = 1;
    for (int i = 1; i <= n; i++) {
        dp[i] = 2 * dp[i - 1];
        if (last[s[i - 1]] != -1)
            dp[i] -= dp[last[s[i - 1]]];

        last[s[i - 1]] = (i - 1);
    }

    return dp[n];
}

// O(N*M)
inline int countDistinctSubsequence(string s, string t) {
    if (s.empty() || t.empty())
        return 0;

    int n = (int)s.length();
    int m = (int)t.length();

    vector<vector<int>> dp(n + 1, vector<int>(m + 1));

    dp[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        dp[i][0] = 1;
        for (int j = 1; j <= m; j++) {
            if (s[i - 1] == t[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
            else
                dp[i][j] = dp[i - 1][j];
        }
    }

    return dp[n][m];
}
