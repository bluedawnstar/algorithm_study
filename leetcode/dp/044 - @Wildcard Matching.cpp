class Solution_Memoization {
public:
    int dfs(vector<vector<int>>& dp, const string& s, int i, const string& p, int j) {
        int& res = dp[i][j];

        if (dp[i][j] >= 0)
            return res;

        while (i < (int)s.length() && j < (int)p.length() && p[j] != '*') {
            if (p[j] != '?' && s[i] != p[j])
                return res = 0;
            i++;
            j++;
        }

        if (j >= (int)p.length())
            return res = (i >= (int)s.length());
        else if (p[j] != '*')
            return res = 0;

        return res = (i < (int)s.length() && dfs(dp, s, i + 1, p, j)) || dfs(dp, s, i, p, j + 1);
    }

    bool isMatch(string s, string p) {
        vector<vector<int>> dp(s.length() + 1, vector<int>(p.length() + 1, -1));
        dp[s.length()][p.length()] = 1;
        return dfs(dp, s, 0, p, 0) == 1;
    }
};

class Solution_DP {
public:
    bool isMatch(string s, string p) {
        int n = (int)s.length();
        int m = (int)p.length();

        vector<vector<bool>> dp(n + 1, vector<bool>(m + 1));

        dp[0][0] = true;
        for (int j = 1; j <= m && p[j - 1] == '*'; j++)
            dp[0][j] = true;

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (p[j - 1] == '?' || s[i - 1] == p[j - 1])
                    dp[i][j] = dp[i - 1][j - 1];
                else if (p[j - 1] == '*')
                    dp[i][j] = dp[i - 1][j - 1] || dp[i][j - 1] || dp[i - 1][j];
                else
                    dp[i][j] = false;
            }
        }

        return dp[n][m];
    }
};
