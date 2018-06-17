class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        int n = (int)s1.length();
        int m = (int)s2.length();
        if ((n + m) != (int)s3.length())
            return false;

        vector<vector<bool>> dp(n + 1, vector<bool>(m + 1));

        dp[0][0] = true;
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j <= m; j++) {
                if (!dp[i][j])
                    continue;
                if (i < n && s1[i] == s3[i + j])
                    dp[i + 1][j] = true;
                if (j < m && s2[j] == s3[i + j])
                    dp[i][j + 1] = true;
            }
        }

        return dp[n][m];
    }
};
