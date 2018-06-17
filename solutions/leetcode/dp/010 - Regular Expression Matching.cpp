class Solution {
public:
    bool isMatch(string s, string p) {
        string pch, ps;
        for (int i = 0; i < (int)p.length(); ) {
            pch.push_back(p[i++]);
            if (i < (int)p.length() && p[i] == '*')
                ps.push_back(p[i++]);
            else
                ps.push_back(' ');
        }

        int n = (int)s.length();
        int m = (int)pch.length();

        vector<vector<bool>> dp(n + 1, vector<bool>(m + 1));

        dp[0][0] = true;
        for (int i = 1; i <= m && ps[i - 1] == '*'; i++)
            dp[0][i] = true;

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                bool matched = (pch[j - 1] == '.' || s[i - 1] == pch[j - 1]);
                if (ps[j - 1] == '*')
                    dp[i][j] = (matched && (dp[i - 1][j - 1] || dp[i - 1][j])) || dp[i][j - 1];
                else
                    dp[i][j] = matched && (dp[i - 1][j - 1] || (j > 1 && ps[j - 2] == '*') && dp[i - 1][j - 2]);
            }
        }

        return dp[n][m];
    }
};